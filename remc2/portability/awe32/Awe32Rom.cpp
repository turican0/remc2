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
	const char* kRomFile = "awe32.raw";
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

std::string Awe32Rom::Locate(const std::string& folder, bool download, const std::string& url, std::string& error)
{
	const std::string dir = GetSubDirectoryPath(folder.c_str());
	if (!dir.empty())
	{
		const std::string path = (std::filesystem::path(dir) / kRomFile).string();
		if (FileSize(path) >= kRomSizeMin)
		{
			const std::string sha = FileSha256(path);
			if (sha != kRomSha256)
				Logger->warn("AWE32: {} is not the known ROM image (SHA-256 {}); using it anyway", path, sha);
			return path;
		}
	}

	if (!download)
	{
		error = "the wave ROM " + folder + "/" + kRomFile + " was not found";
		return std::string();
	}
	const std::filesystem::path target = TargetDir(folder);
	if (target.empty())
	{
		error = "no writable folder for " + folder + "/" + kRomFile;
		return std::string();
	}
	std::string path;
	if (!DownloadRom(target, RawUrl(url), path, error))
		return std::string();
	Logger->info("AWE32: wave ROM saved to {}", path);
	return path;
}
