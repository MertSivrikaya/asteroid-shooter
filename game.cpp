#include "game.h"
#include "asteroid.h"
#include "bullet.h"

#include "UIManager.h"

Game::Game() : player()
{
    gameState = START;
}

Game & Game::getInstance()
{
    static Game instance;                               // The private Default constructor will be called without a problem as we are in the scope of the class
                                                        // This is a static local variable whose lifetime extends to the end of the program and this variable will 
                                                        // only be allocated in data segment when the function is called for the first time
    
    return instance;                                    // If this function is called again, it will return the already created instance in the data segment                    
}

void Game::initGame()

{
    InitWindow(screenWidth, screenHeight, gameName);
    
    if (!IsWindowReady())                               // Check if the window was created successfully
    {
        std::cerr << "Failed to initialize window!" << std::endl;
        exit(EXIT_FAILURE);                             // Return EXIT_FAILURE (1) to the operation system
    }
    
    SetTargetFPS(targetFPS);

    // Disable closing the game with "Escape" key as it will be used for pausing the game
    SetExitKey(0);
}

void Game::updateGame()
{
    
    switch (gameState)
    {
        case START:
            if (IsKeyPressed(KEY_SPACE))
                startGame();

            break;

        case RUNNING:
            // Update entities
            player.update();
            asteroids.updateEntities();
            bullets.updateEntities();

            // Update the difficulty of the game (Must be called before updateAsteroidWave() function)
            updateDifficulty();

            // Instantiate random asteroids to continue the current wave
            updateAsteroidWave();

            // Collision detection
            bullets.checkCollisionsWithAsteroids(asteroids, player.damage);
            asteroids.checkCollisionWithPlayer(player);

            // Update the timers
            asteroidSpawnCooldownTimer.update();
            screenShakeTimer.update();
            difficultyTimer.update();

            // Get User input to pause to game
            if (IsKeyPressed(KEY_ESCAPE))
                pauseGame();

            // For Debug purposes
            if (IsKeyPressed(KEY_K))
                instantiateRandomAsteroid();

            break;
        
        case PAUSE:
            if (IsKeyPressed(KEY_ESCAPE))
                unPauseGame();

            break;

        case DEATHSCREEN:
            if (IsKeyPressed(KEY_SPACE))
                startGame();

            break;
    }
}

void Game::drawFrame()
{
    BeginDrawing();
        ClearBackground(BLACK);                         // Clear the screen with a black background color to delete the drawings from the previous frame
    
        switch (gameState)
        {
            case START:
                UIManager::getInstance().drawStartScreen();

                break;

            case RUNNING:
                // Draw entities
                player.draw(getScreenShakeOffset());
                asteroids.drawEntities(getScreenShakeOffset());
                bullets.drawEntities(getScreenShakeOffset());

                // Draw UI elements
                UIManager::getInstance().drawPlayerHealth();
                UIManager::getInstance().drawPlayerScore();
                UIManager::getInstance().drawPlayerCrosshair();
                
                break;
            
            case PAUSE:
                UIManager::getInstance().drawPauseScreen();

                break;

            case DEATHSCREEN:
                UIManager::getInstance().drawEndGameScreen();

                break;
        }

    EndDrawing();
}

void Game::updateDrawFrame()
{
    updateGame();
    drawFrame();
}

void Game::unloadGame()
{
    CloseWindow();
    // Default destructors of the linked list classes will handle the asteroids and bullets objects (nodes and data)
}

Player & Game::getPlayer()
{
    return player;
}

void Game::instantiateBullet(Vector2 initialPoint, Vector2 terminalPoint)
{
    bullets.addNode(new Bullet(*this, initialPoint, terminalPoint));
}

void Game::instantiateAsteroid(Vector2 position, float radius, Vector2 directionVector, float speed)
{
    asteroids.addNode(new Asteroid(*this, position, radius, directionVector, speed));
}

void Game::instantiateRandomAsteroid()
{
    // Define spawn margin beyond the screen
    float margin = 50.0f;
    int side = rand() % 4; // 0 = top, 1 = right, 2 = bottom, 3 = left
    
    Vector2 position;
    switch (side)
    {
        case 0: position = {static_cast<float>(rand() % screenWidth), -margin}; break;       // Top
        case 1: position = {screenWidth + margin, static_cast<float>(rand() % screenHeight)}; break; // Right
        case 2: position = {static_cast<float>(rand() % screenWidth), screenHeight + margin}; break; // Bottom
        case 3: position = {-margin, static_cast<float>(rand() % screenHeight)}; break;      // Left
    }

    // There is a %20 chance that instantiated asteroid will directly target the player's last position instead of the middle
    Vector2 target;

    if (rand() % 100 < 20)
    {
        target = player.getCentroid();
        // std::cout << "Going to player" << std::endl;
    }
    else
    {
         // Random variation near the center
        float centerVariation = 100.0f;
        target = {screenWidth / 2.0f + (rand() % (int)(2 * centerVariation) - centerVariation),
                  screenHeight / 2.0f + (rand() % (int)(2 * centerVariation) - centerVariation)};
    }
    
    // Calculate direction towards the target
    Vector2 directionVector = Vector2Normalize(Vector2Subtract(target, position));
    
    // Random speed
    int minSpeed = Asteroid::minSpeed;
    int maxSpeed = Asteroid::maxSpeed;

    float speed = static_cast<float>(minSpeed) + static_cast<float>(rand() % (maxSpeed - minSpeed)); // Speed between minSpeed and maxSpeed
    
    // Random radius
    int minRadius = Asteroid::minRadius;
    int maxRadius = Asteroid::maxRadius;

    float radius = static_cast<float>(minRadius) + static_cast<float>(rand() % maxRadius); // Radius between 10 and 30
    
    instantiateAsteroid(position, radius, directionVector, speed);
}

