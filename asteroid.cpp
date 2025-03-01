#include "asteroid.h"

#include "geometry.h"
#include "game.h"

#include <iostream>

Asteroid::Asteroid(Game & gameInstance, Vector2 position, float radius, Vector2 directionVector, float speed)
    : centerOfInternalTangentCircle(position), internalCircleRadius(radius),
    flashAnimationTimer(flashAnimationDuration),
    directionVector(directionVector), speed(speed)
{
    float x0 = centerOfInternalTangentCircle.x;
    float y0 = centerOfInternalTangentCircle.y;

    float r = internalCircleRadius;

    verticePositions[0] = (Vector2) {x0 + r / sqrt(3), y0 + r};
    verticePositions[1] = (Vector2) {x0 + 2 * r / sqrt(3), y0};
    verticePositions[2] = (Vector2) {x0 + r / sqrt(3), y0 - r};
    verticePositions[3] = (Vector2) {x0 - r / sqrt(3), y0 - r};
    verticePositions[4] = (Vector2) {x0 - 2 * r / sqrt(3), y0};
    verticePositions[5] = (Vector2) {x0 - r / sqrt(3), y0 + r};

    velocityVector = Vector2Scale(directionVector, speed);

    // Health is proportional with the the radius
    health = minHealth + (maxHealth - minHealth) * Normalize(radius, minRadius, maxRadius);

    // Rotation speed is inversely proportional with the radius
    rotationSpeed = minRotationSpeed + (maxRotationSpeed - minRotationSpeed) * (1 - Normalize(radius, minRadius, maxRadius));

    if (velocityVector.x > 0)
        rotationSpeed *= -1;
}

void Asteroid::update()
{
    // std::cout << "X : " << centerOfInternalTangentCircle.x << "Y: " << centerOfInternalTangentCircle.y << std::endl;
    if (!shouldBeDestroyed)
    {
        if (!hasEnteredTheScreen)
        {
            if (!Geometry::isPointOffScreen(centerOfInternalTangentCircle))
                hasEnteredTheScreen = true;
        }


        move();
        rotate();

        // First check if the asteroid has entered the screen to prevent deletion immediately after instantiation
        if (hasEnteredTheScreen && Geometry::isShapeOffScreen(verticePositions, 6))
        {
            shouldBeDestroyed = true;
        }
    
        // if (isCollidingWithPlayer())
        //    gameInstance.getPlayer().getDamage();

        // Update the timers
        flashAnimationTimer.update();
    }   
}

void Asteroid::draw(Vector2 screenShakeOffset)
{
    Color currentColor = (flashAnimationTimer.active) ? flashColor : regularColor;

    for (int i = 0; i < 6; i++)
    {
        DrawLineEx(Vector2Add(verticePositions[i], screenShakeOffset), Vector2Add(verticePositions[(i + 1) % 6], screenShakeOffset), 3, currentColor);
    }
}

void Asteroid::move()
{
    Vector2 translateVector = Vector2Scale(velocityVector, GetFrameTime());

    Geometry::translatePoints(verticePositions, 6, translateVector);
    centerOfInternalTangentCircle = Vector2Add(centerOfInternalTangentCircle, translateVector);
}

void Asteroid::rotate()
{
    Geometry::rotatePoints(verticePositions, 6, rotationSpeed, centerOfInternalTangentCircle);
}

bool Asteroid::isCollidingWithPlayer()
{
    return CheckCollisionCircles(Game::getInstance().getPlayer().getCentroid(), Game::getInstance().getPlayer().hitboxCircleRadius, 
                        centerOfInternalTangentCircle, internalCircleRadius);
}

void Asteroid::takeDamage(float damage)
{   
    flashAnimationTimer.start();

    Game::getInstance().startScreenShake(hitScreenShakeDuration, hitScreenShakeMagnitude);

    health -= damage;

    if (health <= 0)
    {
        shouldBeDestroyed = true;
        Game::getInstance().score++;
    }
}