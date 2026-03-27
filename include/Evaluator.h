#pragma once
#include "Bitboard.h"

namespace Poker {

class Evaluator {
public:
    // Now returns a 32-bit hexadecimal score instead of an enum
    static uint32_t evaluate(const Bitboard& board);

private:
    // Helper to shift our hand ranks and kickers into the correct bit positions
    static inline uint32_t buildScore(uint32_t rankCategory, uint32_t a = 0, uint32_t b = 0, uint32_t c = 0, uint32_t d = 0, uint32_t e = 0) {
        return (rankCategory << 20) | (a << 16) | (b << 12) | (c << 8) | (d << 4) | e;
    }

    static int hasStraight(uint64_t mask);
    static uint32_t checkMultiples(uint64_t mask);
};

} // namespace Poker