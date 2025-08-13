#include <gtest/gtest.h>
#include <minefield/utils.h>
#include <minefield/types.h>

namespace minefield::utils::tests
{
    
TEST(createRandomNumberInRangeFn, should_return) 
{
    int num = Utils::createRandomNumberInRange(10);
    bool cond = num < 10;
    EXPECT_TRUE(cond);
}

TEST(isMineFromPlayer, should_return_true_when_mine_is_from_player)
{
    MinePosition guess{ 1, 1 };
    MinePosition a{ 1, 1 };
    MinePosition b{ 2, 2 };
    MinePosition c{ 3, 3 };
    std::vector<MinePosition> minePositions = {a, b, c};
    EXPECT_TRUE(Utils::isMineFromPlayer(guess, minePositions));
}

TEST(isMineFromPlayer, should_return_false_when_mine_is_not_from_player)
{
    MinePosition guess{5, 5};
    MinePosition a{1, 1};
    MinePosition b{2, 2};
    MinePosition c{3, 3};
    std::vector<MinePosition> minePositions = {a, b, c};
    EXPECT_FALSE(Utils::isMineFromPlayer(guess, minePositions));
}
TEST(isInvalidPosition, should_return_false_if_mine_is_empty_or_with_mine)
{
    MinePosition mineA{ 1, 1, PositionState::Empty };
    MinePosition mineB{ 1, 1, PositionState::WithMine };
    EXPECT_FALSE(Utils::isInvalidPosition(mineA.state));
    EXPECT_FALSE(Utils::isInvalidPosition(mineB.state));
}
TEST(nameExists, should_return_false_if_player_vector_is_empty) 
{
    EXPECT_FALSE(Utils::nameExists("PlayerName", Players{}));
}

TEST(nameExists, should_return_true_if_name_exists_in_player_vector)
{
    Players players = Players{};
    Player p1;

    p1.name = "PlayerName";
    players.emplace_back(p1);
    EXPECT_TRUE(Utils::nameExists("PlayerName", players));
}

TEST(checkBoardFull, should_return_true_if_board_is_full)
{
    unsigned int width = 5;
    unsigned int height = 5;
    Players players;
    Board board;
    Utils::initializeBoard(board, height, width);
    
    for (unsigned int x = 0; x < board.size(); x++)
    {
        for (unsigned int y = 0; y < board[0].size(); y++)
        {
            board[x][y].state = PositionState::WithMine;
        }
    }

    EXPECT_TRUE(Utils::checkBoardFull(width, height, board, players));
}

TEST(getPlayerWithHighestScore, should_return_false_if_players_is_empty)
{
    EXPECT_EQ(Utils::getPlayerWithHighestScore(Players{}), nullptr);
}

}
