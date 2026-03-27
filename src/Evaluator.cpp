#include "Evaluator.h"

namespace Poker {

int Evaluator::hasStraight(uint64_t mask) {
    uint16_t ranks = (mask) | (mask >> 13) | (mask >> 26) | (mask >> 39);
    
    // Check for standard straights, starting from the highest possible (Ace-high)
    for (int i = 12; i >= 4; --i) {
        uint16_t straightMask = (0x1F << (i - 4));
        if ((ranks & straightMask) == straightMask) return i; // Return highest card index
    }
    
    // Check the "Wheel" (Ace, 2, 3, 4, 5)
    if ((ranks & 0x100F) == 0x100F) return 3; // 5 is the highest card (index 3)
    
    return -1;
}

uint32_t Evaluator::checkMultiples(uint64_t mask) {
    uint8_t rankCounts[13] = {0};
    for (int i = 0; i < 52; ++i) {
        if ((mask & (1ULL << i))) rankCounts[i % 13]++;
    }

    int quadRank = -1, trip1 = -1, trip2 = -1;
    int pair1 = -1, pair2 = -1;

    // Scan backwards (Ace down to 2) to guarantee we grab the highest multiples first
    for (int i = 12; i >= 0; --i) {
        if (rankCounts[i] == 4) quadRank = i;
        else if (rankCounts[i] == 3) {
            if (trip1 == -1) trip1 = i; else trip2 = i;
        }
        else if (rankCounts[i] == 2) {
            if (pair1 == -1) pair1 = i; else if (pair2 == -1) pair2 = i;
        }
    }

    // 1. Four of a Kind
    if (quadRank != -1) {
        int kicker = -1;
        for (int i = 12; i >= 0; --i) { if (rankCounts[i] > 0 && i != quadRank) { kicker = i; break; } }
        return buildScore(7, quadRank, kicker);
    }

    // 2. Full House
    if (trip1 != -1) {
        if (trip2 != -1) return buildScore(6, trip1, trip2); // Two trips (e.g., KKK, JJJ -> KKK-JJ)
        if (pair1 != -1) return buildScore(6, trip1, pair1);
        
        // 3. Three of a Kind
        int k[2] = {0}, idx = 0;
        for (int i = 12; i >= 0 && idx < 2; --i) { if (rankCounts[i] > 0 && i != trip1) k[idx++] = i; }
        return buildScore(3, trip1, k[0], k[1]);
    }

    // 4. Two Pair
    if (pair1 != -1 && pair2 != -1) {
        int kicker = -1;
        for (int i = 12; i >= 0; --i) { if (rankCounts[i] > 0 && i != pair1 && i != pair2) { kicker = i; break; } }
        return buildScore(2, pair1, pair2, kicker);
    }

    // 5. Pair
    if (pair1 != -1) {
        int k[3] = {0}, idx = 0;
        for (int i = 12; i >= 0 && idx < 3; --i) { if (rankCounts[i] > 0 && i != pair1) k[idx++] = i; }
        return buildScore(1, pair1, k[0], k[1], k[2]);
    }

    // 6. High Card
    int k[5] = {0}, idx = 0;
    for (int i = 12; i >= 0 && idx < 5; --i) { if (rankCounts[i] > 0) k[idx++] = i; }
    return buildScore(0, k[0], k[1], k[2], k[3], k[4]);
}

uint32_t Evaluator::evaluate(const Bitboard& board) {
    uint64_t mask = board.getMask();
    
    // Check for Flush by isolating suits
    uint64_t flushSuitMask = 0;
    if (__builtin_popcountll(mask & SUIT_SPADES) >= 5) flushSuitMask = mask & SUIT_SPADES;
    else if (__builtin_popcountll(mask & SUIT_HEARTS) >= 5) flushSuitMask = mask & SUIT_HEARTS;
    else if (__builtin_popcountll(mask & SUIT_DIAMONDS) >= 5) flushSuitMask = mask & SUIT_DIAMONDS;
    else if (__builtin_popcountll(mask & SUIT_CLUBS) >= 5) flushSuitMask = mask & SUIT_CLUBS;

    if (flushSuitMask) {
        int straightTop = hasStraight(flushSuitMask);
        if (straightTop != -1) return buildScore(8, straightTop); // Straight Flush
        
        // Standard Flush: Extract top 5 cards
        uint16_t ranks = flushSuitMask | (flushSuitMask >> 13) | (flushSuitMask >> 26) | (flushSuitMask >> 39);
        int k[5] = {0}, idx = 0;
        for (int i = 12; i >= 0 && idx < 5; --i) {
            if (ranks & (1 << i)) k[idx++] = i;
        }
        return buildScore(5, k[0], k[1], k[2], k[3], k[4]);
    }

    int straightTop = hasStraight(mask);
    if (straightTop != -1) return buildScore(4, straightTop); // Straight

    return checkMultiples(mask); // Evaluates Quads, Boats, Trips, Pairs, High Card
}

} // namespace Poker