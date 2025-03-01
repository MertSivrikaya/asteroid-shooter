#include "player.h"
#include "game.h"
#include "bullet.h"
#include "geometry.h"

Player::Player() : shootCooldownDuration(0.2f), shootCooldownTimer(shootCooldownDuration), invincibilityTimer(invincibilityDuration)
{
    reset();
}

void Player::update()
{
    getPlayerInput();
    
    if (Vector2Length(directionVector) != 0)
        move();

    rotate();

    if (isThereShootInput && !shootCooldownTimer.active)
        shoot();

    // Update the timers
    shootCooldownTimer.update();
    invincibilityTimer.update();
}

void Player::draw(Vector2 screenShakeOffset)
{
    if (isInvincible())
    {
        playBlinkAnimation(screenShakeOffset);
    }
    else
    {
        DrawTriangle(Vector2Add(verticePositions[0], screenShakeOffset), Vector2Add(verticePositions[1], screenShakeOffset), 
                                Vector2Add(verticePositions[2], screenShakeOffset), WHITE);
    }
}

void Player::reset()
{
    maxHealth = 3;
    health = maxHealth;

    shootCooldownDuration = 0.2f;
    isThereShootInput = false;

    hitboxCircleRadius = 10;

    verticePositions[0] = (Vector2) {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2 - 2 * hitboxCircleRadius};
    verticePositions[1] = (Vector2) {(float) GetScreenWidth() / 2 - hitboxCircleRadius * sqrt(3), (float) GetScreenHeight() / 2 + hitboxCircleRadius};
    verticePositions[2] = (Vector2) {(float) GetScreenWidth() / 2 + hitboxCircleRadius * sqrt(3), (float) GetScreenHeight() / 2 + hitboxCircleRadius};

    // Reset the timers
    shootCooldownTimer.reset();
    invincibilityTimer.reset();
}

void Player::getPlayerInput()
{
    int xDirection = 0;
    int yDirection = 0;

    if (IsKeyDown(KEY_A))
        xDirection = -1;
    if (IsKeyDown(KEY_D))
        xDirection = 1;

    if (IsKeyDown(KEY_W))
        yDirection = -1;
    if (IsKeyDown(KEY_S))
        yDirection = 1;

    directionVector = Vector2Normalize((Vector2) {(float) xDirection, (float) yDirection});

    if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        isThereShootInput = true;
    else
        isThereShootInput = false;
}

void Player::move()
{
    // Determine the velocity vector
    Vector2 velocity = Vector2Scale(directionVector, speed * GetFrameTime());

    // Move the player
    Geometry::translatePoints(verticePositions, 3, velocity);

    // Clamp the player onto the display
    Geometry::clampShape(verticePositions, 3, 0, GetScreenWidth(), 0, GetScreenHeight());
}

void Player::rotate()
{
    // Determine the position of the center of the internal tangent circle of the triangle
    Vector2 centerOfInternalTangentCirclePos = getCentroid();

    // Calculate the direction vector from the center of inner tangent circle to the first vertex
    Vector2 currentDirectionVector = Vector2Subtract(verticePositions[0], centerOfInternalTangentCirclePos);
    
    // Calculate the direction vector from the center of inner tangent circle to the mouse position
    Vector2 mousePosVector = Vector2Subtract(GetMousePosition(), centerOfInternalTangentCirclePos);

    // Determine the angle from the currentDirectionVector to mousePosVector
    double angleOfRotation = Vector2Angle(currentDirectionVector, mousePosVector);

    // Apply rotation to all vertices
    Geometry::rotatePoints(verticePositions, 3, angleOfRotation, centerOfInternalTangentCirclePos);
}

void Player::shoot()
{
    shootCooldownTimer.start(); // Start the shoot cooldown

    Vector2 initialPoint;
    Vector2 terminalPoint;

    // Determine the position of the center of the internal tangent circle of the triangle
    Vector2 centerOfInternalTangentCirclePos = getCentroid();
    
    initialPoint.x = bulletInstantiationDistance * ((verticePositions[0].x - centerOfInternalTangentCirclePos.x) / (2 * hitboxCircleRadius)) + verticePositions[0].x;
    initialPoint.y = bulletInstantiationDistance * ((verticePositions[0].y - centerOfInternalTangentCirclePos.y) / (2 * hitboxCircleRadius)) + verticePositions[0].y;

    terminalPoint.x = Bullet::bulletLength * ((verticePositions[0].x - centerOfInternalTangentCirclePos.x) / (2 * hitboxCircleRadius)) + initialPoint.x;
    terminalPoint.y = Bullet::bulletLength * ((verticePositions[0].y - centerOfInternalTangentCirclePos.y) / (2 * hitboxCircleRadius)) + initialPoint.y;

    Game::getInstance().instantiateBullet(initialPoint, terminalPoint);
}

Vector2 Player::getCentroid()
{
    Vector2 centerOfInternalTangentCirclePos = (Vector2) { 0, 0 };
    for (int i = 0; i < 3; i++)
    {
        centerOfInternalTangentCirclePos.x += verticePositions[i].x;
        centerOfInternalTangentCirclePos.y += verticePositions[i].y;
    }
    
    centerOfInternalTangentCirclePos.x /= 3;
    centerOfInternalTangentCirclePos.y /= 3;

    return centerOfInternalTangentCirclePos;
}

bool Player::isInvincible()
{
    return invincibilityTimer.active;
}

void Player::takeDamage(int damage)
{
    invincibilityTimer.start();

    startBlinkAnimation();

    Game::getInstance().startScreenShake(hitScreenShakeDuration, hitScreenShakeMagnitude);

    health -= damage;

    if (health <= 0)
        die();
}

void Player::die()
{
    Game::getInstance().endGame();
}

void Player::startBlinkAnimation()
{
    blinkAnimationCounter = 0;
    currentBlinkColor = 0; // BLACK
}

void Player::playBlinkAnimation(Vector2 screenShakeOffset)
{
    if (currentBlinkColor == 1) // White
        DrawTriangle(Vector2Add(verticePositions[0], screenShakeOffset), Vector2Add(verticePositions[1], screenShakeOffset), 
                                Vector2Add(verticePositions[2], screenShakeOffset), WHITE);

    blinkAnimationCounter += GetFrameTime();
    if (blinkAnimationCounter >= 0.4)
    {
        blinkAnimationCounter = 0;
        currentBlinkColor = (currentBlinkColor + 1) % 2;
    }
}