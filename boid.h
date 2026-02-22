#include <raylib.h>

#ifndef BOID_H
#define BOID_H

struct Boid
{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float maxSpeed;
    float steerStrenght;
    float repelStrength;
    float repelRadius;
    float boidRadius;
    int id;

    void moveBoid(const Boid flock[], int boidCount);
};

#endif