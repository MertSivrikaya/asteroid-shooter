#ifndef _GAME_
#define _GAME_

#include <iostream>

#include "entityList.h"

#include "entity.h"
#include "player.h"

class Asteroid;

class Game
{
    /*
        A singleton class to initialize, update and end the game process.
    */
    public:
        Game(const Game & rhs) = delete;                // Deleting the copy constructor of the class so that compiler will not provide a default copy constructor.
        Game & operator=(const Game & rhs) = delete;    // Deleting assignement operator overloading of the class so that compiler will not provide a default one.
        
        static Game & getInstance();                    // This method must be static as it will be called directly from the class

        void initGame();                                // Method to initialize the game variables and load the assets
        void updateGame();                              // Method to update the current frame
        void drawFrame();                               // Method to draw the current frame on the display
        void updateDrawFrame();                         // Method to update and draw the current frame
        void unloadGame();                              // Method to deallocate the game variables and unload assets
        
        void instantiateBullet(Vector2 initialPoint, Vector2 terminalPoint);

        void instantiateAsteroid(Vector2 position, float radius, Vector2 directionVector, float speed);
        void instantiateRandomAsteroid();

        void updateAsteroidWave();

        void startScreenShake(double duration, float magnitude);
        Vector2 getScreenShakeOffset();

        void updateDifficulty();

        void startGame();
        void unPauseGame();
        void pauseGame();
        void endGame();

        Timer screenShakeTimer;
        float screenShakeMagnitude;

        Player & getPlayer();

        enum GameState                                  // An enum type definition to determine the current state of the game
        {
            START,
            PAUSE,
            DEATHSCREEN,
            RUNNING
        };

        AsteroidList asteroids;
        BulletList bullets;

        int score = 0;
        int highScore = 0;

        double increaseDifficultyPeriodBase = 15.0;     // Increase the difficulty every 20 seconds
        Timer difficultyTimer;
        int difficulty = 0;
        int maxDifficulty = 7;

    private:
        const int screenWidth = 800;
        const int screenHeight = 800;

        const char * gameName = "Space Shooter";    

        const int targetFPS = 60;

        GameState gameState;

        Player player;

        Timer asteroidSpawnCooldownTimer;

        Game();                                      // Making the default constructor private to prevent object construction.
};


#endif