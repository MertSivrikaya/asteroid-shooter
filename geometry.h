#ifndef _GEOMETRY_
#define _GEOMETRY_

#include "raylib.h"

// Utility functions related to geometry
namespace Geometry 
{
    void translatePoints(Vector2 *points, int numPoints, Vector2 translation);
    Vector2 clampShape(Vector2 *points, int numPoints, int minX, int maxX, int minY, int maxY);                
    
    void rotatePoints(Vector2 *points, int numPoints, double angleOfRotation, Vector2 rotationOrigin);

    bool isPointOffScreen(Vector2 point);
    bool isShapeOffScreen(Vector2 *points, int numPoints); // Determines whether the shape is completely out of the screen
}

#endif