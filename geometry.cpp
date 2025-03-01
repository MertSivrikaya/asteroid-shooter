#include "geometry.h"

#include "raymath.h"
#include "iostream"

namespace Geometry
{
    void translatePoints(Vector2 *points, int numPoints, Vector2 translation)
    {
    // Update the position of each point by translation vector
    for (int i = 0; i < numPoints; i++)
        points[i] = Vector2Add(points[i], translation);
    }

    Vector2 clampShape(Vector2 *points, int numPoints, int minX, int maxX, int minY, int maxY)
    {
        float minXShape = points[0].x;
        float maxXShape = points[0].x;
        float minYShape = points[0].y;
        float maxYShape = points[0].y;

        // Finding the minX and maxX among the positions of the points
        for (int i = 1; i < numPoints; i++)
        {
            minXShape = std::min(minXShape, points[i].x);
            maxXShape = std::max(maxXShape, points[i].x);
            minYShape = std::min(minYShape, points[i].y);
            maxYShape = std::max(maxYShape, points[i].y);
        }

        float addX = 0;
        float addY = 0;

        // Find the x coordinate difference to snap the shape into given boundaries
        if (minXShape < minX) addX = minX - minXShape;                          // Shape is outside the left edge
        if (maxXShape > maxX) addX = -(maxXShape - maxX);                       // Shape is outside the right edge

        // Find the y coordinate difference to snap the shape into given boundaries
        if (minYShape < minY) addY = minY -minYShape;                           // Shape is outside the top edge
        if (maxYShape > maxY) addY = -(maxYShape - maxY);                       // Shape is outside the bottom edge

        Vector2 addVector = (Vector2) {addX, addY};

        // Clamp the position vectors so that the shape always stays in the screen
        if (addX != 0 || addY != 0)
        {
            for (int i = 0; i < numPoints; i++)
                points[i] = Vector2Add(points[i], addVector);
        }

        return addVector;
    }

    void rotatePoints(Vector2 *points, int numPoints, double angleOfRotation, Vector2 rotationOrigin)
    {
        // Apply rotation to each point
        for (int i = 0; i < numPoints; i++)
        {
            points[i] = Vector2Subtract(points[i], rotationOrigin);   // Translate to origin temporarily
            points[i] = Vector2Rotate(points[i], angleOfRotation);    // Rotate with respect to origin
            points[i] = Vector2Add(points[i], rotationOrigin);        // Translate back the rotated point
        }
    }

    bool isPointOffScreen(Vector2 point)
    {
        return !(point.x >= 0 && point.x <= GetScreenWidth() 
                && point.y >= 0 && point.y <= GetScreenHeight());
    }
    
    bool isShapeOffScreen(Vector2 *points, int numPoints)           // Determines whether the shape is completely out of the screen
    {
        for (int i = 0; i < numPoints; i++)
        {
            if (!isPointOffScreen(points[i]))
                return false;
        }

        return true;
    }
} 
