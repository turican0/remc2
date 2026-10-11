// lzcompress.h — self-contained LZ77 + adaptive range-coder byte compressor.
// No external dependencies. Literals use an order-1 model on the previous byte.
// The match finder keeps a window of kWindow bytes, so its memory does not grow with the input;
// the stream itself does not depend on the window, the decoder takes any distance.
#pragma once
#include "range_coder.h"
#include <cstdint>
#include <cstring>
#include <vector>

struct LzModels {
    uint16_t isMatch[2];               // context: 0 = previous token was literal, 1 = previous was match
    uint16_t literal[256][256];        // [prevByte][tree node] order-1 literal model
    uint16_t lenIsZero;                // (length - kMinMatch) == 0 ?
    uint16_t lenNbits[32];
    uint16_t distNbits[32];

    LzModels() {
        isMatch[0] = isMatch[1] = kProbInitVal;
        for (auto& ctx : literal) for (auto& p : ctx) p = kProbInitVal;
        lenIsZero = kProbInitVal;
        for (auto& p : lenNbits) p = kProbInitVal;
        for (auto& p : distNbits) p = kProbInitVal;
    }
};

static const int kMinMatch = 4;
static const int kHashBits = 20;
static const int kHashSize = 1 << kHashBits;
static const int kMaxChainDepth = 64; // speed/ratio tradeoff for the match finder
static const size_t kWindow = (size_t)1 << 22; // positions the match finder remembers

inline uint32_t Hash4(const uint8_t* p) {
    uint32_t v = (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
    return (v * 2654435761u) >> (32 - kHashBits);
}

// Compress an arbitrary byte buffer. The original size is stored by the caller.
inline std::vector<uint8_t> LzCompress(const uint8_t* data, size_t n) {
    std::vector<uint8_t> out;
    RangeEncoder rc(out);
    LzModels* m = new LzModels();

    std::vector<int64_t> head(kHashSize, -1);
    const size_t window = n < kWindow ? (n ? n : 1) : kWindow;
    std::vector<int64_t> prevChain(window, -1);
    auto chainSlot = [&](size_t pos) -> int64_t& { return prevChain[pos % window]; };

    uint8_t prevByte = 0;
    size_t i = 0;
    int prevWasMatch = 0;

    auto matchLenAt = [&](size_t a, size_t b) -> int {
        size_t maxLen = n - b;
        size_t len = 0;
        while (len < maxLen && data[a + len] == data[b + len]) len++;
        return (int)len;
    };

    auto insert = [&](size_t pos) {
        uint32_t h = Hash4(&data[pos]);
        chainSlot(pos) = head[h];
        head[h] = (int64_t)pos;
    };

    auto findBestMatch = [&](size_t pos, int& bestLen, size_t& bestSrc) {
        bestLen = 0; bestSrc = 0;
        if (pos + 4 > n) return;
        uint32_t h = Hash4(&data[pos]);
        int64_t cand = head[h];
        int depth = kMaxChainDepth;
        while (cand >= 0 && pos - (size_t)cand < window && depth-- > 0) {
            if (data[cand] == data[pos]) {
                int len = matchLenAt((size_t)cand, pos);
                if (len > bestLen) { bestLen = len; bestSrc = (size_t)cand; if (len > 4096) break; }
            }
            cand = chainSlot((size_t)cand);
        }
    };

    while (i < n) {
        int bestLen; size_t bestSrc;
        findBestMatch(i, bestLen, bestSrc);

        // simple 1-step lazy evaluation: is pos+1 meaningfully better?
        if (bestLen >= kMinMatch && i + 1 < n) {
            if (i + 4 <= n) insert(i);
            int bestLen2; size_t bestSrc2;
            findBestMatch(i + 1, bestLen2, bestSrc2);
            if (bestLen2 > bestLen + 1) {
                // emit literal at i, take the better match at i+1 next iteration
                rc.EncodeBit(m->isMatch[prevWasMatch], 0);
                BitTreeEncode(rc, m->literal[prevByte], 8, data[i]);
                prevByte = data[i]; prevWasMatch = 0;
                i++;
                continue;
            }
        } else if (i + 4 <= n) {
            insert(i);
        }

        if (bestLen >= kMinMatch) {
            rc.EncodeBit(m->isMatch[prevWasMatch], 1);
            uint32_t dist = (uint32_t)(i - bestSrc); // >=1
            EncodeValue(rc, m->distNbits, dist);
            EncodeValueZ(rc, m->lenIsZero, m->lenNbits, (uint32_t)(bestLen - kMinMatch));
            // insert hash entries for the skipped positions (bounded, for speed)
            size_t end = i + bestLen;
            size_t insertEnd = (end < i + 64) ? end : i + 64; // cap insertion cost on huge matches
            for (size_t j = i + 1; j < insertEnd && j + 4 <= n; j++)
                insert(j);
            prevByte = data[end - 1];
            prevWasMatch = 1;
            i = end;
        } else {
            rc.EncodeBit(m->isMatch[prevWasMatch], 0);
            BitTreeEncode(rc, m->literal[prevByte], 8, data[i]);
            prevByte = data[i]; prevWasMatch = 0;
            i++;
        }
    }
    rc.Flush();
    delete m;
    return out;
}

inline std::vector<uint8_t> LzDecompress(const uint8_t* comp, size_t compSize, size_t origSize) {
    std::vector<uint8_t> out;
    out.reserve(origSize);
    RangeDecoder rc(comp, compSize);
    LzModels* m = new LzModels();
    uint8_t prevByte = 0;
    int prevWasMatch = 0;

    while (out.size() < origSize) {
        uint32_t isMatch = rc.DecodeBit(m->isMatch[prevWasMatch]);
        if (isMatch == 0) {
            uint32_t b = BitTreeDecode(rc, m->literal[prevByte], 8);
            out.push_back((uint8_t)b);
            prevByte = (uint8_t)b;
            prevWasMatch = 0;
        } else {
            uint32_t dist = DecodeValue(rc, m->distNbits);
            uint32_t lenM = DecodeValueZ(rc, m->lenIsZero, m->lenNbits);
            uint32_t len = lenM + kMinMatch;
            if (dist == 0 || dist > out.size() || out.size() + len > origSize)
                break; // a damaged stream: the caller finds the size short
            size_t src = out.size() - dist;
            for (uint32_t k = 0; k < len; k++) out.push_back(out[src + k]);
            prevByte = out.back();
            prevWasMatch = 1;
        }
    }
    delete m;
    return out;
}
