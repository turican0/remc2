// sequence_codec.h — the sequences of the regression tests, sequence-*.binz.
//
// "MC2SEQZ1" (old): u32 frame size; per frame u32 length + runs against the previous frame:
//     varint unchanged bytes, varint changed bytes, the changed bytes.
// "MC2SEQZ4" (new): the same writes ordered by memory offset instead of by frame, packed by LZ + range coder:
//     u32 frame size, u32 frames, u32 struct size, u32 struct packed, u32 payload size, u32 payload packed,
//     packed struct, packed payload.
//   struct:  varint offsets; per offset varint offset - previous offset, varint writes,
//            per write varint frame - previous frame (the first from -1)
//   payload: per offset and write the byte minus the byte written there before (the first from 0)
// Both the conversion and the reading keep memory near the size of the writes: the conversion
// goes twice through the old file, the reading unpacks frames in blocks.
#pragma once
#include "lzcompress.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace seqz {

inline void PutVarint(std::vector<uint8_t>& out, uint32_t v)
{
	while (v >= 0x80) { out.push_back((uint8_t)(v | 0x80)); v >>= 7; }
	out.push_back((uint8_t)v);
}

inline size_t PutVarintAt(uint8_t* out, uint32_t v)
{
	size_t n = 0;
	while (v >= 0x80) { out[n++] = (uint8_t)(v | 0x80); v >>= 7; }
	out[n++] = (uint8_t)v;
	return n;
}

inline uint32_t VarintSize(uint32_t v)
{
	uint32_t n = 1;
	while (v >= 0x80) { v >>= 7; n++; }
	return n;
}

inline uint32_t GetVarint(const uint8_t* in, size_t size, size_t& p)
{
	uint32_t v = 0;
	for (int shift = 0; p < size && shift < 35; shift += 7)
	{
		const uint8_t b = in[p++];
		v |= (uint32_t)(b & 0x7F) << shift;
		if (!(b & 0x80))
			break;
	}
	return v;
}

// 1 for "MC2SEQZ1", 4 for "MC2SEQZ4", 0 for anything else
inline int Magic(const std::string& path)
{
	char magic[8] = {};
	FILE* f = fopen(path.c_str(), "rb");
	if (f == nullptr)
		return 0;
	const size_t n = fread(magic, 1, 8, f);
	fclose(f);
	if (n != 8 || memcmp(magic, "MC2SEQZ", 7) != 0)
		return 0;
	return magic[7] == '1' ? 1 : magic[7] == '4' ? 4 : 0;
}

// the old format frame by frame
class Z1Reader
{
public:
	~Z1Reader() { if (file_) fclose(file_); }

	bool Open(const std::string& path)
	{
		file_ = fopen(path.c_str(), "rb");
		char magic[8];
		if (file_ == nullptr || fread(magic, 1, 8, file_) != 8 || memcmp(magic, "MC2SEQZ1", 8) != 0
			|| fread(&frameSize_, 4, 1, file_) != 1)
			return false;
		return true;
	}

	uint32_t FrameSize() const { return frameSize_; }
	void Rewind() { fseek(file_, 12, SEEK_SET); }

	// write(offset, byte) for every byte of the next frame; false at the end of the file
	template <class Write> bool NextFrame(Write write)
	{
		uint32_t length = 0;
		if (fread(&length, 4, 1, file_) != 1)
			return false;
		buffer_.resize(length);
		if (length && fread(buffer_.data(), 1, length, file_) != length)
			return false;
		size_t p = 0;
		uint32_t pos = 0;
		while (p < length)
		{
			pos += GetVarint(buffer_.data(), length, p);
			const uint32_t changed = GetVarint(buffer_.data(), length, p);
			for (uint32_t k = 0; k < changed && p + k < length; k++)
				if (pos + k < frameSize_)
					write(pos + k, buffer_[p + k]);
			p += changed;
			pos += changed;
		}
		return true;
	}

private:
	FILE* file_ = nullptr;
	uint32_t frameSize_ = 0;
	std::vector<uint8_t> buffer_;
};

