#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>
#include "Simulator.h"

// Helper function to find the optimal move
std::string getOptimalMove(double evStand, double evHit, double evDouble) {
    if (evDouble > evStand && evDouble > evHit) return "D";
    if (evHit > evStand) return "H";
    return "S";
}

int main() {
    Simulator sim;
    const size_t ITERATIONS = 1'000'000; // 1M runouts per cell

    std::cout << "Booting Monte Carlo Matrix Generator..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ofstream outFile("basic_strategy.csv");
    outFile << "HandType,PlayerTotal,DealerCard,EV_Stand,EV_Hit,EV_Double,OptimalMove\n";

    std::cout << "Calculating Hard Totals (5-21)..." << std::endl;
    for (uint8_t pTotal = 5; pTotal <= 21; ++pTotal) {
        for (uint8_t dCard = 2; dCard <= 11; ++dCard) {
            Card dealerCard = static_cast<Card>(dCard);
            
            double evStand = sim.calculateEV(pTotal, false, dealerCard, Action::Stand, ITERATIONS);
            double evHit = sim.calculateEV(pTotal, false, dealerCard, Action::Hit, ITERATIONS);
            
            // You can only double on your first two cards, so it's a valid starting action
            double evDouble = sim.calculateEV(pTotal, false, dealerCard, Action::Double, ITERATIONS);
            
            std::string optimal = getOptimalMove(evStand, evHit, evDouble);

            outFile << "Hard," << static_cast<int>(pTotal) << "," << static_cast<int>(dCard) << "," 
                    << evStand << "," << evHit << "," << evDouble << "," << optimal << "\n";
        }
    }

    std::cout << "Calculating Soft Totals (13-21)..." << std::endl;
    for (uint8_t pTotal = 13; pTotal <= 21; ++pTotal) {
        for (uint8_t dCard = 2; dCard <= 11; ++dCard) {
            Card dealerCard = static_cast<Card>(dCard);
            
            double evStand = sim.calculateEV(pTotal, true, dealerCard, Action::Stand, ITERATIONS);
            double evHit = sim.calculateEV(pTotal, true, dealerCard, Action::Hit, ITERATIONS);
            double evDouble = sim.calculateEV(pTotal, true, dealerCard, Action::Double, ITERATIONS);
            
            std::string optimal = getOptimalMove(evStand, evHit, evDouble);

            outFile << "Soft," << static_cast<int>(pTotal) << "," << static_cast<int>(dCard) << "," 
                    << evStand << "," << evHit << "," << evDouble << "," << optimal << "\n";
        }
    }

    outFile.close();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Pipeline complete. CSV generated in " << elapsed.count() << " seconds." << std::endl;
    return 0;
}