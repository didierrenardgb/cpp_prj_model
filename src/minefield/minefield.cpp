#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iomanip>
#include <random>
#include <sstream>
#include <thread>
#include <chrono> 
#include <algorithm>

static const int kMinDim = 5;
static const int kMaxDim = 50;
static const int kMinMine = 1;
static const int kMaxMine = 8;

enum class CellState
{
    Empty,
    Disabled,
    Used
};

enum class AxisOptions
{
    AxisOptionWidth,
    AxisOptionHeight
};

struct Board
{
    unsigned int width = 0;
    unsigned int height = 0;
    std::vector<std::vector<CellState>> grid;
    unsigned int cellCount = 0;
};

struct Coord
{
    unsigned int posX = 0;
    unsigned int posY = 0;
};

struct Player
{
    std::string name;
    int id = 0;
    unsigned int numberOfmines = 0;
    bool isBot = false;
    std::vector<Coord> playerMines;
    std::vector<Coord> playerGuesses;
};

struct State;
struct GameContext;
typedef State NextState;
typedef NextState (*StateUpdateFn)(GameContext &);

struct State
{
    StateUpdateFn updateFunction;
};

struct GameContext
{
    State currentState;
    Board table;
    std::vector<Player> players;
    unsigned int guesses = 0;
};

enum class Color
{
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};


char const *const colorString(Color forColor)
{
    switch (forColor)
    {
    case Color::Red:
        return "\033[1;31m";
    case Color::Green:
        return "\033[1;32m";
    case Color::Yellow:
        return "\033[1;33m";
    case Color::Blue:
        return "\033[1;34m";
    case Color::Magenta:
        return "\033[1;35m";
    case Color::Cyan:
        return "\033[1;36m";
    case Color::White:
        return "\033[1;37m";
    default:
        return "InvalidColor";
    }
}
std::string applyColorWindows(Color color, std::string const &toMessage)
{
    static char const* const kResetColor = "\033[0m";
    return std::string{colorString(color)} + toMessage + kResetColor;
}

std::string applyColorPosix(Color color, std::string const &toMessage)
{
    static char const *const kResetColor = "\033[0m";
    return std::string{colorString(color)} + toMessage + kResetColor;
}

std::string applyColor(Color color, std::string const &toMessage)
{
#ifdef WIN32
    return applyColorWindows(color, toMessage);
#else
    return applyColorPosix(color, toMessage);
#endif
}

void showMenu()
{
    std::string titleMenu = applyColor(Color::Yellow, "===== MENU =====\n");
    std::cout << titleMenu;
    std::cout << "1. Play\n2. Quit\n> ";
}

void showGameTittle(const Board board)
{
    static const unsigned int kCellWidth = 4;
    static const unsigned int kEdge = 10;
    unsigned int boardWidth = (board.width * kCellWidth) + kEdge;
    std::string boardTitle = applyColor(Color::Red, "MINEFIELD");
    unsigned int padding = (boardWidth - boardTitle.length()) / 2;
    std::cout << std::setfill('=') << std::setw(padding)        // left fill
              << ' ' << boardTitle << ' '                       // tittle with spaces
              << std::setw(padding) << ' ' << std::setfill(' ') // right fill
              << std::endl;
}

void showPlayerData(Player const p)
{
    if (p.isBot)
    {
        std::cout << "The computer is locating the mines\n";
        std::cout << "The computer "<< p.name <<" has " << p.numberOfmines << " mines left\n";
    }
    else
    {
        std::cout << "Player " << p.name << " can enter the coordinates of the mines\n";
        std::cout << "You have " << p.numberOfmines << " mines left\n";
    }
}

