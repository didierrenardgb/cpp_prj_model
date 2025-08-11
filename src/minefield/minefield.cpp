#include <minefield/minefield.h>
#include <minefield/setup_game.h>

void runMainLoop()
{
    bool quit = false;
    GameContext context;
    context.currentState = {&setupGame};
    while (!quit)
    {
        if (context.currentState.updateFunction != nullptr)
        {
            context.currentState = (*context.currentState.updateFunction)(context);
        }
        quit = context.currentState.updateFunction == nullptr;
    }
}

int main(int argc, const char *argv[])
{
    runMainLoop();

    return 0;
}
