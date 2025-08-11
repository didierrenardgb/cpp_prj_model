#pragma once
#include "minefield.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <set>

NextState processMineDetection(GameContext &context);
std::set<unsigned int> getExplodedMines(const std::vector<unsigned int> &minesToCheck, const std::vector<unsigned int> &minesToMatchAgainst);
void disableUsedCells(std::unordered_set<unsigned int> usedCells, Board &board);
void disableCell(std::vector<int> &cells, int cell);
void resizePlayerMineCount(std::unordered_map<Player *, int> playerExplodedMineCount);
void printExplodedMinesMessage(const std::set<unsigned int> &explodedMines, const std::string &playerName);