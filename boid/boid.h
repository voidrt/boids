#include <raylib.h>
#include "../squoid/squoid.h"

#ifndef BOID_H
#define BOID_H

struct Boid
{
    bool isAlive = true;

    Vector2 position;
    Vector2 velocity;
    float rotation;
    float maxSpeed;
    float squoidSeparationStrength;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float repelRadius;
    float perceptionRadius;
    float boidSize;
    int id;
    Color color;

    void SteerBoid(const Boid flock[], int boidCount, const Squoid squoids[], int squoidCount);
};

#endif