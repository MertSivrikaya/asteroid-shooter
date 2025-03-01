#include "bullet.h"
#include "game.h"

Bullet::Bullet(Game & gameInstance, Vector2 initialPoint, Vector2 terminalPoint) 
: initialPoint(initialPoint), terminalPoint(terminalPoint)
{
    Vector2 directionVector = Vector2Normalize(Vector2Subtract(terminalPoint, initialPoint));
    velocityVector = Vector2Scale(directionVector, bulletSpeed);
}

void Bullet::update()
{
    initialPoint = Vector2Add(initialPoint, Vector2Scale(velocityVector, GetFrameTime()));
    terminalPoint = Vector2Add(terminalPoint, Vector2Scale(velocityVector, GetFrameTime()));

    // Destroy the bullet if it is off screen and remove it from the allEntities list of the game object
    if (initialPoint.x < 0 || initialPoint.x > GetScreenWidth() || initialPoint.y < 0 || initialPoint.y > GetScreenHeight())
        shouldBeDestroyed = true;
}

void Bullet::draw(Vector2 screenShakeOffset)
{
    DrawLineEx(Vector2Add(initialPoint, screenShakeOffset), Vector2Add(terminalPoint, screenShakeOffset), 3, RED);
}