// range_coder.h — self-contained adaptive binary range coder (no external deps)
// Based on the classic LZMA-SDK carryless range coder design (public domain algorithm).
#pragma once
#include <cstdint>
#include <vector>

static const uint32_t kTopValue = 1u << 24;
static const int kNumBitModelTotalBits = 11;
static const uint32_t kBitModelTotal = 1u << kNumBitModelTotalBits; // 2048
static const int kNumMoveBits = 5;
static const uint16_t kProbInitVal = kBitModelTotal / 2;

// ---------------- Encoder ----------------
struct RangeEncoder {
    std::vector<uint8_t>& out;
    uint64_t low = 0;
    uint32_t range = 0xFFFFFFFFu;
    uint8_t cache = 0xFF;
    uint64_t cacheSize = 1;
    bool started = false;

    explicit RangeEncoder(std::vector<uint8_t>& o) : out(o) {}

    void ShiftLow() {
        if ((uint32_t)(low >> 32) != 0 || low < 0xFF000000ULL) {
            uint8_t temp = cache;
            do {
                if (started) out.push_back((uint8_t)(temp + (uint8_t)(low >> 32)));
                started = true;
                temp = 0xFF;
            } while (--cacheSize != 0);
            cache = (uint8_t)(low >> 24);
        }
        cacheSize++;
        low = (uint32_t)low << 8;
    }

    inline void EncodeBit(uint16_t& prob, uint32_t bit) {
        uint32_t bound = (range >> kNumBitModelTotalBits) * prob;
        if (bit == 0) {
            range = bound;
            prob = (uint16_t)(prob + ((kBitModelTotal - prob) >> kNumMoveBits));
        } else {
            low += bound;
            range -= bound;
            prob = (uint16_t)(prob - (prob >> kNumMoveBits));
        }
        while (range < kTopValue) { range <<= 8; ShiftLow(); }
    }

    inline void EncodeDirectBits(uint32_t v, int numBits) {
        for (int i = numBits - 1; i >= 0; i--) {
            range >>= 1;
            if ((v >> i) & 1) low += range;
            if (range < kTopValue) { range <<= 8; ShiftLow(); }
        }
    }

    void Flush() { for (int i = 0; i < 5; i++) ShiftLow(); }
};

// ---------------- Decoder ----------------
struct RangeDecoder {
    const uint8_t* buf;
    size_t size, pos = 0;
    uint32_t range = 0xFFFFFFFFu;
    uint32_t code = 0;

    RangeDecoder(const uint8_t* b, size_t s) : buf(b), size(s) {
        for (int i = 0; i < 4; i++) code = (code << 8) | ReadByte();
    }
    inline uint8_t ReadByte() { return pos < size ? buf[pos++] : 0; }

    inline uint32_t DecodeBit(uint16_t& prob) {
        uint32_t bound = (range >> kNumBitModelTotalBits) * prob;
        uint32_t bit;
        if (code < bound) {
            range = bound;
            prob = (uint16_t)(prob + ((kBitModelTotal - prob) >> kNumMoveBits));
            bit = 0;
        } else {
            code -= bound;
            range -= bound;
            prob = (uint16_t)(prob - (prob >> kNumMoveBits));
            bit = 1;
        }
        while (range < kTopValue) { range <<= 8; code = (code << 8) | ReadByte(); }
        return bit;
    }

    inline uint32_t DecodeDirectBits(int numBits) {
        uint32_t res = 0;
        for (int i = 0; i < numBits; i++) {
            range >>= 1;
            code -= range;
            uint32_t t = (uint32_t)0 - (code >> 31);
            code += range & t;
            res = (res << 1) + (t + 1);
            if (range < kTopValue) { range <<= 8; code = (code << 8) | ReadByte(); }
        }
        return res;
    }
};

// ---------------- Bit-tree helpers (for byte literals with context) ----------------
inline void BitTreeEncode(RangeEncoder& rc, uint16_t* probs, int numBits, uint32_t symbol) {
    uint32_t m = 1;
    for (int i = numBits - 1; i >= 0; i--) {
        uint32_t bit = (symbol >> i) & 1;
        rc.EncodeBit(probs[m], bit);
        m = (m << 1) | bit;
    }
}
inline uint32_t BitTreeDecode(RangeDecoder& rc, uint16_t* probs, int numBits) {
    uint32_t m = 1;
    for (int i = 0; i < numBits; i++) m = (m << 1) | rc.DecodeBit(probs[m]);
    return m - (1u << numBits);
}

// ---------------- Variable-magnitude value coder ----------------
// Encodes v>=1 as: (nbits-1) via a 5-bit tree [context array of size 32], then
// (nbits-1) direct raw bits (top bit implied 1). Adaptive nbits model learns the
// typical magnitude distribution for whatever stream it's used on (lengths, distances...).
inline int BitLength(uint32_t v) { int n = 0; while (v) { n++; v >>= 1; } return n; }

inline void EncodeValue(RangeEncoder& rc, uint16_t* nbitsProbs, uint32_t v) {
    int nbits = BitLength(v); // v>=1 so nbits>=1
    BitTreeEncode(rc, nbitsProbs, 5, (uint32_t)(nbits - 1));
    if (nbits > 1) rc.EncodeDirectBits(v & ((1u << (nbits - 1)) - 1), nbits - 1);
}
inline uint32_t DecodeValue(RangeDecoder& rc, uint16_t* nbitsProbs) {
    int nbits = (int)BitTreeDecode(rc, nbitsProbs, 5) + 1;
    if (nbits == 1) return 1;
    uint32_t low = rc.DecodeDirectBits(nbits - 1);
    return (1u << (nbits - 1)) | low;
}

// zero-or-value variant (adds one adaptive "is-zero" flag ahead of EncodeValue)
inline void EncodeValueZ(RangeEncoder& rc, uint16_t& isZeroProb, uint16_t* nbitsProbs, uint32_t v) {
    if (v == 0) { rc.EncodeBit(isZeroProb, 0); return; }
    rc.EncodeBit(isZeroProb, 1);
    EncodeValue(rc, nbitsProbs, v);
}
inline uint32_t DecodeValueZ(RangeDecoder& rc, uint16_t& isZeroProb, uint16_t* nbitsProbs) {
    if (rc.DecodeBit(isZeroProb) == 0) return 0;
    return DecodeValue(rc, nbitsProbs);
}
