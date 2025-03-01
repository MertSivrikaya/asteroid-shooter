#include "entityList.h"
#include "entity.h"

#include "bullet.h"
#include "asteroid.h"
#include "player.h"

#include "vector"

#include "raymath.h"

#include <iostream>

void EntityList::updateEntities()
{
    Node *curr = head, *prev = nullptr;
    std::vector<Entity *> toDelete;

    while (curr)
    {
        curr->data->update();

        if (curr->data->shouldBeDestroyed)
        {
            toDelete.push_back(curr->data);
            curr->data = nullptr;

            if (!prev)
            {
                head = curr->next;
                delete curr;
                curr = head;
            }
            else
            {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
            }
        }
        
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }

    for (int i = 0; i < toDelete.size(); i++)
        delete toDelete[i];
}

void EntityList::drawEntities(Vector2 screenShakeOffset)
{
    Node *curr = head;
    while (curr)
    {
        if (curr->data)
            curr->data->draw(screenShakeOffset);
        else
            std::cout << "There is a problem!" << std::endl;
    
        curr = curr->next;
    }
}

void BulletList::checkCollisionsWithAsteroids(AsteroidList & asteroids, int playerDamage)
{
    Node *curr = head;
    while (curr)
    {
        if (!curr->data->shouldBeDestroyed) // if the game object is still active
        {
            Bullet * currBullet = static_cast<Bullet *>(curr->data);
            if (asteroids.checkCollisionWithBullet(*currBullet, playerDamage))
            {
                currBullet->shouldBeDestroyed = true;
            }
        }

        curr = curr->next;
    }
}

bool AsteroidList::checkCollisionWithBullet(const Bullet & bullet, int playerDamage)
{
    bool isBulletColliding = false;

    Node *curr = head;
    
    while (curr)
    {
        if (!curr->data->shouldBeDestroyed)
        {
            Asteroid * currAsteroid = static_cast<Asteroid *>(curr->data);
            if (CheckCollisionPointCircle(bullet.terminalPoint, currAsteroid->centerOfInternalTangentCircle, 
                                          currAsteroid->internalCircleRadius))
            {
                currAsteroid->takeDamage(playerDamage);
                isBulletColliding = true;
            }
        }
        
        curr = curr->next;
    }

    return isBulletColliding;
}

void AsteroidList::checkCollisionWithPlayer(Player & player)
{
    Node *curr = head;

    while (curr)
    {
        if (!curr->data->shouldBeDestroyed)
        {
            Asteroid * currAsteroid = static_cast<Asteroid *>(curr->data);
            if (CheckCollisionCircles(currAsteroid->centerOfInternalTangentCircle, currAsteroid->internalCircleRadius, 
                                      player.getCentroid(), player.hitboxCircleRadius))
            {
                currAsteroid->shouldBeDestroyed = true;
                if (!player.isInvincible())
                {
                    player.takeDamage(1);
                }
            }
        }

        curr = curr->next;
    }
}
