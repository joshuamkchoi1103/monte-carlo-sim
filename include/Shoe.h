#pragma once
#include "Types.h"
#include <array>
#include <random>

class Shoe {
private:
    static constexpr size_t NUM_DECKS = 6;
    static constexpr size_t CARDS_PER_SHOE = NUM_DECKS * 52;
    
    std::array<Card, CARDS_PER_SHOE> cards;
    size_t currentIndex;
    std::mt19937 rng; 

    void initializeDecks();

public:
    Shoe();
    
    void shuffle();
    
    inline Card dealCard() {
        // Simulates the casino cut card at 75% penetration
        if (currentIndex >= CARDS_PER_SHOE * 0.75) { 
            shuffle();
        }
        return cards[currentIndex++];
    }
};