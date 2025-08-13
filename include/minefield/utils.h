#pragma once

#include "types.h"
#include "constants.h"

#include <iostream>
#include <set>
#include <string>

namespace Utils
{
    int getStateValue(PositionState state);
    unsigned int enterNumberInRange(const char* message, unsigned int min, unsigned int max);
    void initializeBoard(Board& board, unsigned int height, unsigned int width);
    bool nameExists(std::string const& name, std::vector<Player> const& players);
    int whoHasLessAvailableMines(Players const& players);
    unsigned int createRandomNumberInRange(int max); // getRandomNumberInRange
    MinePosition createRandomBoardPosition(unsigned int width, unsigned int height); // getRandomBoardPosition
    MinePosition enterBoardPosition(unsigned int width, unsigned int height, Player const& player, RandomPosFn randomPos);
    std::string showInvalidPositionReason(PositionState const& state);
    bool isInvalidPosition(PositionState const& state);
    MinePosition validBoardPosition(unsigned int width, unsigned int height, Player const& player); // validBoardPositionState
    void enterMine(GameContext& context, Player& player);
    void savePlayerMines(Player& player);
    void savePlayerGuesses(Player& player);
    void checkPlayerGuessedOwnMine(Player& player, MinePosition const& mine, Board& board);
    void handleOwnMine(Player& player, MinePosition const& mine, Board& board);
    void handleOpponentMine(Player& player, MinePosition const& mine, Board& board, Players const& players);
    void handleMiss(Player const& player, MinePosition const& mine, Board& board);
    bool isMineFromPlayer(MinePosition const& guess, std::vector<MinePosition> const& minePositions);
    void showPlayerBoardPerPlayer(unsigned int width, unsigned int height, Board const& board, Player const& player);
    int countOpponentMines(Player const& player, Players const& players);
    Players getPlayersAfterElimination(Players const& players, Players const& removed);
    bool checkForWinners(Players const& winners);
    bool checkForElimination(Players const& players);
    bool hasAvailablePositionsForMines(unsigned int width, unsigned int height, Board const& board);
    Player const* getPlayerWithHighestScore(Players const& players);
    bool checkBoardFull(unsigned int width, unsigned int height, Board const& board, Players const& players);
}