#include "boid.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>

void Boid::moveBoid(const Boid flock[], int boidCount)
{
    float boidsInRange;
    Vector2 repelVelocity = (Vector2){0.0f, 0.0f};

    this->position.x += this->velocity.x * 0.1f + GetFrameTime();
    this->position.y += this->velocity.y * 0.1f + GetFrameTime();

    for (int i = 0; i < boidCount; ++i)
    {
        Boid otherBoid = flock[i];
        if (this->id == otherBoid.id)
            continue;
        float distanceToOtherBoid = Vector2Distance(this->position, otherBoid.position);

        if (distanceToOtherBoid < this->repelRadius)
        {
            Vector2 repelDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));

            // increased velocity equal to distance
            repelVelocity = Vector2ClampValue(repelDirection, 0, repelStrength);
            repelVelocity = (Vector2){repelVelocity.x / distanceToOtherBoid, repelVelocity.y / distanceToOtherBoid};
            ++boidsInRange;
        }
    }

    if (boidsInRange > 0)
    {
        this->velocity = Vector2Add(velocity, repelVelocity);
        this->velocity = Vector2ClampValue(Vector2Normalize(velocity), 0, maxSpeed);
    }
}