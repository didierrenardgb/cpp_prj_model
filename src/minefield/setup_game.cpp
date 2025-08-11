#include <iostream>
#include <limits>
#include <minefield/minefield.h>
#include <minefield/show_board.h>
#include <minefield/shared.h>

int readIntInRange(unsigned int min, unsigned int max)
{
    unsigned int input = 0;
    bool validInput = false;
    while (!validInput)
    {
        std::cin >> input;
        if (std::cin.fail())
        {
            clearStandardInput();
            std::cout << "Invalid move! Digits only, sneaky miner!" << std::endl;
            continue;
        }
        validInput = input >= min && input <= max;
        if (!validInput)
        {
            std::cout << "Too wild! Choose something in the safe zone: " << min << " to " << max << std::endl;
        }
    }
    return input;
}

void createPlayers(GameContext &context, unsigned int humanPlayers, unsigned int computerPlayers)
{
    context.players.clear();
    std::vector<std::string> usedNames;
    unsigned int playerNumber = 1;

    for (unsigned int i = 0; i < humanPlayers; ++i)
    {
        std::string name;
        bool validName = false;

        while (!validName)
        {
            std::cout << "What's the name of human player #" << playerNumber << "? ";
            std::cin >> name;

            validName = std::find(usedNames.begin(), usedNames.end(), name) == usedNames.end();
            if (!validName)
            {
                std::cout << "That name already exists. Please choose a different one!";
            }
            else
            {
                usedNames.push_back(name);
            }
        }

        Player player;
        player.name = name;
        player.type = PlayerType::Human;

        context.players.push_back(player);
        ++playerNumber;
    }

    for (unsigned int i = 0; i < computerPlayers; ++i)
    {
        Player player;
        player.name = "Computer_" + std::to_string(i + 1);
        player.type = PlayerType::Computer;
        std::cout << "Our creative team (the compiler) named computer player #" << playerNumber << ": " << player.name << std::endl;

        context.players.push_back(player);
        ++playerNumber;
    }
}

NextState setupGame(GameContext &context)
{
    std::cout << "=================================" << std::endl;
    std::cout << "Welcome to Minefield, brave soul!" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << "Let's start defining the basics of the game" << std::endl;

    std::cout << "First things first: how many human players will be joining the game? (up to 5!)" << std::endl;
    int humanPlayers = readIntInRange(1, 5);

    std::cout << "And how many fearless computer-controlled players shall we unleash? (again, no more than 5)" << std::endl;
    int computerPlayers = readIntInRange(0, 5);

    std::cout << "Perfect! That makes us " << humanPlayers + computerPlayers << ". Now, to keep things organized, let's name our heroes." << std::endl;
    createPlayers(context, humanPlayers, computerPlayers);

    std::cout << "Let's craft the board now. How many tiles across? (it should be a number between 24 and 50)" << std::endl;
    context.board.width = readIntInRange(5, 50);

    std::cout << "Now, how many tiles from top to bottom? (again, between 24 and 50)" << std::endl;
    context.board.height = readIntInRange(5, 50);

    std::cout << "Final step: how many mines should we drop? (let's say, between 3 and 8)" << std::endl;
    context.board.initialMines = readIntInRange(3, 8);

    for (Player &player : context.players)
    {
        player.mines = std::vector<unsigned int>(context.board.initialMines);
        player.guesses = std::vector<unsigned int>(context.board.initialMines);
    }

    for (int i = 0; i < context.board.width * context.board.height; i++)
    {
        context.board.boardCells.push_back(i + 1);
        context.board.availableCells.push_back(i + 1);
    }
    return {&showBoard};
}