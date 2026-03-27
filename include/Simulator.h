#pragma once
#include "Bitboard.h"
#include "Deck.h"

namespace Poker {

class Simulator {
public:
    static double calculateEquity(
        const Bitboard& heroHole,
        const Bitboard& villainHole,
        const Bitboard& currentBoard,
        size_t iterations
    );

    // Simulates a hero hand against a random villain hand
    static double calculateStartingHandEquity(const Bitboard& heroHole, size_t iterations);
};

} // namespace Poker