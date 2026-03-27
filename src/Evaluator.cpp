#include "Evaluator.h"

namespace Poker {

bool Evaluator::hasFlush(uint64_t mask) {
    if (__builtin_popcountll(mask & SUIT_SPADES) >= 5) return true;
    if (__builtin_popcountll(mask & SUIT_HEARTS) >= 5) return true;
    if (__builtin_popcountll(mask & SUIT_DIAMONDS) >= 5) return true;
    if (__builtin_popcountll(mask & SUIT_CLUBS) >= 5) return true;
    return false;
}

bool Evaluator::hasStraight(uint64_t mask) {
    // Collapse suits into a single 13-bit sequence
    uint16_t ranks = (mask) | (mask >> 13) | (mask >> 26) | (mask >> 39);
    
    // Bitwise AND shift to find 5 consecutive bits
    uint16_t straight = ranks & (ranks >> 1) & (ranks >> 2) & (ranks >> 3) & (ranks >> 4);
    if (straight > 0) return true;

    // Check the "Wheel" edge case (Ace, 2, 3, 4, 5)
    if ((ranks & 0x100F) == 0x100F) return true;

    return false;
}

HandRank Evaluator::checkMultiples(uint64_t mask) {
    uint8_t rankCounts[13] = {0};
    
    // Tally the frequencies of each rank across all suits
    for (int i = 0; i < 52; ++i) {
        if ((mask & (1ULL << i))) {
            rankCounts[i % 13]++;
        }
    }

    bool pairs = false;
    bool twoPair = false;
    bool trips = false;
    bool quads = false;

    // Evaluate the frequencies
    for (int i = 0; i < 13; ++i) {
        if (rankCounts[i] == 4) quads = true;
        else if (rankCounts[i] == 3) {
            if (trips || pairs) twoPair = true; // Two trips or a trip+pair makes a full house in 7 cards
            trips = true;
        }
        else if (rankCounts[i] == 2) {
            if (pairs) twoPair = true;
            pairs = true;
        }
    }

    if (quads) return HandRank::FourOfAKind;
    if (trips && twoPair) return HandRank::FullHouse; 
    if (trips) return HandRank::ThreeOfAKind;
    if (twoPair) return HandRank::TwoPair;
    if (pairs) return HandRank::Pair;
    return HandRank::HighCard;
}

HandRank Evaluator::evaluate(const Bitboard& board) {
    uint64_t mask = board.getMask();
    
    bool flush = hasFlush(mask);
    bool straight = hasStraight(mask);

    if (flush && straight) return HandRank::StraightFlush; 
    if (flush) return HandRank::Flush;
    if (straight) return HandRank::Straight;

    return checkMultiples(mask);
}

} // namespace Poker