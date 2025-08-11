#pragma once
#include "minefield.h"

NextState setupGame(GameContext &context);
int readIntInRange(unsigned int min, unsigned int max);
void createPlayers(GameContext &context, unsigned int humanPlayers, unsigned int computerPlayers);