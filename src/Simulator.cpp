#include "Simulator.h"
#include "Evaluator.h"
#include "RNG.h"
#include <chrono>

namespace Poker {

double Simulator::calculateEquity(
    const Bitboard& heroHole,
    const Bitboard& villainHole,
    const Bitboard& currentBoard,
    size_t iterations) 
{
    // 1. Set up the Master Deck
    // We remove all known cards from the deck so they can't be drawn
    Deck masterDeck;
    masterDeck.removeCards(heroHole);
    masterDeck.removeCards(villainHole);
    masterDeck.removeCards(currentBoard);

    // 2. Determine how many community cards still need to be dealt
    int cardsToDraw = 5 - currentBoard.countCards();

    // 3. Initialize our lightning-fast xoshiro256++ RNG
    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    RNG rng(seed);

    int wins = 0;
    int ties = 0;

    // 4. The Hot Loop
    for (size_t i = 0; i < iterations; ++i) {
        
        // Copy the master state (incredibly fast because it's stack-allocated)
        Deck simDeck = masterDeck; 
        Bitboard simBoard = currentBoard;

        // Deal the remaining community cards
        for (int c = 0; c < cardsToDraw; ++c) {
            simBoard.addCard(simDeck.draw(rng));
        }

        // Combine hole cards with the final board
        Bitboard heroFinal = heroHole;
        heroFinal.combine(simBoard);

        Bitboard villainFinal = villainHole;
        villainFinal.combine(simBoard);

        // Evaluate both hands
        uint32_t heroRank = Evaluator::evaluate(heroFinal);
        uint32_t villainRank = Evaluator::evaluate(villainFinal);

        // Tally results
        if (heroRank > villainRank) {
            wins++;
        } else if (heroRank == villainRank) {
            ties++;
        }
    }

    // Equity = (Wins + (Ties / 2)) / Total Iterations
    return static_cast<double>(wins + (ties / 2.0)) / iterations;
}

double Simulator::calculateStartingHandEquity(const Bitboard& heroHole, size_t iterations) {
    Deck masterDeck;
    masterDeck.removeCards(heroHole);

    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    RNG rng(seed);

    int wins = 0, ties = 0;

    for (size_t i = 0; i < iterations; ++i) {
        Deck simDeck = masterDeck;
        Bitboard simBoard;
        Bitboard villainHole;

        // Draw 2 random cards for the Villain
        villainHole.addCard(simDeck.draw(rng));
        villainHole.addCard(simDeck.draw(rng));

        // Draw 5 community cards for the Board
        for (int c = 0; c < 5; ++c) {
            simBoard.addCard(simDeck.draw(rng));
        }

        Bitboard heroFinal = heroHole;
        heroFinal.combine(simBoard);

        Bitboard villainFinal = villainHole;
        villainFinal.combine(simBoard);

        // Evaluate both using our new 32-bit hex score
        uint32_t heroRank = Evaluator::evaluate(heroFinal);
        uint32_t villainRank = Evaluator::evaluate(villainFinal);

        if (heroRank > villainRank) wins++;
        else if (heroRank == villainRank) ties++;
    }

    return static_cast<double>(wins + (ties / 2.0)) / iterations;
}

} // namespace Poker