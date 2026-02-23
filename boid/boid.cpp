#include "boid.h"
#include "../squoid/squoid.h"
#include <raylib.h>
#include <raymath.h>

void Boid::SteerBoid(const Boid flock[], int boidCount, const Squoid squoids[], int squoidCount)
{
    float boidsInRange;
    float squoidsInSeparationRange;
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionDirection = (Vector2){0.0f, 0.0f};
    Vector2 squoidSeparationAcceleration = (Vector2){0.0f, 0.0f};

    Vector2 totalBoidAcceleration = {0};

    if (!this->isAlive)
    {
        return;
    }

    for (int i = 0; i < boidCount; ++i)
    {
        Boid otherBoid = flock[i];

        if (this->id == otherBoid.id)
            continue;
        if (!otherBoid.isAlive)
            continue;
        float distanceToOtherBoid = Vector2Distance(otherBoid.position, this->position);

        if (distanceToOtherBoid < this->perceptionRadius && distanceToOtherBoid > 0.01f)
        {
            cohesionDirection = Vector2Normalize(Vector2Subtract(otherBoid.position, this->position));

            if (distanceToOtherBoid < this->repelRadius)
            {
                Vector2 separationDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));
                Vector2 wSeparationAccel = Vector2Scale(separationDirection, 1 / distanceToOtherBoid);

                separationAcceleration = Vector2Add(separationAcceleration, wSeparationAccel);
            }

            alignmentAcceleration = Vector2Add(alignmentAcceleration, otherBoid.velocity);

            cohesionAcceleration = Vector2Add(cohesionAcceleration, cohesionDirection);

            boidsInRange++;
        }
    }

    for (int j = 0; j < squoidCount; ++j)
    {
        Squoid squoid = squoids[j];

        float distanceToSquoid = (Vector2Distance(this->position, squoid.position) - squoid.squoidRadius - this->boidRadius);

        if (distanceToSquoid <= perceptionRadius * 3 && distanceToSquoid > 0.01f)
        {
            Vector2 squoidSeparationDirection = Vector2Normalize(Vector2Subtract(this->position, squoid.position));

            Vector2 wSquoidSeparationVelocity = Vector2Scale(squoidSeparationDirection, 1 / (distanceToSquoid * 0.33f));

            squoidSeparationAcceleration = Vector2Add(squoidSeparationAcceleration, wSquoidSeparationVelocity);

            if (distanceToSquoid < squoid.squoidRadius /2 )
            {
                this->isAlive = false;
                
            }
        }
    }

    if (boidsInRange > 0)
    {
        alignmentAcceleration = Vector2Scale(alignmentAcceleration, 1 / boidsInRange);
        cohesionAcceleration = Vector2Scale(cohesionAcceleration, 1 / boidsInRange);
    }

    cohesionAcceleration = Vector2Scale(cohesionAcceleration, cohesionStrength);
    alignmentAcceleration = Vector2Scale(alignmentAcceleration, alignmentStrength);
    separationAcceleration = Vector2Scale(separationAcceleration, separationStrength);
    squoidSeparationAcceleration = Vector2Scale(squoidSeparationAcceleration, squoidSeparationStrength);

    totalBoidAcceleration.x += cohesionAcceleration.x + alignmentAcceleration.x + separationAcceleration.x + squoidSeparationAcceleration.x;

    totalBoidAcceleration.y +=cohesionAcceleration.y + alignmentAcceleration.y + separationAcceleration.y + squoidSeparationAcceleration.y;

    this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidAcceleration), 0, maxSpeed);

    this->position = Vector2Add(this->position, this->velocity);
    this->rotation = atan2f(this->velocity.y, this->velocity.x);
}