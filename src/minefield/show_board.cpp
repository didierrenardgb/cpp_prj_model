#include <minefield/minefield.h>
#include <minefield/select_mines.h>

#include <iomanip>
#include <iostream>

NextState showBoard(GameContext& context)
{
    std::cout << "What a battlefield! Hide your mines carefully --but remember, tiles marked with -1 are disabled!" << std::endl;

    for (int i = 0; i < context.board.boardCells.size(); ++i)
    {
        if (i % context.board.width == 0)
        {
            std::cout << std::endl;
        }
        std::cout << std::setw(3) << context.board.boardCells[i] << " ";
    }
    std::cout << std::endl;
    return {&selectMines};
}
