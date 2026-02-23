#include <raylib.h>

#ifndef BOID_H
#define BOID_H

struct Boid
{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float maxSpeed;
    float repelStrength;
    float alignmentStrength;
    float cohesionStrength;
    float repelRadius;
    float perceptionRadius;
    float boidRadius;
    int id;
    Color color;

    void moveBoid(const Boid flock[], int boidCount);
};

#endif