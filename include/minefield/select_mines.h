#pragma once
#include "minefield.h"
#include <vector>
#include <random>

NextState selectMines(GameContext &context);
NextState selectGuesses(GameContext &context);
void manualMineSelection(int selectionSize, std::vector<unsigned int> &minesVector, const std::vector<unsigned int> &availableCells);
void randomMineSelection(int selectionSize, std::vector<unsigned int> &randomSelection, const std::vector<unsigned int> &availableCells, const RandomGenerationFn &randomGenerator);
void printMineSelection(const std::vector<int> &mines, const std::string &message = "");
unsigned int getRivalsMaxMineCount(const Player &player, const std::vector<Player> &players);