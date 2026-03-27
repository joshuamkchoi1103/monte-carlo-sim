#pragma once
#include <cstdint>
#include "Constants.h"

namespace Poker {

class Bitboard {
private:
    uint64_t mask;

public:
    constexpr Bitboard() : mask(0) {}
    constexpr Bitboard(uint64_t m) : mask(m) {}

    // Bitwise OR to set a card
    inline void addCard(uint8_t cardIndex) {
        mask |= (1ULL << cardIndex);
    }

    // Bitwise AND with inverted bit to remove a card
    inline void removeCard(uint8_t cardIndex) {
        mask &= ~(1ULL << cardIndex);
    }

    inline bool containsCard(uint8_t cardIndex) const {
        return (mask & (1ULL << cardIndex)) != 0;
    }

    inline void combine(const Bitboard& other) {
        mask |= other.mask;
    }

    // Hardware-level population count (single CPU cycle)
    inline int countCards() const {
        return __builtin_popcountll(mask);
    }

    inline uint64_t getMask() const { return mask; }
};

} // namespace Poker