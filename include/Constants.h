#pragma once
#include <cstdint>

namespace Poker {
    // 13 bits set to 1 (0001 1111 1111 1111). Represents the ranks Two through Ace.
    constexpr uint64_t RANK_MASK = 0x1FFF; 

    // Pre-computed masks for entire suits
    constexpr uint64_t SUIT_SPADES   = RANK_MASK; 
    constexpr uint64_t SUIT_HEARTS   = RANK_MASK << 13;
    constexpr uint64_t SUIT_DIAMONDS = RANK_MASK << 26;
    constexpr uint64_t SUIT_CLUBS    = RANK_MASK << 39;
}