#include <iostream>
#include <vector>
#include <set>
#include <minefield/process_mine_detection.h>
#include <minefield/process_round_result.h>

void printExplodedMinesMessage(const std::set<unsigned int> &explodedMines, const std::string &playerName)
{
    if (!explodedMines.empty())
    {
        std::cout << playerName << ", you lost your mine(s) at spot(s) ";
        for (auto it = explodedMines.begin(); it != explodedMines.end(); ++it)
        {
            std::cout << *it;
            if (std::next(it) != explodedMines.end())
            {
                std::cout << ", ";
            }
        }
        std::cout << '.' << std::endl;
    }
}

void disableCell(std::vector<int> &cells, int cell)
{
    static const int kDisabledCell = -1;
    auto cellToDisable = std::find(cells.begin(), cells.end(), cell);
    if (cellToDisable != cells.end())
    {
        *cellToDisable = kDisabledCell;
    }
}

void disableUsedCells(std::unordered_set<unsigned int> usedCells, Board &board)
{
    for (unsigned int cell : usedCells)
    {
        std::ignore = std::remove(board.availableCells.begin(), board.availableCells.end(), cell);
        disableCell(board.boardCells, cell);
    }
}

void resizePlayerMineCount(std::unordered_map<Player *, int> playerExplodedMineCount)
{
    for (auto [playerPtr, count] : playerExplodedMineCount)
    {
        if (count > 0 && count <= playerPtr->mines.size())
        {
            playerPtr->mines.resize(playerPtr->mines.size() - count);
        }
    }
}

std::set<unsigned int> getExplodedMines(const std::vector<unsigned int> &minesToCheck, const std::vector<unsigned int> &minesToMatchAgainst)
{
    std::set<unsigned int> explodedMines;

    for (unsigned int mineToCheck : minesToCheck)
    {
        for (int mineToMatchAgainst : minesToMatchAgainst)
        {
            if (mineToCheck == mineToMatchAgainst)
            {
                explodedMines.insert(mineToCheck);
            }
        }
    }
    return explodedMines;
}

NextState processMineDetection(GameContext &context)
{
    std::unordered_set<unsigned int> boardUsedCells;
    std::unordered_map<Player *, int> playerExplodedMineCount;

    for (Player &player : context.players)
    {
        std::set<unsigned int> explodedMines = getExplodedMines(player.mines, player.guesses); // self hits
        for (Player &rival : context.players)
        {
            if (&player != &rival)
            {
                auto rivalMinesHits = getExplodedMines(player.mines, rival.mines);
                explodedMines.insert(rivalMinesHits.begin(), rivalMinesHits.end());

                auto rivalGuessesHits = getExplodedMines(player.mines, rival.guesses);
                explodedMines.insert(rivalGuessesHits.begin(), rivalGuessesHits.end());
            }
        }

        printExplodedMinesMessage(explodedMines, player.name);

        for (unsigned int cell : player.mines)
        {
            boardUsedCells.insert(cell);
        }
        for (unsigned int cell : player.guesses)
        {
            boardUsedCells.insert(cell);
        }

        playerExplodedMineCount[&player] = explodedMines.size();
    }

    resizePlayerMineCount(playerExplodedMineCount);
    disableUsedCells(boardUsedCells, context.board);

    return {&processRoundResult};
}