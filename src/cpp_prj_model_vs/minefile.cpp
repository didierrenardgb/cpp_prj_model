#include <cpp_prj_model_vs/game_states.h>
#include <cpp_prj_model_vs/types.h>
#include <cpp_prj_model_vs/utils.h>

#include <iostream>

void runMainLoop()
{
    bool quit = false;
    GameContext context;
    context.currentState = { &GameStates::stateMainMenuUpdate };
    while (!quit)
    {
        if (context.currentState.updateFunction != nullptr)
        {
            context.currentState = (*context.currentState.updateFunction)(context);
        }
        quit = context.currentState.updateFunction == nullptr;
    }
}

void initializeRandomNumberGenerator()
{
    srand(static_cast<unsigned int>(time(0)));
}

int main()
{
    initializeRandomNumberGenerator();
    runMainLoop();
    return 0;
}