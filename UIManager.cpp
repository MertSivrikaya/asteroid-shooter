#include "UIManager.h"

#include "game.h"
#include "player.h"
#include "raylib.h"

UIManager::UIManager() {}

UIManager & UIManager::getInstance()
{
    static UIManager instance;                               
                                                
    return instance;                                    
}

void UIManager::drawStartScreen()
{
    int textWidth;
    static int frameCounter = 0;

    frameCounter++;

    textWidth = MeasureText("Space Shooter", 50);

    DrawText("Space Shooter", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 , 50, WHITE);

    textWidth = MeasureText("Designed by Mert Sivrikaya-", 20);

    DrawText("-Designed by Mert Sivrikaya-", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 + 30 , 20, WHITE);

    // Blink animation
    if (frameCounter <= 45)
    {
        textWidth = MeasureText("Press 'Space' to Play.", 20);
        DrawText("Press 'Space' to Play.", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 + 300, 20, WHITE);    
    }
    else if (frameCounter >= 90)
    {
        frameCounter = 0;
    }
}

void UIManager::drawPauseScreen()
{
    int textWidth;

    static int frameCounter = 0;

    frameCounter++;

    textWidth = MeasureText("Game Paused", 30);

    DrawText("Game Paused", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 , 30, WHITE);

    if (frameCounter <= 45)
    {
        textWidth = MeasureText("Press 'Esc' to Continue", 20);
        DrawText("Press 'Esc' to Continue", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 + 150, 20, WHITE);    
    }
    else if (frameCounter >= 90)
    {
        frameCounter = 0;
    }
}

void UIManager::drawEndGameScreen()
{
    char buffer[40];
    int textWidth;

    static int frameCounter = 0;

    frameCounter++;

    textWidth = MeasureText("Game Over!", 50);

    DrawText("Game Over!", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 - 100, 50, WHITE);

    snprintf(buffer, 40, "Score : %d", Game::getInstance().score);
    textWidth = MeasureText(buffer, 30);

    DrawText(buffer, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 , 30, WHITE);

    snprintf(buffer, 40, "High Score : %d", Game::getInstance().highScore);
    textWidth = MeasureText(buffer, 30);

    DrawText(buffer, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2  + 100, 30, WHITE);

    if (frameCounter <= 45)
    {
        textWidth = MeasureText("Press 'Space' to Play Again.", 20);
        DrawText("Press 'Space' to Play Again.", GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - textWidth / 2 + 300, 20, WHITE);    
    }
    else if (frameCounter >= 90)
    {
        frameCounter = 0;
    }
}

void UIManager::drawPlayerHealth()
{
    int playerHealth = Game::getInstance().getPlayer().health;

    int r = 8;
    Vector2 verticePositions[3];

    float initialLeftMargin = 18.0f;
    float topMargin = 18.0f;

    for (int i = 0; i < playerHealth; i++)
    {
        float leftMargin = initialLeftMargin + 5 * r * i;

        verticePositions[0] = (Vector2) {leftMargin, topMargin - 2 * r};
        verticePositions[1] = (Vector2) {leftMargin - r * sqrt(3), topMargin + r};
        verticePositions[2] = (Vector2) {leftMargin + r * sqrt(3), topMargin + r};

        DrawTriangleLines(verticePositions[0], verticePositions[1], verticePositions[2], WHITE);
        DrawTriangle(verticePositions[0], verticePositions[1], verticePositions[2], RED);
    }
}

void UIManager::drawPlayerScore()
{
    int playerScore = Game::getInstance().score;

    char buffer[40];
    int textWidth;

    snprintf(buffer, 40, "Score : %d", playerScore);
    textWidth = MeasureText(buffer, 30);

    DrawText(buffer, GetScreenWidth() / 2 - textWidth / 2, 18, 30, WHITE);
}

void UIManager::drawPlayerCrosshair()
{
    Vector2 pos;

    // Determine the position of the center of the internal tangent circle of the triangle
    Player & player = Game::getInstance().getPlayer();
    Vector2 centerOfInternalTangentCirclePos = player.getCentroid();

    Vector2 targetVertex = player.verticePositions[0];
    float offset = 50;

    pos.x = offset * ((targetVertex.x - centerOfInternalTangentCirclePos.x) / (2 * player.hitboxCircleRadius)) + targetVertex.x;
    pos.y = offset * ((targetVertex.y - centerOfInternalTangentCirclePos.y) / (2 * player.hitboxCircleRadius)) + targetVertex.y;

    DrawCircle(pos.x, pos.y, 3, RED);
}