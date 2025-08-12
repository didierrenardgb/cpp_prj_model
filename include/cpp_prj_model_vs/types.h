#pragma once

#include <vector>
#include <string>

enum class PositionState
{
    Empty,         // -> 0
    WithMine,      // -> 1 
    Removed,       // -> 2, if two mines are in the same position, they are both removed
    GuessedEmpty,  // -> 3, a player guessed a position that didn't have a mine
    GuessedMine    // -> 4, a player correctly guessed a mine
};

enum class PlayerType
{
    None,
    HumanPlayer,
    PC
};

struct MinePosition
{
    unsigned int x = 0;
    unsigned int y = 0;
    PositionState state = PositionState::Empty; 

    bool operator<(MinePosition const& other) const 
    {
        return (x < other.x) || (x == other.x && y < other.y);
    }

    bool operator==(MinePosition const& other) const 
    {
        return x == other.x && y == other.y;
    }
};

struct Player;
struct State;
struct GameContext;

typedef std::vector<std::vector<MinePosition>> Board;
typedef std::vector<Player> Players;
typedef State NextState;
typedef NextState (*StateUpdateFn)(GameContext&);
typedef void (*EnterMineFn)(GameContext&, Player&);

// used in utils.cpp

typedef MinePosition(*EnterPosFn)(unsigned int, unsigned int, Player const&);
typedef MinePosition(*RandomPosFn)(unsigned int, unsigned int);

struct Player
{
    std::string name;
    PlayerType type = PlayerType::None;
    std::vector<MinePosition> placedMines;
    std::vector<MinePosition> placedGuesses;
    std::vector<MinePosition> minesHistory;
    std::vector<MinePosition> guessesHistory;
    unsigned int remainingMines = 0;
    unsigned int remainingGuesses = 0;
    unsigned int opponentMinesDetected = 0;
    unsigned int ownMinesDetected = 0;
    EnterMineFn enterMine = nullptr;

    bool operator==(Player const &other) const
    {
        return (name == other.name);
    }
};

struct State
{
    StateUpdateFn updateFunction = nullptr;
};

struct LanguagePack;

struct GameContext
{
    State currentState;
    unsigned int width = 0;
    unsigned int height = 0;
    Board board;
    unsigned int round = 1;
    unsigned int mines = 0;
    unsigned int initialMines = 0;
    Players players;
    const LanguagePack* currentLanguage;
};