// old file -> new file
inline bool ConvertZ1ToZ4(const std::string& z1Path, const std::string& z4Path)
{
	Z1Reader z1;
	if (!z1.Open(z1Path))
		return false;
	const uint32_t size = z1.FrameSize();

	// 1st pass: how many writes every offset has and how long their frame steps are
	std::vector<uint32_t> writes(size, 0), stepBytes(size, 0);
	std::vector<int64_t> last(size, -1);
	uint32_t frames = 0;
	while (z1.NextFrame([&](uint32_t o, uint8_t) { writes[o]++; stepBytes[o] += VarintSize((uint32_t)(frames - last[o])); last[o] = frames; }))
		frames++;

	std::vector<uint8_t> structStream, payload;
	std::vector<size_t> structAt(size, 0), payloadAt(size, 0);
	uint32_t offsets = 0;
	for (uint32_t o = 0; o < size; o++)
		offsets += writes[o] != 0;
	PutVarint(structStream, offsets);
	size_t payloadSize = 0;
	for (uint32_t o = 0, previous = 0; o < size; o++)
	{
		if (!writes[o])
			continue;
		PutVarint(structStream, o - previous);
		PutVarint(structStream, writes[o]);
		previous = o;
		structAt[o] = structStream.size();
		structStream.resize(structStream.size() + stepBytes[o]);
		payloadAt[o] = payloadSize;
		payloadSize += writes[o];
	}
	payload.resize(payloadSize);

	// 2nd pass: the steps and the bytes at their places
	std::fill(last.begin(), last.end(), -1);
	std::vector<uint8_t> previousByte(size, 0);
	z1.Rewind();
	frames = 0;
	while (z1.NextFrame([&](uint32_t o, uint8_t v)
		{
			structAt[o] += PutVarintAt(structStream.data() + structAt[o], (uint32_t)(frames - last[o]));
			payload[payloadAt[o]++] = (uint8_t)(v - previousByte[o]);
			previousByte[o] = v;
			last[o] = frames;
		}))
		frames++;

	const std::vector<uint8_t> structPacked = LzCompress(structStream.data(), structStream.size());
	const std::vector<uint8_t> payloadPacked = LzCompress(payload.data(), payload.size());
	FILE* out = fopen(z4Path.c_str(), "wb");
	if (out == nullptr)
		return false;
	const uint32_t header[6] = { size, frames, (uint32_t)structStream.size(), (uint32_t)structPacked.size(),
		(uint32_t)payload.size(), (uint32_t)payloadPacked.size() };
	bool ok = fwrite("MC2SEQZ4", 1, 8, out) == 8 && fwrite(header, 4, 6, out) == 6
		&& fwrite(structPacked.data(), 1, structPacked.size(), out) == structPacked.size()
		&& fwrite(payloadPacked.data(), 1, payloadPacked.size(), out) == payloadPacked.size();
	return fclose(out) == 0 && ok;
}

// the new format frame by frame
class Z4Reader
{
public:
	bool Open(const std::string& path)
	{
		FILE* f = fopen(path.c_str(), "rb");
		if (f == nullptr)
			return false;
		fseek(f, 0, SEEK_END);
		const long fileSize = ftell(f);
		fseek(f, 0, SEEK_SET);
		std::vector<uint8_t> data(fileSize > 0 ? (size_t)fileSize : 0);
		const bool read = !data.empty() && fread(data.data(), 1, data.size(), f) == data.size();
		fclose(f);
		if (!read || data.size() < 32 || memcmp(data.data(), "MC2SEQZ4", 8) != 0)
			return false;
		uint32_t header[6];
		memcpy(header, data.data() + 8, sizeof(header));
		frameSize_ = header[0];
		frames_ = header[1];
		if (32 + (size_t)header[3] + header[5] > data.size())
			return false;
		struct_ = LzDecompress(data.data() + 32, header[3], header[2]);
		payload_ = LzDecompress(data.data() + 32 + header[3], header[5], header[4]);
		if (struct_.size() != header[2] || payload_.size() != header[4])
			return false;

		size_t p = 0, payloadAt = 0;
		const uint32_t offsets = GetVarint(struct_.data(), struct_.size(), p);
		offset_.resize(offsets); count_.resize(offsets); structStart_.resize(offsets); payloadStart_.resize(offsets);
		uint32_t o = 0;
		for (uint32_t k = 0; k < offsets; k++)
		{
			o += GetVarint(struct_.data(), struct_.size(), p);
			offset_[k] = o;
			count_[k] = GetVarint(struct_.data(), struct_.size(), p);
			structStart_[k] = p;
			payloadStart_[k] = payloadAt;
			payloadAt += count_[k];
			for (uint32_t i = 0; i < count_[k]; i++)
				GetVarint(struct_.data(), struct_.size(), p);
			if (o >= frameSize_)
				return false;
		}
		if (payloadAt != payload_.size())
			return false;
		Rewind();
		return true;
	}

