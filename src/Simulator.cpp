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
    Deck masterDeck;
    masterDeck.removeCards(heroHole);
    masterDeck.removeCards(villainHole);
    masterDeck.removeCards(currentBoard);

    int cardsToDraw = 5 - currentBoard.countCards();
    uint64_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    RNG rng(seed);

    int wins = 0, ties = 0;

    for (size_t i = 0; i < iterations; ++i) {
        Deck simDeck = masterDeck; 
        Bitboard simBoard = currentBoard;

        for (int c = 0; c < cardsToDraw; ++c) {
            simBoard.addCard(simDeck.draw(rng));
        }

        Bitboard heroFinal = heroHole;
        heroFinal.combine(simBoard);

        Bitboard villainFinal = villainHole;
        villainFinal.combine(simBoard);

        uint32_t heroRank = Evaluator::evaluate(heroFinal);
        uint32_t villainRank = Evaluator::evaluate(villainFinal);

        if (heroRank > villainRank) wins++;
        else if (heroRank == villainRank) ties++;
    }

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

        villainHole.addCard(simDeck.draw(rng));
        villainHole.addCard(simDeck.draw(rng));

        for (int c = 0; c < 5; ++c) {
            simBoard.addCard(simDeck.draw(rng));
        }

        Bitboard heroFinal = heroHole;
        heroFinal.combine(simBoard);

        Bitboard villainFinal = villainHole;
        villainFinal.combine(simBoard);

        uint32_t heroRank = Evaluator::evaluate(heroFinal);
        uint32_t villainRank = Evaluator::evaluate(villainFinal);

        if (heroRank > villainRank) wins++;
        else if (heroRank == villainRank) ties++;
    }

    return static_cast<double>(wins + (ties / 2.0)) / iterations;
}

} // namespace Poker