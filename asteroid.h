#ifndef _ASTEROID_
#define _ASTEROID_

#include "raylib.h"
#include "raymath.h"

#include "entity.h"
#include "timer.h"

class Game;

class Asteroid : public Entity
{
    public:
        Asteroid(Game & gameInstance, Vector2 position, float radius, Vector2 directionVector, float speed);
        
        void update() override;
        void draw(Vector2 screenShakeOffset) override;

        void move();
        void rotate();

        bool isCollidingWithPlayer();

        void takeDamage(float damage);

        static constexpr int minSpeed = 200;
        static constexpr int maxSpeed = 250;

        static constexpr int minRadius = 10;
        static constexpr int maxRadius = 50;

        static constexpr int minHealth = 20;
        static constexpr int maxHealth = 120;

        float internalCircleRadius;
        Vector2 centerOfInternalTangentCircle;          // Position to instantiate

        static constexpr Color flashColor = WHITE;
        static constexpr Color regularColor = (Color) {255, 255, 255, 127};

    private:
        Vector2 verticePositions[6];

        int health;

        static constexpr double flashAnimationDuration = 0.1;
        Timer flashAnimationTimer;

        static constexpr double hitScreenShakeDuration = 0.1;
        static constexpr float hitScreenShakeMagnitude = 2.0f;

        Vector2 directionVector;
        float speed;
        
        Vector2 velocityVector;

        const float minRotationSpeed = 0.001;              // radians per frame
        const float maxRotationSpeed = 0.1;

        float rotationSpeed;

        bool hasEnteredTheScreen = false;
};

#endif