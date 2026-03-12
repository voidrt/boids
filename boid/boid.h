#ifndef BOID_H
#define BOID_H

#include <raylib.h>
#include "../config.h"
struct Boid
{
    Vector2 position;
    Vector2 velocity;
    int identifier;
    int size;
    Color color;

    void UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock, const SpatialGrid &worldGrid);
    void UpdatePosition();
};

#endif