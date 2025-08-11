#include <minefield/shared.h>
#include <iostream>

void clearStandardInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}