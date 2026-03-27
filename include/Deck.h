#pragma once
#include <cstdint>
#include <array>
#include "RNG.h"
#include "Bitboard.h"

namespace Poker {

class Deck {
private:
    std::array<uint8_t, 52> cards;
    uint8_t remainingCards;

public:
    Deck();
    void reset();
    void removeCard(uint8_t cardIndex);
    void removeCards(const Bitboard& board);

    // O(1) swap-and-pop draw
    inline uint8_t draw(RNG& rng) {
        uint32_t idx = rng.nextBound(remainingCards);
        uint8_t drawnCard = cards[idx];
        remainingCards--;
        cards[idx] = cards[remainingCards];
        return drawnCard;
    }
};

} // namespace Poker