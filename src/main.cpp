#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <string>
#include "Bitboard.h"
#include "Simulator.h"

const char* rankNames = "23456789TJQKA";

int main() {
    using namespace Poker;
    
    const size_t ITERATIONS = 1'000'000; 

    std::ofstream outFile("preflop_equity.csv");
    outFile << "Hand,Equity\n";

    std::cout << "Generating 169-Hand Preflop Equity Matrix...\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int r1 = 12; r1 >= 0; --r1) {
        for (int r2 = 12; r2 >= 0; --r2) {
            Bitboard heroHole;
            std::string handName = "";
            handName += rankNames[r1];
            handName += rankNames[r2];

            if (r1 == r2) {
                heroHole.addCard(r1);      
                heroHole.addCard(r1 + 13); 
            } else if (r1 > r2) {
                heroHole.addCard(r1);
                heroHole.addCard(r2);
                handName += "s";
            } else {
                heroHole.addCard(r1);
                heroHole.addCard(r2 + 13);
                handName += "o";
            }

            double equity = Simulator::calculateStartingHandEquity(heroHole, ITERATIONS);
            
            outFile << handName << "," << equity << "\n";
            std::cout << "Evaluated " << handName << "\r" << std::flush; 
        }
    }

    outFile.close();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::cout << "\nCompleted 169 Million simulations in " << elapsed.count() << " seconds.\n";
    std::cout << "Data saved to 'preflop_equity.csv'." << std::endl;

    return 0;
}