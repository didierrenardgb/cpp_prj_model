#include <cpp_prj_model_vs/game_states.h>
#include <cpp_prj_model_vs/types.h>
#include <cpp_prj_model_vs/constants.h>
#include <cpp_prj_model_vs/utils.h>

#include <iostream>
#include <cstdio>
#include <format>
#include <string>

namespace GameStates
{
    NextState stateMainMenuUpdate(GameContext& context)
    {
        std::cout << MainMenu::kHeader;
        std::cout << std::format(MainMenu::kStart, MainMenu::Options::kStart);
        std::cout << std::format(MainMenu::kQuit, MainMenu::Options::kQuit);

        std::cout << MainMenu::kPrompt;

        int userSelection = 0;
        std::cin >> userSelection;

        NextState next = { nullptr };
        switch (userSelection)
        {
            case MainMenu::Options::kStart:
                next = { &stateEnteringBoardMeasures };
                break;
            case MainMenu::Options::kQuit:
                std::cout << MainMenu::kThanksForPlaying;
                next = { nullptr };
                break;
            default:
                std::cout << MainMenu::kInvalidOption;
                std::cout << MainMenu::kPrompt;
                next = context.currentState;
                break;
        }
        return next;
    }

    NextState stateEnteringBoardMeasures(GameContext& context)
    {
        std::cout << BoardConfig::kHeader;
        std::cout << BoardConfig::kConfigMsg;

        context.width = Utils::enterNumberInRange(BoardConfig::kEnterWidth, BoardConfig::Limits::kMinWidth, BoardConfig::Limits::kMaxdWidth);
        context.height = Utils::enterNumberInRange(BoardConfig::kEnterHeight, BoardConfig::Limits::kMinHeight, BoardConfig::Limits::kMaxHeight);

        Utils::initializeBoard(context.board, context.height, context.width);

        std::cout << std::format(BoardConfig::kSetMsg, context.width, context.height);

        return { &stateEnteringMineCount };
    }

    NextState stateEnteringMineCount(GameContext &context)
    {
        std::cout << MineConfig::kHeader;
        std::cout << MineConfig::kExplain;

        context.initialMines = Utils::enterNumberInRange(MineConfig::kEnterMines, MineConfig::Limits::kMin, MineConfig::Limits::kMax);
        context.mines = context.initialMines;
        
        return { &stateCreatingPlayers };
    }

    NextState stateCreatingPlayers(GameContext& context)
    {
        std::cout << PlayerCreation::kHeader;

        auto newPlayer = [](std::string const& name, int initialMines)
            {
                Player player;
                player.name = name;
                player.remainingMines = initialMines;
                player.remainingGuesses = initialMines;
                player.enterMine = &Utils::enterMine;
                return player;
            };

        std::string name;
        std::cout << std::format(PlayerCreation::kNamePrompt, PlayerCreation::Options::kStopCreation);
        std::cin >> name;

        // PlayerCreation::Options::kStopCreation is a char '*'
        // It's casted to std::string to be compared with name (std::string)

        std::string stopCreation = std::string(1, PlayerCreation::Options::kStopCreation);

        while (name != stopCreation)
        {
            if (Utils::nameExists(name, context.players))
            {
                std::cout << std::format(PlayerCreation::kRepeatedName, name);
            } 
            else
            {
                Player player = newPlayer(name, context.initialMines);

                // Type value is initialized with X by default
                char type = 'X';

                std::cout << std::format(PlayerCreation::kTypePrompt, player.name, PlayerCreation::Options::kHuman, PlayerCreation::Options::kPC);
                std::cin >> type;

                while (type != PlayerCreation::Options::kHuman && type != PlayerCreation::Options::kPC)
                {
                    std::cout << std::format(PlayerCreation::kInvalidType, PlayerCreation::Options::kHuman, PlayerCreation::Options::kPC);
                    std::cin >> type;
                }

                player.type = (type == PlayerCreation::Options::kHuman) ? PlayerType::HumanPlayer : PlayerType::PC;

                context.players.push_back(player);

                std::cout << std::format(PlayerCreation::kAdded, name);
            }
            
            std::cout << std::format(PlayerCreation::kNamePrompt, PlayerCreation::Options::kStopCreation);
            std::cin >> name;
        }

        if (!context.players.empty())
        {
            if (context.players.size() == 1)
            {
                Player player = newPlayer(PlayerCreation::kPCName, context.initialMines);
                player.type = PlayerType::PC;

                context.players.push_back(player);

                std::cout << std::format(PlayerCreation::kPCAdded, context.players[0].name, player.name);
            }
            else
            {
                std::cout << std::format(PlayerCreation::kCreated, context.players.size());
            }
        }
        else 
        {
            std::cout << PlayerCreation::kZeroAdded;
            return { nullptr };
        }

        return { &statePuttingMines };
    }

