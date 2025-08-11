#pragma once
#include "minefield.h"

NextState processRoundResult(GameContext &context);
unsigned int getMaxMineCount(const std::vector<Player> &players);