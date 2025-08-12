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

//int countOpponentMines(Player const& player, Players const& players);

//TEST(countOpponentMines, should_return_3_mines)
//{
//    GameContext context;
//    Player player1;
//    Player player2;
//    EnterMineFn enterMine;
//    enterMine(context, player1);
//}

}
