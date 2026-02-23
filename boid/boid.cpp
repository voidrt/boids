#include "boid.h"
#include "../squoid/squoid.h"
#include <raylib.h>
#include <raymath.h>

void Boid::SteerBoid(const Boid flock[], int boidCount, const Squoid squoids[], int squoidCount)
{
    float boidsInRange;
    float squoidsInSeparationRange;
    float boidsInSeparationRange;
    Vector2 separationVelocity = (Vector2){0.0f, 0.0f};
    Vector2 alignmentVelocity = (Vector2){0.0f, 0.0f};
    Vector2 cohesionVelocity = (Vector2){0.0f, 0.0f};
    Vector2 cohesionDirection = (Vector2){0.0f, 0.0f};
    Vector2 squoidSeparationVelocity = (Vector2){0.0f, 0.0f};

    Vector2 totalBoidVelocity = {0};

    for (int i = 0; i < boidCount; ++i)
    {
        Boid otherBoid = flock[i];

        if (this->id == otherBoid.id)
            continue;

        float distanceToOtherBoid = Vector2Distance(otherBoid.position, this->position);

        if (distanceToOtherBoid < this->perceptionRadius && distanceToOtherBoid > 0.01f)
        {
            cohesionDirection = Vector2Normalize(Vector2Subtract(otherBoid.position, this->position));

            if (distanceToOtherBoid < this->repelRadius)
            {
                Vector2 separationDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));
                Vector2 wSeparationVelocity = Vector2Scale(separationDirection, 1 / distanceToOtherBoid);

                separationVelocity = Vector2Add(separationVelocity, wSeparationVelocity);
                ++boidsInSeparationRange;
            }

            alignmentVelocity = Vector2Add(alignmentVelocity, otherBoid.velocity);

            cohesionVelocity = Vector2Add(cohesionVelocity, cohesionDirection);

            boidsInRange++;
        }
    }

    for (int j = 0; j < squoidCount; ++j)
    {
        Squoid squoid = squoids[j];
        float distanceToSquoid = (Vector2Distance(this->position, squoid.position) + squoid.squoidRadius + this->boidRadius);

        if (distanceToSquoid <= perceptionRadius * 3 && distanceToSquoid > 0.01f)
        {
            Vector2 squoidSeparationDirection = Vector2Normalize(Vector2Subtract(this->position, squoid.position));

            Vector2 wSquoidSeparationVelocity = Vector2Scale(squoidSeparationDirection, 1 / distanceToSquoid);

            squoidSeparationVelocity = Vector2Add(squoidSeparationVelocity, wSquoidSeparationVelocity);

            squoidsInSeparationRange++;
        }

        if (boidsInRange > 0)
        {

            alignmentVelocity = Vector2Scale(alignmentVelocity, 1 / boidsInRange);

            cohesionVelocity = Vector2Scale(cohesionVelocity, 1 / boidsInRange);
        }

        totalBoidVelocity.x += (cohesionVelocity.x * cohesionStrength) + (separationVelocity.x * separationStrength) + (alignmentVelocity.x * alignmentStrength) + (squoidSeparationVelocity.x * squoidSeparationStrength);
        totalBoidVelocity.y += (cohesionVelocity.y * cohesionStrength) + (separationVelocity.y * separationStrength) + (alignmentVelocity.y * alignmentStrength) + (squoidSeparationVelocity.y * 2);
    }
    this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidVelocity), 0, maxSpeed);

    this->position = Vector2Add(this->position, this->velocity);
}