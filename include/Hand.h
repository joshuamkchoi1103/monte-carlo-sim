#pragma once
#include "Types.h"

class Hand {
private:
    uint8_t total;
    uint8_t usableAces;

public:
    // New constructor to initialize specific starting states
    Hand(uint8_t startTotal = 0, uint8_t startAces = 0) 
        : total(startTotal), usableAces(startAces) {}

    inline void addCard(Card c) {
        uint8_t val = static_cast<uint8_t>(c);
        total += val;
        
        if (c == Card::Ace) {
            usableAces++;
        }
        
        if (total > 21 && usableAces > 0) {
            total -= 10;
            usableAces--;
        }
    }

    inline uint8_t getTotal() const { return total; }
    inline bool isBust() const { return total > 21; }
    inline bool isSoft() const { return usableAces > 0; }
    
    inline void reset(uint8_t startTotal = 0, uint8_t startAces = 0) {
        total = startTotal;
        usableAces = startAces;
    }
};