	uint32_t FrameSize() const { return frameSize_; }
	uint32_t Frames() const { return frames_; }
	long long Index() const { return index_; }//the frame State() holds, -1 before the first
	const std::vector<uint8_t>& State() const { return state_; }

	void Rewind()
	{
		const size_t offsets = offset_.size();
		left_ = count_;
		structAt_ = structStart_;
		payloadAt_ = payloadStart_;
		value_.assign(offsets, 0);
		next_.assign(offsets, -1);
		for (size_t k = 0; k < offsets; k++)
			if (left_[k])
				next_[k] += GetVarint(struct_.data(), struct_.size(), structAt_[k]);
		state_.assign(frameSize_, 0);
		index_ = -1;
		blockStart_ = blockEnd_ = 0;
	}

	// the next frame into State(); false after the last one
	bool Next()
	{
		if (index_ + 1 >= (long long)frames_)
			return false;
		index_++;
		if (index_ >= blockEnd_)
			FillBlock(index_);
		for (const auto& write : block_[(size_t)(index_ - blockStart_)])
			state_[write.first] = write.second;
		return true;
	}

private:
	static const int kBlock = 256;//frames unpacked at once

	void FillBlock(long long from)
	{
		blockStart_ = from;
		blockEnd_ = std::min<long long>(from + kBlock, frames_);
		block_.resize(kBlock);
		for (auto& frame : block_)
			frame.clear();
		for (size_t k = 0; k < offset_.size(); k++)
			while (left_[k] && next_[k] < blockEnd_)
			{
				value_[k] = (uint8_t)(value_[k] + payload_[payloadAt_[k]++]);
				block_[(size_t)(next_[k] - blockStart_)].push_back({ offset_[k], value_[k] });
				if (--left_[k])
					next_[k] += GetVarint(struct_.data(), struct_.size(), structAt_[k]);
			}
	}

	uint32_t frameSize_ = 0, frames_ = 0;
	std::vector<uint8_t> struct_, payload_, state_;
	std::vector<uint32_t> offset_, count_, left_;
	std::vector<size_t> structStart_, payloadStart_, structAt_, payloadAt_;
	std::vector<int64_t> next_;
	std::vector<uint8_t> value_;
	std::vector<std::vector<std::pair<uint32_t, uint8_t>>> block_;
	long long index_ = -1, blockStart_ = 0, blockEnd_ = 0;
};

// the old and the new file give the same state after every frame
inline bool SameFrames(const std::string& z1Path, const std::string& z4Path)
{
	Z1Reader z1;
	Z4Reader z4;
	if (!z1.Open(z1Path) || !z4.Open(z4Path) || z1.FrameSize() != z4.FrameSize())
		return false;
	std::vector<uint8_t> state(z1.FrameSize(), 0);
	uint32_t frames = 0;
	while (z1.NextFrame([&](uint32_t o, uint8_t v) { state[o] = v; }))
	{
		if (!z4.Next() || z4.State() != state)
			return false;
		frames++;
	}
	return frames == z4.Frames() && !z4.Next();
}

// an old file becomes a new one in place, once it gives the same frames
inline bool UpgradeToZ4(const std::string& path)
{
	const std::string temp = path + ".z4tmp";
	if (!ConvertZ1ToZ4(path, temp) || !SameFrames(path, temp))
	{
		remove(temp.c_str());
		return false;
	}
	remove(path.c_str());
	return rename(temp.c_str(), path.c_str()) == 0;
}

} // namespace seqz
