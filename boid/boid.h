#ifndef BOID_H
#define BOID_H

#include <raylib.h>
#include <raymath.h>
#include "../config.h"

struct BoidForce
{
    Vector2 acceleration = {0};
    Vector2 direction = {0};

    void ComputeForce(float iBoidsInRange, Vector2 currentVelocity, float scaleStrength)
    {
        this->direction *= iBoidsInRange;
        this->direction = Vector2Normalize(this->direction) * BOID_SPEED;
        this->acceleration = Vector2Subtract(this->direction, currentVelocity);
        this->acceleration *= scaleStrength;
    };
};

struct Boid
{
    Vector2 position;
    Vector2 velocity;
    int identifier;
    float size;
    Color color;

    void DrawBoid();
    void MoveBoid();
    void SteerBoid(const std::array<Boid, MAX_BOIDS> &flock, const SpatialGrid &worldGrid);
};
#endif