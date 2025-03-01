#ifndef _PLAYER_
#define _PLAYER_

#include "entity.h"

#include "raylib.h"
#include "raymath.h"
#include "timer.h"

class Player : public Entity
{
    public:
        Player();

        void update() override;
        void draw(Vector2 screenShakeOffset) override;

        void reset();

        void getPlayerInput();
        void move();
        void rotate();
        void shoot();

        bool isInvincible();

        void takeDamage(int damage = 1);
        void die();

        Vector2 getCentroid();                  // Determine the position of the center of the internal tangent circle of the triangle
    
        int hitboxCircleRadius;                 // Radius of the hitbox circle of the player
        int damage = 30;

        int health;                             // Current health of the player

        Vector2 verticePositions[3];            // Array of positions of the vertices of the player triangle

    private:
        void startBlinkAnimation();
        void playBlinkAnimation(Vector2 screenShakeOffset);

        float blinkAnimationCounter = 0.0f;
        int currentBlinkColor = 0;

        int maxHealth;                          // Maximum health of the player

        int speed = 200;                        // Move speed of the player in any direction

        Vector2 directionVector;                // Direction vector determining moving direction formed by the player input
        bool isThereShootInput;                 // Boolean to determine whether shoot input exist
    
        double shootCooldownDuration;           // Shoot cooldown duration in seconds
        Timer shootCooldownTimer;               // Timer to determine whether the player is on shooting cooldown
        float bulletInstantiationDistance = 3;
        
        static constexpr float invincibilityDuration = 2.0f;
        Timer invincibilityTimer;

        static constexpr double hitScreenShakeDuration = 0.7;
        static constexpr float hitScreenShakeMagnitude = 6.0f;
};

#endif