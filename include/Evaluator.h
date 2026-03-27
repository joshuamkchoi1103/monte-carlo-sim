#pragma once
#include "Bitboard.h"

namespace Poker {

enum class HandRank : uint8_t {
    HighCard = 0, Pair, TwoPair, ThreeOfAKind, 
    Straight, Flush, FullHouse, FourOfAKind, StraightFlush
};

class Evaluator {
public:
    static HandRank evaluate(const Bitboard& board);

private:
    static bool hasFlush(uint64_t mask);
    static bool hasStraight(uint64_t mask);
    static HandRank checkMultiples(uint64_t mask);
};

} // namespace Poker