#pragma once
#include "Shoe.h"
#include "Hand.h"

class Simulator {
private:
    Shoe shoe;
    uint8_t playDealerHand(Card upCard);

public:
    Simulator() = default;
    
    // Added 'isSoft' parameter
    double calculateEV(uint8_t playerTotal, bool isSoft, Card dealerUpCard, Action action, size_t iterations);
};