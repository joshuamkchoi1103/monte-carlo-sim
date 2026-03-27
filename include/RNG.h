#pragma once
#include <cstdint>

namespace Poker {

class RNG {
private:
    uint64_t s[4];

    static inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

public:
    RNG(uint64_t seed) {
        // SplitMix64 algorithm to initialize the 4 states properly
        uint64_t z = (seed + 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        s[0] = z ^ (z >> 31);
        
        s[1] = s[0] ^ 0x1234567890ABCDEFULL;
        s[2] = s[1] ^ 0xDEADBEEFCAFEBABULL;
        s[3] = s[2] ^ 0x876543210FEDCBAULL;
    }

    inline uint64_t next() {
        const uint64_t result = rotl(s[0] + s[3], 23) + s[0];
        const uint64_t t = s[1] << 17;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }

    inline uint32_t nextBound(uint32_t bound) {
        return (uint32_t)((next() & 0xFFFFFFFF) * bound >> 32);
    }
};

} // namespace Poker