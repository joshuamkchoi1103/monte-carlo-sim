#include "Shoe.h"
#include <algorithm> 
#include <chrono>    

Shoe::Shoe() : currentIndex(0) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = std::mt19937(seed);
    
    initializeDecks();
    shuffle();
}

void Shoe::initializeDecks() {
    size_t idx = 0;
    for (int deck = 0; deck < NUM_DECKS; ++deck) {
        for (int suit = 0; suit < 4; ++suit) {
            for (uint8_t val = 2; val <= 10; ++val) {
                cards[idx++] = static_cast<Card>(val);
            }
            cards[idx++] = Card::Jack;
            cards[idx++] = Card::Queen;
            cards[idx++] = Card::King;
            cards[idx++] = Card::Ace;
        }
    }
}

void Shoe::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
    currentIndex = 0; 
}