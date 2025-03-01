#ifndef _ENTITY_LIST_
#define _ENTITY_LIST_

#include "linkedList.h"

#include "raylib.h"

// Forward declare class entity
class Entity;
class Asteroid;
class Bullet;
class Player;
class AsteroidList;

class EntityList : public LinkedList<Entity>
{
    public:
        void updateEntities();
        void drawEntities(Vector2 screenShakeOffset);

};

class BulletList : public EntityList
{
    public:
        void checkCollisionsWithAsteroids(AsteroidList & asteroid, int playerDamage);
};

class AsteroidList : public EntityList
{
    public:
        bool checkCollisionWithBullet(const Bullet & bullet, int playerDamage);
        void checkCollisionWithPlayer(Player & player);
};


#endif