#include <iostream>
#include <minefield/minefield.h>
#include <minefield/exit_game.h>
#include <minefield/show_board.h>

unsigned int getMaxMineCount(const std::vector<Player> &players)
{
    unsigned int max = 0;
    for (const Player &player : players)
    {
        if (player.mines.size() > max)
        {
            max = player.mines.size();
        }
    }
    return max;
}

NextState processRoundResult(GameContext &context)
{
    unsigned int maxMineCount = getMaxMineCount(context.players);
    if (maxMineCount > context.board.availableCells.size())
    {
        std::cout << "It looks like the available cells on the board are not enough to keep playing. We'll have to call it a tie between the remaining players and start over!" << std::endl;
        return {&exitGame};
    }

    for (auto iterator = context.players.begin(); iterator != context.players.end();)
    {
        if (iterator->mines.empty())
        {
            std::cout << iterator->name << " got completely mined out!" << std::endl;
            iterator = context.players.erase(iterator);
        }
        else
        {
            ++iterator;
        }
    }

    if (context.players.empty())
    {
        std::cout << "A perfect tie, in total destruction. No mines, no survivors!" << std::endl;
        return {&exitGame};
    }
    else if (context.players.size() == 1)
    {
        std::cout << context.players[0].name << " takes the crown! The sole survivor of the minefield!" << std::endl;
        return {&exitGame};
    }
    else
    {
        std::cout << context.players.size() << " players still in the game. Next round begins!" << std::endl;
        return {&showBoard};
    }
}