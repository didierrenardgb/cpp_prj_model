#pragma once
#include "minefield.h"

template<typename T>
using GetInputFn = T(*)();

template <typename T>
T getInputFromCin()
{
    T value;
    while (true)
    {
        std::cin >> value;
        if (!std::cin.fail())
        {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Try again: ";
    }
}

NextState setupGame(GameContext &context);
int readIntInRange(unsigned int min, unsigned int max, GetInputFn<unsigned int> getIntInput);
void createPlayers(GameContext &context, unsigned int humanPlayers, unsigned int computerPlayers, GetInputFn<std::string> getStringInput);