void showBoard(const Board &board)
{
    showGameTittle(board);
    std::cout << "   ";
    for (int j = 0; j < board.width; ++j)
    {
        std::cout << std::setw(4) << j;
    }
    std::cout << '\n';
    std::cout << "   ";
    for (int j = 0; j < board.width; ++j)
    {
        std::cout << "----";
    }
    std::cout << "-\n";

    for (int i = 0; i < board.grid.size(); ++i)
    {
        std::cout << std::setw(2) << i << '|';

        for (int j = 0; j < board.grid[i].size(); ++j)
        {
            std::string symbol = (board.grid[i][j] == CellState::Disabled) ? applyColor(Color::Red, "   X") : " O";
            std::cout << std::setw(4) << symbol;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void clearConsoleBuffer()
{
    static char const *const kClearConsoleBufferCommand = "cls";
    system(kClearConsoleBufferCommand);
}

Coord askValidCoord(const Board &board)
{
    unsigned int x = board.width + 1;
    unsigned int y = board.height + 1;
    while (x < 0 || x >= board.width)
    {
        std::cout << "Row: ";
        std::cin >> x;
    }
    while (y < 0 || y >= board.height)
    {
        std::cout << "Column: ";
        std::cin >> y;
    }

    return {x, y};
}

unsigned int makeRandomDistribution(unsigned int measurement)
{
    static std::random_device random;
    static std::mt19937 gen(random());
    std::uniform_int_distribution<unsigned int> dist(0, measurement - 1);
    unsigned int pos = dist(gen);
    return pos;
}

Coord makeValidRandomCoord(unsigned int height, unsigned int width)
{
    Coord c;
    c.posX = makeRandomDistribution(height);
    c.posY = makeRandomDistribution(width);
    return c;
}

bool containsCoordinate(const std::vector<Coord> &coordinates, Coord c)
{
    for (int i = 0; i < coordinates.size(); i++)
    {
        if (coordinates[i].posX == c.posX && coordinates[i].posY == c.posY)
        {
            return true;
        }
    }
    return false;
}

void makeCellUsed(Board &board, const Coord &cord)
{
    if (cord.posX < board.width && cord.posY < board.height && board.grid[cord.posX][cord.posY] == CellState::Empty)
    {
        board.grid[cord.posX][cord.posY] = CellState::Used;
    }
}

void cleanPlayerMines(GameContext &ctx)
{
    for (int i = 0; i < ctx.players.size(); i++)
    {
        ctx.players[i].playerMines.clear();
    }
}

Coord getValidBotCoord(Board &board)
{
    Coord cBot = makeValidRandomCoord(board.height, board.width);
    while (board.grid[cBot.posX][cBot.posY] == CellState::Disabled)
    {
        cBot = makeValidRandomCoord(board.height, board.width);
    }
    return cBot;
}

Coord askValidCoordAndEmpty(const Board &board)
{
    Coord validCoord = askValidCoord(board);
    while (board.grid[validCoord.posX][validCoord.posY] == CellState::Disabled)
    {
        std::cout << "Cell already disabled. Pick another.\n";
        validCoord = askValidCoord(board);
    }
    return validCoord;
}

void enterToContinue(bool isBot)
{ 
    if(isBot)
    {
        std::string outMessageBot = applyColor(Color::Yellow, "\nThe computer is thinking...");
        std::cout << outMessageBot;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    else
    {
        std::string outMessageEnter = applyColor(Color::Yellow, "\nPress ENTER to continue...");
        std::cout << outMessageEnter;
        std::cin.ignore();
        std::cin.get();
    }
}

void clsAndShowBoard(Board const &board, bool isBot)
{
    enterToContinue(isBot);
    clearConsoleBuffer();
    showBoard(board);
}

void setPlayersNames(std::vector<Player> &player, int number, unsigned int mines, bool isBot)
{
    std::string playerName;
    std::ostringstream oss;
    if (!isBot)
    {
        std::cout << "Player" << (number + 1) << " -> ";
        std::cin >> playerName;
        player.push_back({playerName, number, mines, false, {}, {}});
    }
    else
    {
        std::cout << "Player" << (number + 1) << " -> Bot" << number << "\n";
        oss << "Bot" << number;
        playerName = oss.str();
        player.push_back({playerName, number, mines, true, {}, {}});
    }
}

void addMines(GameContext &ctx)
{
    for (Player &player : ctx.players)
    {
        showPlayerData(player);
        player.playerMines.resize(player.numberOfmines);
        for (int j = 0; j < player.numberOfmines; j++)
        {
            Coord &mine = player.playerMines[j];
            if (player.isBot)
            {
                mine = getValidBotCoord(ctx.table);
                player.playerMines[j] = mine;
#ifdef DEBUG
                std::cout << "BotMine #" << (j + 1) << " will be at [" << mine.posX << ", " << mine.posY << "]" << std::endl;
#endif
            }
            else
            {
                mine = askValidCoordAndEmpty(ctx.table);
                std::cout << "Mine #" << j + 1 << " will be at [" << mine.posX << ", " << mine.posY << "]\n";
                player.playerMines[j] = mine;
            }
            makeCellUsed(ctx.table, mine);
        }
        clsAndShowBoard(ctx.table, player.isBot);
    }
}

unsigned int amountOfGuesses(const GameContext &ctx)
{
    if (ctx.players.empty())
    {
        return 0;
    }
    unsigned int minGuesses = ctx.players[0].playerMines.size();
    for (int i = 1; i < ctx.players.size(); i++)
    {
        unsigned int size = ctx.players[i].playerMines.size();
        minGuesses = std::min(minGuesses, size);
    }
    return minGuesses;

}

void addGuesses(GameContext &ctx)
{
    unsigned int guessesCount = amountOfGuesses(ctx);
    for (Player &player : ctx.players)
    {
        std::cout << "Player " << player.name << " can enter " << guessesCount << " guesses\n";
        player.playerGuesses.resize(guessesCount);
        for (int j = 0; j < guessesCount; j++)
        {
            Coord &mine = player.playerGuesses[j];
            if (player.isBot)
            {
                mine = getValidBotCoord(ctx.table);
                player.playerGuesses[j] = mine;
#ifdef DEBUG
                std::cout << "BotGuess #" << (j + 1) << " will be at [" << mine.posX << ", " << mine.posY << "]\n";
#endif
            }
            else
            {
                std::cout << "Guess #" << (j + 1) << '\n';
                mine = askValidCoordAndEmpty(ctx.table);
                std::cout << "Guess #" << (j + 1) << " will be at [" << mine.posX << ", " << mine.posY << "]\n";
                player.playerGuesses[j] = mine;
            }
            makeCellUsed(ctx.table, mine);
        }
        clsAndShowBoard(ctx.table, player.isBot);
    }
}

void processGuesses(GameContext &ctx)
{
    for (Player &attacker : ctx.players)
    {
        for (Player &defender : ctx.players)
        {
            if (&attacker == &defender)
            {
                continue;
            }
            for (const Coord &mine : attacker.playerGuesses)
            {
                // player found another player mine
                if (containsCoordinate(defender.playerMines, mine))
                {
                    defender.numberOfmines--;
                }
                // player found their own mine
                if (containsCoordinate(attacker.playerMines, mine))
                {
                    attacker.numberOfmines--;
                }
                makeCellUsed(ctx.table, mine);
            }
        }
    }
}

void resetMines(GameContext &ctx)
{
    for (Player &player : ctx.players)
    {
        for (const Coord &mine : player.playerMines)
        {
            // changes cell state from used to disabled
            if (ctx.table.grid[mine.posX][mine.posY] == CellState::Used)
            {
                ctx.table.grid[mine.posX][mine.posY] = CellState::Disabled;
                --ctx.table.cellCount;
            }
        }
        player.playerMines.clear();
    }
}

unsigned int askValidDimensions(AxisOptions axisOption)
{
    unsigned int value = 0;
    while (value < kMinDim || value > kMaxDim)
    {
        if (axisOption == AxisOptions::AxisOptionWidth)
        {
            std::cout << "Width: ";
        }
        else
        {
            std::cout << "Height: ";
        }
        std::cin >> value;
    }
    return value;
}

unsigned int askValidNumberOfMines()
{
    unsigned int mines = 0;
    while (mines < kMinMine || mines > kMaxMine)
    {
        std::cin >> mines;
    }
    return mines;
}

int getPlayersWithMines(const std::vector<Player> &players)
{
    int playersWithMines = 0;
    for (const Player &player : players)
    {
        if (player.numberOfmines > 0)
        {
            playersWithMines++;
        }
    }
    return playersWithMines;
}

void eraseLoser(std::vector<Player> &players)
{
    players.erase(
        std::remove_if(players.begin(), players.end(), [](const Player& player){
            return player.numberOfmines == 0;
        }), players.end());

}

Player winnerPlayer (const std::vector<Player> &players)
{
    Player winner = players[0];
    for (const Player &player : players)
    {
        if (player.numberOfmines > winner.numberOfmines)
        {
            winner = player;
        }
    }
    return winner;
}

unsigned int amountOfTotalMines(const std::vector<Player> &players)
{
    unsigned int mineCount = 0;
    for(const Player &player : players)
    {
        mineCount += player.numberOfmines;
    }
    return mineCount;
}

NextState stateMainMenuUpdate(GameContext &ctx);
NextState stateConfigUpdate(GameContext &ctx);
NextState statePlaceMines(GameContext &ctx);
NextState stateAddGuesses(GameContext &ctx);
NextState stateProcessGuesses(GameContext &ctx);
NextState stateCheckVictory(GameContext &ctx);

NextState stateMainMenuUpdate(GameContext &ctx)
{
    clearConsoleBuffer();
    showMenu();
    int opt = 0;
    static constexpr int kOptionPlay = 1;
    std::cin >> opt;
    if (opt == kOptionPlay)
    {
        return {&stateConfigUpdate};
    }
    return {nullptr};
}

State stateConfigUpdate(GameContext &ctx)
{
    clearConsoleBuffer();
    ctx.players.clear();
    std::string configurationTitle = applyColor(Color::Yellow, "===== CONFIGURATION =====\n");
    std::cout << configurationTitle;

    std::cout << "\nTable measurements must be between " << kMinDim << " and " << kMaxDim << '\n';
    unsigned int validWidth = askValidDimensions(AxisOptions::AxisOptionWidth);
    unsigned int validHeight = askValidDimensions(AxisOptions::AxisOptionHeight);
    ctx.table.cellCount = validHeight * validWidth;
    std::cout << "\nYou can play with between " << kMinMine << " to " << kMaxMine << " mines" << '\n';
    std::string amountOfPlayerTittle = applyColor(Color::Cyan, "How many mines per players?: ");
    std::cout << amountOfPlayerTittle;
    unsigned int validMines = askValidNumberOfMines();

    int amountOfRealPlayers = 0;
    std::string amountOfHumanPlayersTitle = applyColor(Color::Green, "\nHow many human players?: ");
    std::cout << amountOfHumanPlayersTitle;
    std::cin >> amountOfRealPlayers;

    int amountOfBotPlayers = 0;
    std::string amountOfBotPlayersTittle = applyColor(Color::Green, "How many bot players: ");
    std::cout << amountOfBotPlayersTittle;
    std::cin >> amountOfBotPlayers;

    int totalAmountOfPlayers = amountOfRealPlayers + amountOfBotPlayers;

    std::string playersUsernamesTittle = applyColor(Color::Magenta, "\nPlayers usernames\n");
    std::cout << playersUsernamesTittle;

    for (int i = 0; i < amountOfRealPlayers; i++)
    {
        setPlayersNames(ctx.players, i, validMines, false);
    }

    for (int i = amountOfRealPlayers; i < totalAmountOfPlayers; i++)
    {
        setPlayersNames(ctx.players, i, validMines, true);
    }

    ctx.table.height = validHeight;
    ctx.table.width = validWidth;
    ctx.table.grid = std::vector<std::vector<CellState>>(validHeight, std::vector<CellState>(validWidth, CellState::Empty));
    static const bool kContinueForHuman = false;
    enterToContinue(kContinueForHuman);
    return {&statePlaceMines};
}

State statePlaceMines(GameContext &ctx)
{
    clearConsoleBuffer();
    showBoard(ctx.table);
    cleanPlayerMines(ctx);
    addMines(ctx);
    return {&stateAddGuesses};
}

State stateAddGuesses(GameContext &ctx)
{
    addGuesses(ctx);
    return {&stateProcessGuesses};
}

State stateProcessGuesses(GameContext &ctx)
{
    processGuesses(ctx);
    return {&stateCheckVictory};
}

State stateCheckVictory(GameContext &ctx)
{
    int playersWithMines = getPlayersWithMines(ctx.players);
    if (playersWithMines == 0)
    {
        clearConsoleBuffer();
        showBoard(ctx.table);
        std::cout << "It's a draw!\n";
        enterToContinue(false);
        return {&stateMainMenuUpdate};
    }

    if (playersWithMines == 1)
    {
        for (const Player &player : ctx.players)
        {
            if (player.numberOfmines > 0)
            {
                clearConsoleBuffer();
                showBoard(ctx.table);
                std::cout << player.name << " wins!\n";
                enterToContinue(false);
            }
        }
        return {&stateMainMenuUpdate};
    }

    resetMines(ctx);
    if (ctx.table.cellCount < amountOfTotalMines(ctx.players))
    {
        clearConsoleBuffer();
        showBoard(ctx.table);
        std::cout <<"There are "<< amountOfTotalMines(ctx.players) 
                <<" mines left to place but there are " << ctx.table.cellCount 
                << " cells in the board\n";
        std::string noCellsLeftMessage = "There is no more available cells in the board D:\n";
        applyColor(Color::Red, noCellsLeftMessage);
        std::cout << noCellsLeftMessage;
        std::cout << winnerPlayer(ctx.players).name << " wins!\n";
        enterToContinue(false);
        return {&stateMainMenuUpdate};
    }
    eraseLoser(ctx.players);
    return {&statePlaceMines};
}

void runMainLoop()
{
    GameContext ctx;
    ctx.currentState = {&stateMainMenuUpdate};
    while (ctx.currentState.updateFunction != nullptr)
    {
        ctx.currentState = (*ctx.currentState.updateFunction)(ctx);
    }
}

int main()
{
    runMainLoop();
    return 0;
}