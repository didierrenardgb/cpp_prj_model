#pragma once

#include "types.h"

namespace GameStates
{
    State stateMainMenuUpdate (GameContext& context);
    State stateEnteringBoardMeasures (GameContext& context);
    State stateEnteringMineCount (GameContext& context);
    State stateCreatingPlayers (GameContext& context);
    State statePuttingMines (GameContext& context);
    State stateProcessingMines (GameContext& context);
    State stateGuessingMines (GameContext& context);
    State stateProcessingGuesses (GameContext& context);
    State stateCheckingNextTurn (GameContext& context);
}