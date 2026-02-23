#include "boid.h"
#include <raylib.h>
#include <raymath.h>
#include <iostream>

void Boid::moveBoid(const Boid flock[], int boidCount)
{
    float boidsInRange;
    float boidsInSeparationRange;
    Vector2 separationVelocity = (Vector2){0.0f, 0.0f};
    Vector2 alignmentVelocity = (Vector2){0.0f, 0.0f};
    Vector2 cohesionVelocity = (Vector2){0.0f, 0.0f};
    Vector2 cohesionDirection = (Vector2){0.0f, 0.0f};
    Vector2 totalBoidVelocity = {0};

    for (int i = 0; i < boidCount; ++i)
    {
        Boid otherBoid = flock[i];

        if (this->id == otherBoid.id)
            continue;

        float distanceToOtherBoid = Vector2Distance(otherBoid.position, this->position);

        if (distanceToOtherBoid < this->perceptionRadius && distanceToOtherBoid > 0.01f)
        {
            if (distanceToOtherBoid < this->repelRadius)
            {
                Vector2 repelDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));
                Vector2 wSeparationVelocity = Vector2Scale(repelDirection, 1 / distanceToOtherBoid);

                separationVelocity = Vector2Add(separationVelocity, wSeparationVelocity);
                ++boidsInSeparationRange;
            }

            alignmentVelocity = Vector2Add(alignmentVelocity, otherBoid.velocity);

            cohesionDirection = Vector2Normalize(Vector2Subtract(otherBoid.position, this->position));
            cohesionVelocity = Vector2Add(cohesionVelocity, cohesionDirection);

            boidsInRange++;
        }
    }

    if (boidsInRange > 0)
    {
        if (boidsInSeparationRange > 0)
        {
            separationVelocity = Vector2Scale(separationVelocity, 1 / boidsInSeparationRange);
            separationVelocity = Vector2Scale(separationVelocity, this->repelStrength);
        }

        alignmentVelocity = Vector2Scale(Vector2Scale(alignmentVelocity, 1 / boidsInRange), this->alignmentStrength);

        cohesionVelocity = Vector2Scale(Vector2Scale(cohesionVelocity, 1 / boidsInRange), this->cohesionStrength);

        totalBoidVelocity.x += (cohesionVelocity.x * 0.2) + separationVelocity.x + alignmentVelocity.x;
        totalBoidVelocity.y += (cohesionVelocity.y * 0.2) + separationVelocity.y + alignmentVelocity.y;

        this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidVelocity), maxSpeed / 2, maxSpeed);
    }
    this->position = Vector2Add(this->position, this->velocity);
}