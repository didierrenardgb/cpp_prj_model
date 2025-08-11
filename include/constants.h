#pragma once

namespace MainMenu
{
    namespace Options
    {
        static int const kStart = 1;
        static int const kQuit = 2;
    }

    static constexpr char const* kHeader = "\nMINEFIELD GAME\n\n";
    static constexpr char const* kStart = "{} - Start a new game\n";
    static constexpr char const* kQuit  = "{} - Quit\n";
    static constexpr char const* kPrompt = "\nEnter your choice: ";
    static constexpr char const* kThanksForPlaying = "\nThanks for playing!\n";
    static constexpr char const* kInvalidOption = "\nInvalid option. Please try again.\n";
}

namespace Display
{
    static int const kBoardColWidth = 3; // for std::setw()
}

namespace BoardConfig
{
    namespace Limits
    {
        static int const kMinWidth = 24;
        static int const kMaxdWidth = 50;
        static int const kMinHeight = 24;
        static int const kMaxHeight = 50;
    }

    static constexpr char const* kHeader = "\nBOARD SETUP\n\n";
    static constexpr char const* kConfigMsg = "Configure the game board size\n";
    static constexpr char const* kEnterWidth = "\nWidth (enter a number between {} and {}): ";
    static constexpr char const* kEnterHeight = "\nHeight (enter a number between {} and {}): ";
    static constexpr char const* kSetMsg = "\nBoard size set to {}x{}\n";
}

namespace MineConfig
{
    namespace Limits
    {
        static int const kMin = 3;
        static int const kMax = 8;
    }

    static constexpr char const* kHeader = "\nMINE CONFIGURATION\n";
    static constexpr char const* kExplain = "\nEach player will place the same number of mines.\n";
    static constexpr char const* kEnterMines = "\nEnter number of mines (between {} and {}): ";
}

namespace PlayerCreation
{
    namespace Options
    {
        static constexpr char kStopCreation = '*';
        static constexpr char kHuman = 'H';
        static constexpr char kPC = 'P';
    }

    static constexpr char const* kHeader = "\nPLAYER CREATION\n";
    static constexpr char const* kNamePrompt = "\nEnter player name or {} to finish: ";
    static constexpr char const* kTypePrompt = "\nSelect type for {} ('{}' for human or '{}' for PC): ";
    static constexpr char const* kInvalidType = "\nInvalid type. Please enter '{}' or '{}': ";
    static constexpr char const* kAdded = "\nPlayer {} was successfully added to the game!\n";
    static constexpr char const* kPCAdded = "\n{} will be playing against {}!\n";
    static constexpr char const* kCreated = "\n{} player(s) were created!\n";
    static constexpr char const* kZeroAdded = "\nNo players have been added to the game!\n";
    static constexpr char const* kPCName = "PC";
    static constexpr char const* kRepeatedName = "\nEnter a new name. '{}' was already used!\n";
}

namespace PuttingMines
{
    static constexpr char const* kHeader = "\nPUTTING MINES\n";
    static constexpr char const* kFirstRound = "\nFIRST ROUND\n";
    static constexpr char const* kRoundNumber = "\nROUND NO. {}\n";
    static constexpr char const* kPlayersWillPlaceMines = "\nPlayers will place {} mine(s)!\n";
    static constexpr char const* kNoAvailableMines = "\nThere are no available mines to place!\n";
    static constexpr char const* kPlayerTurn = "\nIt's {}'s turn to place mines\n";
    static constexpr char const* kMessage = "\nMine {} of {}\n";
    static constexpr char const* kSuccessMessage = "\n{} placed a mine at ({}, {})\n";
}

namespace ProcessingMines
{
    static constexpr char const* kHeader = "\nPROCESSING MINES\n";
    static constexpr char const* kNoCollisions = "\nThere haven't been any mine collisions!\n";
    static constexpr char const* kColissionMsg = "\nBoom! There has been a collision at ({},{})\n";
}

namespace GuessingMines
{
    static constexpr char const* kHeader = "\nIT'S TIME TO GUESS MINES\n";
    static constexpr char const* kTotalMsg = "\nAll players have the same amount of guesses = {}\n";
    static constexpr char const* kPlayerTurn = "\nIt's {}'s turn to guess mines!\n";
    static constexpr char const* kSuccess = "\n{} guessed a mine at ({},{})\n";
}

namespace ProcessingGuesses
{
    static constexpr char const* kHeader = "\nPROCESSING GUESSES\n";
    static constexpr char const* kPlayerHeader = "\n{}'s guesses\n";
    static constexpr char const* kHitOwnMine = "\n{} hit their OWN mine at ({},{})!";
    static constexpr char const* kMinesRemaining = "\n   -> Mines remaining: {}";
    static constexpr char const* kHitOpponentMine = "\n{} found an opponent's mine at ({},{})";
    static constexpr char const* kItWasPlayersMine = "\n   -> It was {}'s mine!";
    static constexpr char const* kMiss = "\n{} missed at ({},{})\n";
    static constexpr char const* kCurrentScoresHeader = "\nCURRENT SCORES\n";
    static constexpr char const* kScoreLine = "\n{}: {} opponent mines found, {} own mines hit\n";
}

namespace Results
{
    static constexpr char const* kHeader = "\nROUND {} RESULTS\n";
    static constexpr char const* kPlayerInformation = "\n{}: \n - Found {} of {} opponent mines\n - Has {} mines left to place\n";
    static constexpr char const* kHeaderGameOverWinner = "\nGAME OVER - WE HAVE A WINNER!\n";
    static constexpr char const* kWinnerWins = "\n{} WINS!\n";
    static constexpr char const* kCongratulations  = "\nCongratulations on finding all opponent mines!\n";
    static constexpr char const* kTie = "\nIt's a TIE!\n";
    static constexpr char const* kWinnersListHeader = "\nWinners:\n";
    static constexpr char const* kWinnerListItem = "\n  - {}";
    static constexpr char const* kHeaderGameOver = "\nGAME OVER\n";
    static constexpr char const* kWinnerByElimination = "\n{} wins by elimination!\n";
    static constexpr char const* kNoPlayersRemainingTie = "\nNo players remaining!\n";
    static constexpr char const* kHeaderGameOverBoardFull = "\nGAME OVER - BOARD FULL\n";
    static constexpr char const* kNoMorePositions = "\nNo more positions available for mines!\n";
    static constexpr char const* kFinalScores = "\nFinal scores:\n";
    static constexpr char const* kScoreOfPlayer = "\n- {}: {} points";
    static constexpr char const* kWinnerByPoints = "\nWinner by points: {}\n";
    static constexpr char const* kProceedRound = "\nProceeding to round {}...\n";
}

namespace UtilsMsg
{
    static constexpr char const* kEnterXValue = "\nEnter X value (between {} and {}): ";
    static constexpr char const* kEnterYValue = "\nEnter Y value (between {} and {}): ";
    static constexpr char const* kTryAgain = "Try again.";

    static constexpr char const* kAlreadyGuessedMessage = "\nPosition was already guessed.\n";
    static constexpr char const* kAlreadyDetectedMessage = "\nMine was already detected here.\n";
    static constexpr char const* kRemovedMessage = "\nMines were removed from here.\n";

    static constexpr char const* kBoardOfPlayerPrompt = "\nBoard of player {}:\n\n";

    static constexpr char const* kEmptyBoard = "\nBoard wasn't initialized!\n";
    static constexpr char const* kEmptyPlayers = "\nThe list of players is empty!!\n";
}