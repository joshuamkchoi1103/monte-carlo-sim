#include "Deck.h"

namespace Poker {

Deck::Deck() {
    reset();
}

void Deck::reset() {
    for (uint8_t i = 0; i < 52; ++i) {
        cards[i] = i;
    }
    remainingCards = 52;
}

void Deck::removeCard(uint8_t cardIndex) {
    for (uint8_t i = 0; i < remainingCards; ++i) {
        if (cards[i] == cardIndex) {
            remainingCards--;
            cards[i] = cards[remainingCards];
            return;
        }
    }
}

void Deck::removeCards(const Bitboard& board) {
    for (uint8_t i = 0; i < 52; ++i) {
        if (board.containsCard(i)) {
            removeCard(i);
        }
    }
}

} // namespace Poker