#ifndef _BULLET_
#define _BULLET_

#include "entity.h"

#include "raylib.h"
#include "raymath.h"

// Forward declaration of Game to break circular dependency
class Game;

class Bullet : public Entity
{
    public:
        Bullet(Game & gameInstance, Vector2 initialPoint, Vector2 terminalPoint);

        void update() override;
        void draw(Vector2 screenShakeOffset) override;

        static constexpr int bulletLength = 10;

        Vector2 initialPoint;
        Vector2 terminalPoint;

    private:
        static constexpr int bulletSpeed = 800;

        Vector2 velocityVector;
};


#endif