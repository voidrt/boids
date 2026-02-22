#include "boid.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>

void Boid::moveBoid(const Boid flock[], int boidCount)
{
    float boidsInRange;
    Vector2 repelVelocity = (Vector2){0.0f, 0.0f};

    for (int i = 0; i < boidCount; ++i)
    {
        Boid otherBoid = flock[i];
        if (this->id == otherBoid.id)
            continue;
        float distanceToOtherBoid = Vector2Length(Vector2Subtract(otherBoid.position, this->position));

        if (distanceToOtherBoid < this->repelRadius && distanceToOtherBoid > 0.1f)
        {
            Vector2 repelDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));
            Vector2 weightedVelocity = (Vector2){repelDirection.x / distanceToOtherBoid, repelDirection.y / distanceToOtherBoid};

            repelVelocity = Vector2Scale(Vector2Add(repelVelocity, weightedVelocity), repelStrength);

            ++boidsInRange;
        }
    }

    if (boidsInRange > 0)
    {
        repelVelocity = (Vector2) {repelVelocity.x / boidsInRange, repelVelocity.y/boidsInRange};
        this->velocity = Vector2Scale(
            Vector2Normalize(
                Vector2Add(velocity, repelVelocity)
            ), 
            maxSpeed
        );
            
    }
    this->position = Vector2Add(this->position, Vector2Scale(this->velocity, GetFrameTime()));
}