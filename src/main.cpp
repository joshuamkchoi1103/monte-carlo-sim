#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <string>
#include "Bitboard.h"
#include "Simulator.h"

// Ranks map directly to our 0-12 indices (2 through Ace)
const char* rankNames = "23456789TJQKA";

int main() {
    using namespace Poker;
    
    // 1 million iterations per hand * 169 hands = 169 Million simulated hands.
    // Thanks to your bitwise engine, this will take less than 10 seconds.
    const size_t ITERATIONS = 1'000'000; 

    std::ofstream outFile("preflop_equity.csv");
    outFile << "Hand,Equity\n";

    std::cout << "Generating 169-Hand Preflop Equity Matrix...\n";
    auto start = std::chrono::high_resolution_clock::now();

    // Loop backwards from Ace (12) down to 2 (0)
    for (int r1 = 12; r1 >= 0; --r1) {
        for (int r2 = 12; r2 >= 0; --r2) {
            Bitboard heroHole;
            std::string handName = "";
            handName += rankNames[r1];
            handName += rankNames[r2];

            if (r1 == r2) {
                // Pair: Must be different suits (e.g., Spades and Hearts)
                heroHole.addCard(r1);      
                heroHole.addCard(r1 + 13); 
            } else if (r1 > r2) {
                // Suited: Both cards share the same suit mask (e.g., Spades)
                heroHole.addCard(r1);
                heroHole.addCard(r2);
                handName += "s";
            } else {
                // Offsuit: Cards have different suits
                heroHole.addCard(r1);
                heroHole.addCard(r2 + 13);
                handName += "o";
            }

            // Run the Monte Carlo simulation against a random opponent
            double equity = Simulator::calculateStartingHandEquity(heroHole, ITERATIONS);
            
            outFile << handName << "," << equity << "\n";
            std::cout << "Evaluated " << handName << "\r" << std::flush; // Progress indicator
        }
    }

    outFile.close();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << "\nCompleted 169 Million simulations in " << elapsed.count() << " seconds.\n";
    std::cout << "Data saved to 'preflop_equity.csv'." << std::endl;

    return 0;
}