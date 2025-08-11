#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <minefield/minefield.h>
#include <minefield/select_mines.h>
#include <minefield/process_mine_detection.h>
#include <minefield/exit_game.h>
#include <minefield/shared.h>

void printMineSelection(const std::vector<unsigned int> &mines, const std::string &message = "")
{
    std::cout << message << std::endl;
    for (int mine : mines)
    {
        std::cout << mine << ' ';
    }
    std::cout << std::endl;
}

void randomMineSelection(int selectionSize, std::vector<unsigned int> &randomSelection, const std::vector<unsigned int> &availableCells, const RandomGenerationFn &randomGenerator)
{
    randomSelection.clear();
    while (randomSelection.size() < selectionSize)
    {
        int randomIndex = randomGenerator(0, availableCells.size() - 1);
        int candidate = availableCells[randomIndex];
        if (std::find(randomSelection.begin(), randomSelection.end(), candidate) == randomSelection.end())
        {
            randomSelection.push_back(candidate);
        }
    }
}

void manualMineSelection(int selectionSize, std::vector<unsigned int> &minesVector, const std::vector<unsigned int> &availableCells)
{
    minesVector.clear();
    for (int i = 0; i < selectionSize; ++i)
    {
        bool validSelectedCell = false;
        unsigned int selectedCell = 0;

        while (!validSelectedCell)
        {
            std::cout << "Enter cell #" << (i + 1) << ": ";
            std::cin >> selectedCell;

            if (std::cin.fail())
            {
                clearStandardInput();
                std::cout << "Oops! That doesn't look like a number. Try again!" << std::endl;
                continue;
            }

            if (std::find(availableCells.begin(), availableCells.end(), selectedCell) == availableCells.end())
            {
                std::cout << "Cell " << selectedCell << " is disabled! Please choose another one" << std::endl;
                continue;
            }

            if (std::find(minesVector.begin(), minesVector.end(), selectedCell) != minesVector.end())
            {
                std::cout << "You already picked cell " << selectedCell << ". Please choose a different one!" << std::endl;
                continue;
            }

            validSelectedCell = true;
        }
        minesVector.push_back(selectedCell);
    }
}

unsigned int getRivalsMaxMineCount(const Player &player, const std::vector<Player> &players)
{
    unsigned int max = 0;
    for (const Player &rivalPlayer : players)
    {
        if (&rivalPlayer != &player && rivalPlayer.mines.size() > max)
        {

            max = rivalPlayer.mines.size();
        }
    }
    return max;
}

NextState selectGuesses(GameContext &context)
{
    std::cout << "Alright, it's time to strike!" << std::endl;
    std::cout << "Remember: if you pick the spots where you hid your own mines, you'll blow up your stash!" << std::endl;
    for (Player &player : context.players)
    {
        unsigned int minesToGuess = getRivalsMaxMineCount(player, context.players);
        if (player.type == PlayerType::Human)
        {
            std::cout << player.name << ", just so you don't trip over your own traps: your mines are in spots ";
            printMineSelection(player.mines);

            std::cout << "You've got " << minesToGuess << " guess(es) --just as many as the mines your most dangerous rival has left. Where will you shoot?" << std::endl;
            manualMineSelection(minesToGuess, player.guesses, context.board.availableCells);
        }
        else
        {
            randomMineSelection(minesToGuess, player.guesses, context.board.availableCells, context.randomGenerator);
        }
#ifdef DEBUG
        printMineSelection(player.guesses, player.name + "'s guesses: ");
#endif
    }
    return {&processMineDetection};
}

NextState selectMines(GameContext &context)
{
    for (Player &player : context.players)
    {
        if (player.type == PlayerType::Human)
        {
            std::cout << player.name << " it's your turn! Pick " << player.mines.size() << " spot(s) to hide your mine(s)" << std::endl;
            manualMineSelection(player.mines.size(), player.mines, context.board.availableCells);
        }
        else
        {
            randomMineSelection(player.mines.size(), player.mines, context.board.availableCells, context.randomGenerator);
        }
#ifdef DEBUG
        printMineSelection(player.mines, player.name + "'s mines: ");
#endif
    }
    return {&selectGuesses};
}
