#ifndef BOID_H
#define BOID_H

#include <raylib.h>
#include <raymath.h>
#include "../config.h"

struct Boid
{
    Vector2 position;
    Vector2 velocity;
    int identifier;
    float size;
    Color color;

    void DrawBoid(Vector2 velocity, Vector2 position);
};
#endif