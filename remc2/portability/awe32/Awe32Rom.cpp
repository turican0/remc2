#include "Awe32Rom.h"

#include "../port_filesystem.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace
{
	const char* kPackedFile = "data.binz";
	const char* kRomFile = "data.bin";
	const long kRomSize = 1048576;
	// An AWEDUMP image lacks the first word; snd_emu8k.c accepts both.
	const long kRomSizeMin = 1048574;
	const char* kRomSha256 = "4e143b94f758734f594ded78f4e5115635975c16fa37fabeae0baa4938ca710e";

	// SHA-256 (FIPS 180-4)
	class Sha256
	{
	public:
		Sha256()
		{
			static const uint32_t init[8] = {
				0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
			memcpy(m_h, init, sizeof(m_h));
		}
		void Update(const uint8_t* p, size_t n)
		{
			while (n > 0)
			{
				const size_t take = (64 - m_fill) < n ? (64 - m_fill) : n;
				memcpy(m_block + m_fill, p, take);
				m_fill += take; p += take; n -= take; m_total += take;
				if (m_fill == 64) { Compress(); m_fill = 0; }
			}
		}
		std::string HexDigest()
		{
			const uint64_t bits = m_total * 8;
			const uint8_t pad = 0x80;
			Update(&pad, 1);
			const uint8_t zero = 0;
			while (m_fill != 56) Update(&zero, 1);
			uint8_t len[8];
			for (int i = 0; i < 8; i++) len[i] = (uint8_t)(bits >> (56 - 8 * i));
			Update(len, 8);
			static const char hex[] = "0123456789abcdef";
			std::string out;
			for (int i = 0; i < 8; i++)
				for (int s = 28; s >= 0; s -= 4) out += hex[(m_h[i] >> s) & 0xF];
			return out;
		}
	private:
		static uint32_t Ror(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
		void Compress()
		{
			static const uint32_t k[64] = {
				0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
				0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
				0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
				0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
				0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
				0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
				0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
				0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2 };
			uint32_t w[64];
			for (int i = 0; i < 16; i++)
				w[i] = ((uint32_t)m_block[i * 4] << 24) | ((uint32_t)m_block[i * 4 + 1] << 16) | ((uint32_t)m_block[i * 4 + 2] << 8) | m_block[i * 4 + 3];
			for (int i = 16; i < 64; i++)
			{
				const uint32_t s0 = Ror(w[i - 15], 7) ^ Ror(w[i - 15], 18) ^ (w[i - 15] >> 3);
				const uint32_t s1 = Ror(w[i - 2], 17) ^ Ror(w[i - 2], 19) ^ (w[i - 2] >> 10);
				w[i] = w[i - 16] + s0 + w[i - 7] + s1;
			}
			uint32_t a = m_h[0], b = m_h[1], c = m_h[2], d = m_h[3], e = m_h[4], f = m_h[5], g = m_h[6], hh = m_h[7];
			for (int i = 0; i < 64; i++)
			{
				const uint32_t t1 = hh + (Ror(e, 6) ^ Ror(e, 11) ^ Ror(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i];
				const uint32_t t2 = (Ror(a, 2) ^ Ror(a, 13) ^ Ror(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
				hh = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
			}
			m_h[0] += a; m_h[1] += b; m_h[2] += c; m_h[3] += d; m_h[4] += e; m_h[5] += f; m_h[6] += g; m_h[7] += hh;
		}
		uint32_t m_h[8];
		uint8_t m_block[64];
		size_t m_fill = 0;
		uint64_t m_total = 0;
	};

	long FileSize(const std::string& path)
	{
		std::error_code ec;
		const auto n = std::filesystem::file_size(path, ec);
		return ec ? -1 : (long)n;
	}

	std::string DataSha256(const std::vector<uint8_t>& data)
	{
		Sha256 sha;
		sha.Update(data.data(), data.size());
		return sha.HexDigest();
	}

	std::string FileSha256(const std::string& path)
	{
		FILE* f = fopen(path.c_str(), "rb");
		if (f == nullptr) return std::string();
		Sha256 sha;
		std::vector<uint8_t> buf(65536);
		size_t n;
		while ((n = fread(buf.data(), 1, buf.size(), f)) > 0) sha.Update(buf.data(), n);
		fclose(f);
		return sha.HexDigest();
	}

	bool ReadFile(const std::string& path, std::vector<uint8_t>& data)
	{
		FILE* f = fopen(path.c_str(), "rb");
		if (f == nullptr) return false;
		data.clear();
		std::vector<uint8_t> buf(65536);
		size_t n;
		while ((n = fread(buf.data(), 1, buf.size(), f)) > 0) data.insert(data.end(), buf.begin(), buf.begin() + n);
		fclose(f);
		return true;
	}

	// DEFLATE decoder (RFC 1951) after puff.c of zlib; remc2 does not link zlib.
	class Inflater
	{
	public:
		Inflater(const uint8_t* in, size_t size, std::vector<uint8_t>& out) : m_in(in), m_size(size), m_out(out) {}

		bool Run()
		{
			int last;
			do
			{
				last = Bits(1);
				const int type = Bits(2);
				if (type == 0) Stored();
				else if (type == 1) Fixed();
				else if (type == 2) Dynamic();
				else m_failed = true;
			} while (!last && !m_failed);
			return !m_failed;
		}

		// Input bytes used, the last one partly.
		size_t Consumed() const { return m_pos; }

	private:
		struct Huffman
		{
			uint16_t count[16];
			uint16_t symbol[320];
		};

		int Bits(int need)
		{
			uint32_t val = m_bitBuf;
			while (m_bitCount < need)
			{
				if (m_pos >= m_size) { m_failed = true; return 0; }
				val |= (uint32_t)m_in[m_pos++] << m_bitCount;
				m_bitCount += 8;
			}
			m_bitBuf = val >> need;
			m_bitCount -= need;
			return (int)(val & ((1u << need) - 1));
		}

		void Stored()
		{
			m_bitBuf = 0;
			m_bitCount = 0;
			if (m_pos + 4 > m_size) { m_failed = true; return; }
			const unsigned len = m_in[m_pos] | (m_in[m_pos + 1] << 8);
			const unsigned nlen = m_in[m_pos + 2] | (m_in[m_pos + 3] << 8);
			m_pos += 4;
			if (len != (~nlen & 0xFFFF) || m_pos + len > m_size) { m_failed = true; return; }
			m_out.insert(m_out.end(), m_in + m_pos, m_in + m_pos + len);
			m_pos += len;
		}

		// Returns the number of missing codes (incomplete code), negative when over-subscribed.
		static int Construct(Huffman& h, const uint8_t* length, int n)
		{
			memset(h.count, 0, sizeof(h.count));
			for (int i = 0; i < n; i++) h.count[length[i]]++;
			if (h.count[0] == n) return 0;
			int left = 1;
			for (int len = 1; len < 16; len++)
			{
				left <<= 1;
				left -= h.count[len];
				if (left < 0) return left;
			}
			uint16_t offs[16];
			offs[1] = 0;
			for (int len = 1; len < 15; len++) offs[len + 1] = offs[len] + h.count[len];
			for (int i = 0; i < n; i++)
				if (length[i] != 0) h.symbol[offs[length[i]]++] = (uint16_t)i;
			return left;
		}

		int Decode(const Huffman& h)
		{
			int code = 0, first = 0, index = 0;
			for (int len = 1; len < 16; len++)
			{
				code |= Bits(1);
				if (m_failed) return -1;
				const int count = h.count[len];
				if (code - count < first) return h.symbol[index + (code - first)];
				index += count;
				first += count;
				first <<= 1;
				code <<= 1;
			}
			m_failed = true;
			return -1;
		}

		void Codes(const Huffman& lencode, const Huffman& distcode)
		{
			static const uint16_t lbase[29] = {
				3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
			static const uint8_t lext[29] = {
				0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
			static const uint16_t dbase[30] = {
				1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769,
				1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
			static const uint8_t dext[30] = {
				0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
			for (;;)
			{
				int symbol = Decode(lencode);
				if (m_failed) return;
				if (symbol < 256) { m_out.push_back((uint8_t)symbol); continue; }
				if (symbol == 256) return;
				symbol -= 257;
				if (symbol >= 29) { m_failed = true; return; }
				const size_t len = lbase[symbol] + Bits(lext[symbol]);
				symbol = Decode(distcode);
				if (m_failed || symbol >= 30) { m_failed = true; return; }
				const size_t dist = dbase[symbol] + Bits(dext[symbol]);
				if (m_failed || dist > m_out.size()) { m_failed = true; return; }
				const size_t from = m_out.size() - dist;
				for (size_t i = 0; i < len; i++) m_out.push_back(m_out[from + i]);
			}
		}

		void Fixed()
		{
			static bool built = false;
			static Huffman lencode, distcode;
			if (!built)
			{
				uint8_t lengths[288];
				int s = 0;
				for (; s < 144; s++) lengths[s] = 8;
				for (; s < 256; s++) lengths[s] = 9;
				for (; s < 280; s++) lengths[s] = 7;
				for (; s < 288; s++) lengths[s] = 8;
				Construct(lencode, lengths, 288);
				for (s = 0; s < 30; s++) lengths[s] = 5;
				Construct(distcode, lengths, 30);
				built = true;
			}
			Codes(lencode, distcode);
		}

		void Dynamic()
		{
			static const uint8_t order[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
			uint8_t lengths[320];
			const int nlen = Bits(5) + 257;
			const int ndist = Bits(5) + 1;
			const int ncode = Bits(4) + 4;
			if (m_failed || nlen > 286 || ndist > 30) { m_failed = true; return; }

			int index = 0;
			for (; index < ncode; index++) lengths[order[index]] = (uint8_t)Bits(3);
			for (; index < 19; index++) lengths[order[index]] = 0;
			Huffman lencode, distcode;
			if (m_failed || Construct(lencode, lengths, 19) != 0) { m_failed = true; return; }

			index = 0;
			while (index < nlen + ndist)
			{
				int symbol = Decode(lencode);
				if (m_failed) return;
				if (symbol < 16) { lengths[index++] = (uint8_t)symbol; continue; }
				uint8_t len = 0;
				int repeat;
				if (symbol == 16)
				{
					if (index == 0) { m_failed = true; return; }
					len = lengths[index - 1];
					repeat = 3 + Bits(2);
				}
				else if (symbol == 17) repeat = 3 + Bits(3);
				else repeat = 11 + Bits(7);
				if (m_failed || index + repeat > nlen + ndist) { m_failed = true; return; }
				while (repeat--) lengths[index++] = len;
			}
			if (lengths[256] == 0) { m_failed = true; return; }

			int err = Construct(lencode, lengths, nlen);
			if (err < 0 || (err > 0 && nlen - lencode.count[0] != 1)) { m_failed = true; return; }
			err = Construct(distcode, lengths + nlen, ndist);
			if (err < 0 || (err > 0 && ndist - distcode.count[0] != 1)) { m_failed = true; return; }
			Codes(lencode, distcode);
		}

		const uint8_t* m_in;
		size_t m_size;
		size_t m_pos = 0;
		uint32_t m_bitBuf = 0;
		int m_bitCount = 0;
		bool m_failed = false;
		std::vector<uint8_t>& m_out;
	};

	uint32_t Crc32(const std::vector<uint8_t>& data)
	{
		static uint32_t table[256];
		static bool built = false;
		if (!built)
		{
			for (uint32_t i = 0; i < 256; i++)
			{
				uint32_t c = i;
				for (int k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
				table[i] = c;
			}
			built = true;
		}
		uint32_t crc = 0xFFFFFFFFu;
		for (const uint8_t b : data) crc = table[(crc ^ b) & 0xFF] ^ (crc >> 8);
		return crc ^ 0xFFFFFFFFu;
	}

	bool IsGzip(const std::vector<uint8_t>& data)
	{
		return data.size() >= 18 && data[0] == 0x1F && data[1] == 0x8B;
	}

	// One gzip member (RFC 1952), checked by its CRC-32 and size.
	bool Gunzip(const std::vector<uint8_t>& in, std::vector<uint8_t>& out, std::string& error)
	{
		if (in[2] != 8) { error = "unknown gzip compression method"; return false; }
		const uint8_t flags = in[3];
		size_t pos = 10;
		if (flags & 4)
		{
			if (pos + 2 > in.size()) { error = "damaged gzip header"; return false; }
			pos += 2 + (in[pos] | (in[pos + 1] << 8));
		}
		for (const uint8_t text : { (uint8_t)8, (uint8_t)16 })
			if (flags & text)
			{
				while (pos < in.size() && in[pos] != 0) pos++;
				pos++;
			}
		if (flags & 2) pos += 2;
		if (pos + 8 > in.size()) { error = "damaged gzip header"; return false; }

		out.clear();
		out.reserve(kRomSize);
		Inflater inflater(in.data() + pos, in.size() - pos, out);
		if (!inflater.Run()) { error = "damaged gzip data"; return false; }
		pos += inflater.Consumed();
		if (pos + 8 > in.size()) { error = "the gzip data is cut off"; return false; }
		const uint32_t crc = in[pos] | (in[pos + 1] << 8) | (in[pos + 2] << 16) | ((uint32_t)in[pos + 3] << 24);
		const uint32_t size = in[pos + 4] | (in[pos + 5] << 8) | (in[pos + 6] << 16) | ((uint32_t)in[pos + 7] << 24);
		if (size != (uint32_t)out.size() || crc != Crc32(out)) { error = "the gzip data does not match its checksum"; return false; }
		return true;
	}

	bool Writable(const std::filesystem::path& dir)
	{
		std::error_code ec;
		std::filesystem::create_directories(dir, ec);
		if (!std::filesystem::is_directory(dir, ec)) return false;
		const std::string probe = (dir / ".awe32rom-write-test").string();
		FILE* f = fopen(probe.c_str(), "wb");
		if (f == nullptr) return false;
		fclose(f);
		remove(probe.c_str());
		return true;
	}

	// Next to remc2, or in the user data folder of remc2 when that one is read-only (installed builds).
	std::filesystem::path TargetDir(const std::string& folder)
	{
		const std::filesystem::path f(folder);
		if (f.is_absolute()) return f;
		std::vector<std::filesystem::path> dirs;
		dirs.push_back(std::filesystem::path(get_exe_path()) / f);
#if defined(__linux__) || defined(__APPLE__)
		if (const char* xdg = std::getenv("XDG_DATA_HOME"))
			dirs.push_back(std::filesystem::path(xdg) / "remc2" / f);
		if (const char* home = std::getenv("HOME"))
			dirs.push_back(std::filesystem::path(home) / ".local" / "share" / "remc2" / f);
#else
		const char* homeDrive = std::getenv("HOMEDRIVE");
		const char* homePath = std::getenv("HOMEPATH");
		if (homeDrive && homePath)
			dirs.push_back(std::filesystem::path(std::string(homeDrive) + "/" + std::string(homePath)) / "remc2" / f);
#endif
		for (const auto& dir : dirs)
			if (Writable(dir))
				return dir;
		return std::filesystem::path();
	}

#ifdef _WIN32
	// urlmon is loaded at run time, so the build needs no new library.
	bool DownloadFile(const std::string& url, const std::string& dest, std::string& error)
	{
		typedef HRESULT(WINAPI* URLDownloadToFileA_t)(void*, const char*, const char*, DWORD, void*);
		HMODULE urlmon = LoadLibraryA("urlmon.dll");
		if (urlmon == nullptr) { error = "urlmon.dll is not available"; return false; }
		URLDownloadToFileA_t fn = (URLDownloadToFileA_t)(void*)GetProcAddress(urlmon, "URLDownloadToFileA");
		if (fn == nullptr) { FreeLibrary(urlmon); error = "URLDownloadToFileA is not available"; return false; }
		const HRESULT hr = fn(nullptr, url.c_str(), dest.c_str(), 0, nullptr);
		FreeLibrary(urlmon);
		if (FAILED(hr))
		{
			char msg[64];
			snprintf(msg, sizeof(msg), "download failed (HRESULT 0x%08lX)", (unsigned long)hr);
			error = msg;
			return false;
		}
		return true;
	}
#else
	// Runs a program without a shell and returns its exit code, 127 when it could not be started.
	int RunProgram(const char* const argv[])
	{
		const pid_t pid = fork();
		if (pid < 0) return 127;
		if (pid == 0)
		{
			execvp(argv[0], (char* const*)argv);
			_exit(127);
		}
		int status = 0;
		if (waitpid(pid, &status, 0) < 0) return 127;
		return WIFEXITED(status) ? WEXITSTATUS(status) : 127;
	}

	bool DownloadFile(const std::string& url, const std::string& dest, std::string& error)
	{
		const char* curl[] = { "curl", "-fsSL", "--max-time", "300", "-o", dest.c_str(), url.c_str(), nullptr };
		int rc = RunProgram(curl);
		if (rc == 0) return true;
		if (rc == 127)
		{
			const char* wget[] = { "wget", "-q", "-T", "300", "-O", dest.c_str(), url.c_str(), nullptr };
			rc = RunProgram(wget);
			if (rc == 0) return true;
			if (rc == 127) { error = "neither curl nor wget is installed"; return false; }
		}
		char msg[64];
		snprintf(msg, sizeof(msg), "the download tool exited with code %d", rc);
		error = msg;
		return false;
	}
#endif

	// Downloads through a temporary file; the result is kept only when it is the known image.
	bool DownloadRom(const std::filesystem::path& dir, const std::string& url, std::string& path, std::string& error)
	{
		const std::string part = (dir / (std::string(kRomFile) + ".part")).string();
		const std::string dest = (dir / kRomFile).string();
		remove(part.c_str());

		Logger->info("AWE32: downloading the wave ROM from {} to {}", url, dest);
		if (!DownloadFile(url, part, error)) { remove(part.c_str()); return false; }

		const long size = FileSize(part);
		const std::string sha = FileSha256(part);
		if (size != kRomSize || sha != kRomSha256)
		{
			Logger->warn("AWE32: the downloaded file has {} bytes, SHA-256 {} - rejected", size, sha);
			remove(part.c_str());
			error = "the downloaded file is not the AWE32 wave ROM (size or SHA-256 does not match)";
			return false;
		}
		remove(dest.c_str());
		if (rename(part.c_str(), dest.c_str()) != 0)
		{
			remove(part.c_str());
			error = "cannot rename " + part;
			return false;
		}
		path = dest;
		return true;
	}
}

std::string Awe32Rom::RawUrl(const std::string& url)
{
	const std::string github = "https://github.com/";
	const size_t blob = url.find("/blob/");
	if (url.compare(0, github.size(), github) != 0 || blob == std::string::npos)
		return url;
	return "https://raw.githubusercontent.com/" + url.substr(github.size(), blob - github.size()) + url.substr(blob + 5);
}

bool Awe32Rom::Read(const std::string& path, std::vector<uint8_t>& data, std::string& error)
{
	std::vector<uint8_t> file;
	if (!ReadFile(path, file))
	{
		error = "cannot open the wave ROM " + path;
		return false;
	}
	if (!IsGzip(file))
	{
		data = std::move(file);
		return true;
	}
	if (!Gunzip(file, data, error))
	{
		error = path + ": " + error;
		return false;
	}
	return true;
}

std::string Awe32Rom::Locate(const std::string& folder, std::string& error)
{
	const std::string dir = GetSubDirectoryPath(folder.c_str());
	if (!dir.empty())
	{
		// The packed ROM first, then the plain image.
		for (const char* file : { kPackedFile, kRomFile })
		{
			const std::string path = (std::filesystem::path(dir) / file).string();
			if (FileSize(path) < 0)
				continue;
			std::vector<uint8_t> data;
			std::string readError;
			if (!Read(path, data, readError))
			{
				Logger->warn("AWE32: {}", readError);
				continue;
			}
			if ((long)data.size() < kRomSizeMin)
			{
				Logger->warn("AWE32: {} has only {} bytes of the ROM", path, data.size());
				continue;
			}
			const std::string sha = DataSha256(data);
			if (sha != kRomSha256)
				Logger->warn("AWE32: {} is not the known ROM image (SHA-256 {}); using it anyway", path, sha);
			return path;
		}
	}

	error = "the wave ROM " + folder + "/" + kPackedFile + " or " + kRomFile + " was not found";
	return std::string();
}
