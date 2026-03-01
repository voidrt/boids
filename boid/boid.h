#include <raylib.h>
#include <array>
#include "../config.h"

#ifndef BOID_H
#define BOID_H

struct Boid
{
    bool isAlive = true;

    Vector2 position;
    Vector2 velocity;
    int size;
    int identifier;
    Color color;

    void UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock);
    void UpdatePosition();
};

#endif