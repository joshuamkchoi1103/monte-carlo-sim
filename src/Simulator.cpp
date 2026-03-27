#include "Simulator.h"

uint8_t Simulator::playDealerHand(Card upCard) {
    Hand dealerHand;
    dealerHand.addCard(upCard);          
    dealerHand.addCard(shoe.dealCard()); 

    while (dealerHand.getTotal() < 17 || (dealerHand.getTotal() == 17 && dealerHand.isSoft())) {
        dealerHand.addCard(shoe.dealCard());
    }

    return dealerHand.getTotal();
}

double Simulator::calculateEV(uint8_t playerTotal, bool isSoft, Card dealerUpCard, Action action, size_t iterations) {
    int64_t netScore = 0; 
    
    // Doubling down means we risk 2 units instead of 1
    int8_t multiplier = (action == Action::Double) ? 2 : 1;

    for (size_t i = 0; i < iterations; ++i) {
        // Initialize the hand with the exact state (1 usable Ace if soft)
        Hand playerHand(playerTotal, isSoft ? 1 : 0);
        
        if (action == Action::Hit || action == Action::Double) {
            playerHand.addCard(shoe.dealCard());
        }

        if (playerHand.isBust()) {
            netScore += static_cast<int8_t>(Result::Loss) * multiplier;
            continue; 
        }

        uint8_t dealerTotal = playDealerHand(dealerUpCard);
        uint8_t currentTotal = playerHand.getTotal();

        if (dealerTotal > 21 || currentTotal > dealerTotal) {
            netScore += static_cast<int8_t>(Result::Win) * multiplier;
        } else if (currentTotal < dealerTotal) {
            netScore += static_cast<int8_t>(Result::Loss) * multiplier;
        } else {
            netScore += static_cast<int8_t>(Result::Push) * multiplier;
        }
    }

    return static_cast<double>(netScore) / iterations;
}