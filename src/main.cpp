#include <iostream>
#include <chrono>
#include "Bitboard.h"
#include "Evaluator.h"

int main() {
    using namespace Poker;
    
    // Create a dummy board with a Full House (Tens full of Aces)
    Bitboard board;
    board.addCard(8);  // Ten of Spades
    board.addCard(21); // Ten of Hearts
    board.addCard(34); // Ten of Diamonds
    board.addCard(12); // Ace of Spades
    board.addCard(25); // Ace of Hearts
    board.addCard(40); // Random Club
    board.addCard(41); // Random Club

    const size_t ITERATIONS = 50'000'000; 

    std::cout << "Starting Ultra-Low-Latency Bitwise Evaluator Benchmark..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    int fullHouseCount = 0;
    for (size_t i = 0; i < ITERATIONS; ++i) {
        // Run the bitwise evaluator 50 million times
        if (Evaluator::evaluate(board) == HandRank::FullHouse) {
            fullHouseCount++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Evaluated " << ITERATIONS << " hands in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Throughput: " << (ITERATIONS / elapsed.count()) / 1'000'000 << " Million hands/second." << std::endl;
    
    return 0;
}