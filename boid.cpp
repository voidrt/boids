#include "boid.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>

void Boid::moveBoid(const Boid flock[], int boidCount)
{
    float boidsInRange;
    Vector2 repelVelocity = (Vector2){0.0f, 0.0f};
    Vector2 cohesionForce = (Vector2){0.0f, 0.0f};
    Vector2 alignmentForce = (Vector2){0.0f, 0.0f};

    for (int i = 0; i < boidCount; ++i)
    {
        Boid otherBoid = flock[i];
        if (this->id == otherBoid.id)
            continue;
        float distanceToOtherBoid = Vector2Distance(otherBoid.position, this->position);

        if (distanceToOtherBoid < this->repelRadius && distanceToOtherBoid > 0.01f)
        {
            Vector2 repelDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));
            Vector2 weightedVelocity = (Vector2){repelDirection.x / distanceToOtherBoid, repelDirection.y / distanceToOtherBoid};

            repelVelocity = Vector2Add(repelVelocity, weightedVelocity);

            ++boidsInRange;
        }
    }

    if (boidsInRange > 0)
    {
        repelVelocity = Vector2Scale(repelVelocity, 1 / boidsInRange);
        repelVelocity = Vector2Scale(repelVelocity, repelStrength);
        this->velocity =
            Vector2ClampValue(Vector2Add(velocity, repelVelocity), maxSpeed / 2, maxSpeed);
    }
    this->position = Vector2Add(this->position, this->velocity);
}