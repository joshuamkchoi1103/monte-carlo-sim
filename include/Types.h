#pragma once
#include <cstdint>

enum class Card : uint8_t {
    Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7, 
    Eight = 8, Nine = 9, Ten = 10, Jack = 10, Queen = 10, King = 10,
    Ace = 11 
};

enum class Action : uint8_t {
    Stand = 0, Hit = 1, Double = 2, Split = 3
};

enum class Result : int8_t {
    Loss = -1, Push = 0, Win = 1, Blackjack = 2
};