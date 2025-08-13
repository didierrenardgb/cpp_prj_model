#include <gtest/gtest.h>
#include <minefield/utils.h>
#include <minefield/game_states.h>

namespace minefield::game::tests
{
class GameTestSuit : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
    
    void addPlayer(unsigned int remainingMines)
    {
        Player player;
        player.remainingMines = remainingMines;
        context.players.push_back(player);
    }

    void setBoard(unsigned int width, unsigned int height) 
    {
        context.board;
        context.width = width;
        context.height = height;
        Utils::initializeBoard(context.board, context.width, context.height);
    }

    GameContext context;
};

TEST_F(GameTestSuit, should_continue_game_if_two_players_have_mines)
{
    setBoard(10, 10);
    addPlayer(3);
    addPlayer(3);

    EXPECT_TRUE(GameStates::stateCheckingNextTurn(context).updateFunction == GameStates::statePuttingMines);
}

TEST_F(GameTestSuit, should_continue_game_if_at_least_two_players_have_mines)
{
    setBoard(10, 10);
    addPlayer(3);
    addPlayer(3);
    addPlayer(0);
    addPlayer(0);

    EXPECT_TRUE(GameStates::stateCheckingNextTurn(context).updateFunction == GameStates::statePuttingMines);
} 

TEST_F(GameTestSuit, should_finish_game_if_board_is_not_initialized)
{
    addPlayer(3);
    addPlayer(3);

    EXPECT_TRUE(GameStates::stateCheckingNextTurn(context).updateFunction == nullptr);
}

TEST_F(GameTestSuit, should_finish_game_if_only_one_player_has_mines)
{
    setBoard(10, 10);
    addPlayer(3);
    addPlayer(0);

    EXPECT_TRUE(GameStates::stateCheckingNextTurn(context).updateFunction == nullptr);
}  

TEST_F(GameTestSuit, should_finish_game_if_players_are_not_initialized)
{
    setBoard(10, 10);

    EXPECT_TRUE(GameStates::stateCheckingNextTurn(context).updateFunction == nullptr);
}
}
