#ifndef BOID_H
#define BOID_H

#include <raylib.h>
#include "../config.h"
#include "../whoid/whoid.h"

struct Boid
{
    Vector2 position;
    Vector2 velocity;
    int identifier;
    int size;
    Color color;
    bool isAlive;

    void DrawBoid();
    void MoveBoid();
    void SteerBoid(const std::array<Boid, MAX_BOIDS> &flock, const std::array<Whoid, MAX_WHOIDS> &whoidGroup, const SpatialGrid &worldGrid);
};

#endif