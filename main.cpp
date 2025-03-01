#include <iostream>
#include "game.h"

int main()
{
    Game & game = Game::getInstance();

    game.initGame();                                    // Initialize the game variables and load the assets

    while (!WindowShouldClose())                        // Game Loop
    {
        game.updateDrawFrame();
    }

    game.unloadGame();                                  // Deallocate the game variables and unload the assets

    return 0;                                           // Program completed successfully, return EXIT_SUCCESS (0) to the OS
}