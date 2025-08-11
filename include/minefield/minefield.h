#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <functional>
#include <string>

struct State;
struct GameContext;
typedef State NextState;
typedef NextState (*StateUpdateFn)(GameContext &);

struct State
{
    StateUpdateFn updateFunction = nullptr;
};

struct Board
{
    unsigned int width = 0;
    unsigned int height = 0;
    std::vector<int> boardCells;
    std::vector<unsigned int> availableCells;
    unsigned int initialMines = 0;
};

enum class PlayerType
{
    Human,
    Computer
};

struct Player
{
    std::string name;
    PlayerType type = PlayerType::Computer;
    std::vector<unsigned int> mines;
    std::vector<unsigned int> guesses;
};

inline std::mt19937 initializeRandomGenerator()
{
    std::random_device rd;
    std::mt19937 randomGenerator(rd());
    return randomGenerator;
}

struct RandomGenerator
{
    std::mt19937 randomGenerator = initializeRandomGenerator();

    int operator()(int min, int max)
    {
        std::uniform_int_distribution<> distrib{min, max};
        return distrib(randomGenerator);
    }
};

typedef std::function<int(int, int)> RandomGenerationFn;

struct GameContext
{
    State currentState;
    Board board;
    std::vector<Player> players;
    RandomGenerationFn randomGenerator = RandomGenerator{};
};
