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

    // Resets the deck back to 52 cards
    void reset();

    // Removes a specific card from the deck (used when we know the hole/board cards)
    void removeCard(uint8_t cardIndex);

    // Removes all cards present in a given bitboard
    void removeCards(const Bitboard& board);

    // Draws a random card using our custom xoshiro engine in O(1) time
    inline uint8_t draw(RNG& rng) {
        // Get a random index from the available pool
        uint32_t idx = rng.nextBound(remainingCards);
        
        // Grab the card
        uint8_t drawnCard = cards[idx];
        
        // Swap the drawn card with the last available card and shrink the pool
        remainingCards--;
        cards[idx] = cards[remainingCards];
        
        return drawnCard;
    }
};

} // namespace Poker