    NextState statePuttingMines(GameContext& context)
    {
        if (context.players.empty())
        {
            std::cout << UtilsMsg::kEmptyPlayers;
            return { &stateCreatingPlayers };
        }

        std::cout << PuttingMines::kHeader;

        unsigned int minesToPlace = 0;

        if (context.round == 1)
        {
            minesToPlace = context.initialMines;

            std::cout << std::format(PuttingMines::kFirstRound);
            std::cout << std::format(PuttingMines::kPlayersWillPlaceMines, minesToPlace);
        }
        else
        {
            std::cout << std::format(PuttingMines::kRoundNumber, context.round);

            // If there are more than two players, the number of mines a player can guess 
            // is limited to the player with the fewest mines

            minesToPlace = Utils::whoHasLessAvailableMines(context.players);

            if (minesToPlace == 0)
            {
                std::cout << std::format(PuttingMines::kNoAvailableMines);
                return { nullptr };
            }
            else
            {
                std::cout << std::format(PuttingMines::kPlayersWillPlaceMines, minesToPlace);
                context.mines = minesToPlace; 
            }
        }
        
        for (auto& player : context.players)
        {
            std::cout << std::format(PuttingMines::kPlayerTurn, player.name);

            player.enterMine(context, player);

            std::cout << std::format(UtilsMsg::kBoardOfPlayerPrompt, player.name);
            Utils::showPlayerBoardPerPlayer(context.width, context.height, context.board, player);
        }

        context.round++;

        return { &stateProcessingMines };
    }

    NextState stateProcessingMines(GameContext& context)
    {
        std::cout << ProcessingMines::kHeader;

        std::set<MinePosition> mineSet;
        std::set<MinePosition> duplicateMinesSet;

        for (auto const& player : context.players)
        {
            for (auto const& mine : player.placedMines)
            {
                auto insertion = mineSet.insert(mine);
                if (!insertion.second)
                {
                    duplicateMinesSet.insert(mine);
                }
            }
        }

        if (duplicateMinesSet.empty())
        {
            std::cout << ProcessingMines::kNoCollisions;
        }
        else
        {
            for (auto& player : context.players)
            {
                for (auto& mine : player.placedMines)
                {
                    // If two players placed a mine in the same position, it is removed
                    
                    if (duplicateMinesSet.count(mine) > 0)
                    {
                        std::cout << std::format(ProcessingMines::kColissionMsg, mine.x, mine.y);

                        context.board[mine.x][mine.y].state = PositionState::Removed;

                        duplicateMinesSet.erase(mine);
                    }
                }
            }
        }

        for (auto& player : context.players)
        {
            Utils::savePlayerMines(player);
        }

        return { &stateGuessingMines };
    }

    NextState stateGuessingMines(GameContext& context)
    {
        std::cout << GuessingMines::kHeader;
        
        // The number of guesses the players can make is the same
        // to the number of mines they can place
        
        std::cout << std::format(GuessingMines::kTotalMsg, context.mines);

        for (auto& player : context.players)
        {
            std::cout << std::format(GuessingMines::kPlayerTurn, player.name);

            for (unsigned int i = 0; i < context.mines; i++)
            {
                MinePosition minePosition = Utils::validBoardPosition(context.width, context.height, player);

                std::cout << std::format(GuessingMines::kSuccess, player.name, minePosition.x, minePosition.y);
                
                player.placedGuesses.push_back(minePosition);
            }
        }

        return { &stateProcessingGuesses };
    }

    NextState stateProcessingGuesses(GameContext& context)
    {
        std::cout << ProcessingGuesses::kHeader;

        for (auto& player : context.players)
        {
            std::cout << std::format(ProcessingGuesses::kPlayerHeader, player.name);

            for (auto const& guess : player.placedGuesses)
            {
                // If the mine is from the player, it reduces the amount of mines it can place

                if (Utils::isMineFromPlayer(guess, player.minesHistory)) 
                {
                    Utils::handleOwnMine(player, guess, context.board);
                } 
                else if (context.board[guess.x][guess.y].state == PositionState::WithMine) 
                {
                    Utils::handleOpponentMine(player, guess, context.board, context.players);
                } 
                else 
                {
                    Utils::handleMiss(player, guess, context.board);
                }
            }

            Utils::savePlayerGuesses(player);
            
            std::cout << std::format(UtilsMsg::kBoardOfPlayerPrompt, player.name);
            Utils::showPlayerBoardPerPlayer(context.width, context.height, context.board, player);
        }

        std::cout << ProcessingGuesses::kCurrentScoresHeader;
            
        for (auto const& player : context.players)
        {
            std::cout << std::format(ProcessingGuesses::kScoreLine, player.name, player.opponentMinesDetected, player.ownMinesDetected);
        }

        return { &stateCheckingNextTurn };
    }


    NextState stateCheckingNextTurn(GameContext& context)
    {
        std::cout << std::format(Results::kHeader, (context.round - 1));

        Players winners;
        Players eliminated;

        for (auto const& player : context.players)
        {
            unsigned int totalOpponentMines = Utils::countOpponentMines(player, context.players);

            std::cout << std::format(Results::kPlayerInformation, player.name, player.opponentMinesDetected, totalOpponentMines, player.remainingMines);

            if (player.opponentMinesDetected >= totalOpponentMines && totalOpponentMines > 0)
            {
                winners.push_back(player);
            }
            
            // Players who can't place more mines are removed

            if (player.remainingMines == 0 && player.ownMinesDetected > context.initialMines / 2)
            {
                eliminated.push_back(player);
            } 
        }

        context.players = Utils::getPlayersAfterElimination(context.players, eliminated);

        /*
            Game finishes if:
            - A player found all oponnent mines
            - All players were removed
            - The board has no more available positions
        */
    
        if (Utils::checkForWinners(winners) 
            || Utils::checkForElimination(context.players) 
            || Utils::checkBoardFull(context.width, context.height, context.board, context.players))
        {
            return { nullptr };
        }

        std::cout << std::format(Results::kProceedRound, context.round);
        
        return { &statePuttingMines };
    }
}