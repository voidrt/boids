#include <raylib.h>

#ifndef BOID_H
#define BOID_H

struct Boid
{
    Vector2 position;
    Vector2 velocity;
    float acceleration;
    float rotation;
    int maxSpeed;
    int steerStrenght;

    void moveBoid();
};

#endif