/*
struct LanguagePack
{
    struct MainMenu
    {
        const char* const& kHeader;
        const char* const& kStart;
        const char* const& kQuit;
        const char* const& kPrompt;
        const char* const& kThanksForPlaying;
        const char* const& kInvalidOption;
    };

    struct BoardConfig
    {
        const char* const& kHeader;
        const char* const& kConfigMsg;
        const char* const& kEnterWidth;
        const char* const& kEnterHeight;
        const char* const& kSetMsg;
    };

    struct MineConfig
    {
        const char* const& kHeader;
        const char* const& kExplain;
        const char* const& kEnterMines;
    };

    namespace PlayerCreation
    {
        // namespace Options
        // {
        //     static constexpr char kStopCreation = '*';
        //     static constexpr char kHuman = 'H';
        //     static constexpr char kPC = 'P';
        // }

        static constexpr char const *kHeader = "\nPLAYER CREATION\n";
        static constexpr char const *kNamePrompt = "\nEnter player name or {} to finish: ";
        static constexpr char const *kTypePrompt = "\nSelect type for {} ('{}' for human or '{}' for PC): ";
        static constexpr char const *kInvalidType = "\nInvalid type. Please enter '{}' or '{}': ";
        static constexpr char const *kAdded = "\nPlayer {} was successfully added to the game!\n";
        static constexpr char const *kPCAdded = "\n{} will be playing against {}!\n";
        static constexpr char const *kCreated = "\n{} player(s) were created!\n";
        static constexpr char const *kZeroAdded = "\nNo players have been added to the game!\n";
        static constexpr char const *kPCName = "PC";
        static constexpr char const *kRepeatedName = "\nEnter a new name. '{}' was already used!\n";
    }

    namespace PuttingMines
    {
        static constexpr char const *kHeader = "\nPUTTING MINES\n";
        static constexpr char const *kFirstRound = "\nFIRST ROUND\n";
        static constexpr char const *kRoundNumber = "\nROUND NO. {}\n";
        static constexpr char const *kPlayersWillPlaceMines = "\nPlayers will place {} mine(s)!\n";
        static constexpr char const *kNoAvailableMines = "\nThere are no available mines to place!\n";
        static constexpr char const *kPlayerTurn = "\nIt's {}'s turn to place mines\n";
        static constexpr char const *kMessage = "\nMine {} of {}\n";
        static constexpr char const *kSuccessMessage = "\n{} placed a mine at ({}, {})\n";
    }

    namespace ProcessingMines
    {
        static constexpr char const *kHeader = "\nPROCESSING MINES\n";
        static constexpr char const *kNoCollisions = "\nThere haven't been any mine collisions!\n";
        static constexpr char const *kColissionMsg = "\nBoom! There has been a collision at ({},{})\n";
    }

    namespace GuessingMines
    {
        static constexpr char const *kHeader = "\nIT'S TIME TO GUESS MINES\n";
        static constexpr char const *kTotalMsg = "\nAll players have the same amount of guesses = {}\n";
        static constexpr char const *kPlayerTurn = "\nIt's {}'s turn to guess mines!\n";
        static constexpr char const *kSuccess = "\n{} guessed a mine at ({},{})\n";
    }

    namespace ProcessingGuesses
    {
        static constexpr char const *kHeader = "\nPROCESSING GUESSES\n";
        static constexpr char const *kPlayerHeader = "\n{}'s guesses\n";
        static constexpr char const *kHitOwnMine = "\n{} hit their OWN mine at ({},{})!";
        static constexpr char const *kMinesRemaining = "\n   -> Mines remaining: {}";
        static constexpr char const *kHitOpponentMine = "\n{} found an opponent's mine at ({},{})";
        static constexpr char const *kItWasPlayersMine = "\n   -> It was {}'s mine!";
        static constexpr char const *kMiss = "\n{} missed at ({},{})\n";
        static constexpr char const *kCurrentScoresHeader = "\nCURRENT SCORES\n";
        static constexpr char const *kScoreLine = "\n{}: {} opponent mines found, {} own mines hit\n";
    }

    namespace Results
    {
        static constexpr char const *kHeader = "\nROUND {} RESULTS\n";
        static constexpr char const *kPlayerInformation = "\n{}: \n - Found {} of {} opponent mines\n - Has {} mines left to place\n";
        static constexpr char const *kHeaderGameOverWinner = "\nGAME OVER - WE HAVE A WINNER!\n";
        static constexpr char const *kWinnerWins = "\n{} WINS!\n";
        static constexpr char const *kCongratulations = "\nCongratulations on finding all opponent mines!\n";
        static constexpr char const *kTie = "\nIt's a TIE!\n";
        static constexpr char const *kWinnersListHeader = "\nWinners:\n";
        static constexpr char const *kWinnerListItem = "\n  - {}";
        static constexpr char const *kHeaderGameOver = "\nGAME OVER\n";
        static constexpr char const *kWinnerByElimination = "\n{} wins by elimination!\n";
        static constexpr char const *kNoPlayersRemainingTie = "\nNo players remaining!\n";
        static constexpr char const *kHeaderGameOverBoardFull = "\nGAME OVER - BOARD FULL\n";
        static constexpr char const *kNoMorePositions = "\nNo more positions available for mines!\n";
        static constexpr char const *kFinalScores = "\nFinal scores:\n";
        static constexpr char const *kScoreOfPlayer = "\n- {}: {} points";
        static constexpr char const *kWinnerByPoints = "\nWinner by points: {}\n";
        static constexpr char const *kProceedRound = "\nProceeding to round {}...\n";
    }

    namespace UtilsMsg
    {
        static constexpr char const *kEnterXValue = "\nEnter X value (between {} and {}): ";
        static constexpr char const *kEnterYValue = "\nEnter Y value (between {} and {}): ";
        static constexpr char const *kTryAgain = "Try again.";

        static constexpr char const *kAlreadyGuessedMessage = "\nPosition was already guessed.\n";
        static constexpr char const *kAlreadyDetectedMessage = "\nMine was already detected here.\n";
        static constexpr char const *kRemovedMessage = "\nMines were removed from here.\n";

        static constexpr char const *kBoardOfPlayerPrompt = "\nBoard of player {}:\n\n";

        static constexpr char const *kEmptyBoard = "\nBoard wasn't initialized!\n";
        static constexpr char const *kEmptyPlayers = "\nThe list of players is empty!!\n";
    }
};
*/