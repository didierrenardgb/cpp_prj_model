#include <minefield/shared.h>
#include <iostream>
#include <random>

void clearStandardInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::mt19937 initializeRandomGenerator()
{
    std::random_device rd;
    std::mt19937 randomGenerator(rd());
    return randomGenerator;
}