void Game::updateAsteroidWave()
{
    if (asteroidSpawnCooldownTimer.active)
        return;

    // float minCoolDownDuration = 0.3f;
    // float maxCoolDownDuration = 1.0f;

    double minCoolDownDuration;
    double maxCoolDownDuration;

    int spawnCount;

    /*
    
    Stage  MinCoolDown  MaxCoolDown  SpawnCount
    1           1.1         1.8          1
    2           0.8         1.5          1
    3           0.5         1.2          1
    4           0.3         1.0          1
    5           0.2         0.9          1
    6           0.15        0.8          1
    7           0.15        0.8          2
    
    */

   switch (difficulty)
   {
        case 1:
            minCoolDownDuration = 1.1;
            maxCoolDownDuration = 1.8;
            spawnCount = 1;

            break;

        case 2:
            minCoolDownDuration = 0.8;
            maxCoolDownDuration = 1.5;
            spawnCount = 1;

            break;

        case 3:
            minCoolDownDuration = 0.5;
            maxCoolDownDuration = 1.2;
            spawnCount = 1;

            break;

        case 4:
            minCoolDownDuration = 0.3;
            maxCoolDownDuration = 1.0;
            spawnCount = 1;

            break;

        case 5:
            minCoolDownDuration = 0.2;
            maxCoolDownDuration = 0.9;
            spawnCount = 1;

            break;

        case 6:
            minCoolDownDuration = 0.15;
            maxCoolDownDuration = 0.8;
            spawnCount = 1;

            break;

        case 7:
            minCoolDownDuration = 0.15;
            maxCoolDownDuration = 0.8;
            spawnCount = 2;

            break;
   }

    // rand() / RAND_MAX gives a random float between 0 and 1
    double randomCooldown = minCoolDownDuration + ((maxCoolDownDuration - minCoolDownDuration) * (rand() / (float) RAND_MAX));
    
    asteroidSpawnCooldownTimer = Timer(randomCooldown);
    asteroidSpawnCooldownTimer.start();

    for (int i = 0; i < spawnCount; i++)
        instantiateRandomAsteroid();
}

void Game::startScreenShake(double duration, float magnitude)
{
    screenShakeTimer = Timer(duration);
    screenShakeTimer.start();

    screenShakeMagnitude = magnitude;
}

Vector2 Game::getScreenShakeOffset()
{
    if (!screenShakeTimer.active)
        return Vector2Zero();

    int screenShakeOffsetX = (rand() % 9) - 4;
    int screenShakeOffsetY = (rand() % 9) - 4; 

    Vector2 screenShakeOffset = (Vector2) {screenShakeOffsetX, screenShakeOffsetY};

    if (Vector2Length(screenShakeOffset) > 0)
        screenShakeOffset = Vector2Normalize(screenShakeOffset);

    float screenShakeMultiplier = (screenShakeMagnitude - 0.5) + (rand() / (float) RAND_MAX) * (1.5);

    screenShakeOffset = Vector2Scale(screenShakeOffset, screenShakeMultiplier);

    return screenShakeOffset;
}

void Game::updateDifficulty()
{
    if (difficulty == maxDifficulty)
        return;

    if (difficultyTimer.active)
        return;

    difficultyTimer = Timer(increaseDifficultyPeriodBase + difficulty * 6.0);
    difficultyTimer.start();

    difficulty++;
}

void Game::endGame()
{
    gameState = DEATHSCREEN;

    // Update the highscore
    highScore = (score > highScore) ? score : highScore;
}

void Game::pauseGame()
{
    gameState = PAUSE;
}

void Game::unPauseGame()
{
    gameState = RUNNING;
}

void Game::startGame()
{
    gameState = RUNNING;

    // Remove all existing entites except player
    bullets.removeAll();
    asteroids.removeAll();

    // Reset the player
    player.reset();

    // Reset the timers
    screenShakeTimer.reset();
    asteroidSpawnCooldownTimer.reset();
    difficultyTimer.reset();

    // Reset the score
    score = 0;

    // Reset the difficulty
    difficulty = 0;
}