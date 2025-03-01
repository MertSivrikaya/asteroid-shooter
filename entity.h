#ifndef _ENTITY_
#define _ENTITY_

#include "raylib.h"

class Entity
{
    public:
        virtual void update() = 0;
        virtual void draw(Vector2 screenShakeOffset) = 0;

        bool shouldBeDestroyed = false; 
};

#endif