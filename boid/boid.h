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
    float size;
    int id;
    float perceptionRadius;
    float rotation;
    Color color;
    float maxSpeed = BOID_SPEED;
    float squoidSeparationStrength = BOID_TO_SQUOID_SEPARATION_STRENGTH;
    float separationStrength = BOID_SEPARATION_STRENGTH;
    float alignmentStrength = BOID_ALIGNMENT_STRENGTH;
    float cohesionStrength = BOID_COHESION_STRENGTH;
    float repelRadius = BOID_SEPARATION_RADIUS;

    void UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock);
    void UpdatePosition();
};

#endif