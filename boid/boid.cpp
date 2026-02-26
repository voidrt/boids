using namespace std;
#include "boid.h"
#include "../squoid/squoid.h"
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <string>
#include <vector>

void Boid::SteerBoid(const Boid flock[], int boidCount, const Squoid squoids[], int squoidCount, const unordered_map<string, vector<int>> &worldGrid)
{
    float boidsInRange;
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 squoidSeparationAcceleration = (Vector2){0.0f, 0.0f};

    Vector2 totalBoidAcceleration = {0};

    if (!this->isAlive)
    {
        return;
    }

    for (int i = 0; i < boidCount; ++i)
    {

        if (this->id == flock[i].id)
            continue;
        if (!flock[i].isAlive)
            continue;

        float distanceToFlockSqr = Vector2DistanceSqr(flock[i].position, this->position) - this->size;

        if (distanceToFlockSqr < (this->perceptionRadius * this->perceptionRadius) && distanceToFlockSqr > 6)
        {
            boidsInRange++;

            Vector2 cohesionDirection = Vector2Normalize(Vector2Subtract(flock[i].position, this->position));

            if (distanceToFlockSqr < (this->repelRadius * this->repelRadius) && distanceToFlockSqr > 6)
            {
                Vector2 separationDirection = Vector2Scale(cohesionDirection, -1);
                Vector2 wSeparationAccel = Vector2Scale(separationDirection, 1 / sqrt(distanceToFlockSqr));

                separationAcceleration = Vector2Add(separationAcceleration, wSeparationAccel);
            }
            else if (distanceToFlockSqr <= 6)
            {
                float jitter = this->maxSpeed / 2;
                Vector2 randomMovement = (Vector2){(float)GetRandomValue(-jitter, jitter), (float)GetRandomValue(-jitter, jitter)};

                this->velocity += Vector2Add(randomMovement, Vector2Scale(flock[i].velocity, 1 / 5));
            }

            alignmentAcceleration = Vector2Add(alignmentAcceleration, flock[i].velocity);

            cohesionAcceleration = Vector2Add(cohesionAcceleration, cohesionDirection);
        }
    }

    if (boidsInRange > 0)
    {
        alignmentAcceleration = Vector2Scale(alignmentAcceleration, 1 / boidsInRange);
        cohesionAcceleration = Vector2Scale(cohesionAcceleration, 0.1 / boidsInRange);
    }

    for (int j = 0; j < squoidCount; ++j)
    {
        Squoid squoid = squoids[j];

        float distanceToSquoidSqr = (Vector2DistanceSqr(this->position, squoid.position) - squoid.squoidSize - this->size);

        if (distanceToSquoidSqr <= (this->perceptionRadius * this->perceptionRadius * 2) && distanceToSquoidSqr > 6)
        {
            Vector2 squoidSeparationDirection = Vector2Normalize(Vector2Subtract(this->position, squoid.position));

            Vector2 wSquoidSeparationVelocity = Vector2Scale(squoidSeparationDirection, 1 / sqrt(distanceToSquoidSqr));

            squoidSeparationAcceleration = Vector2Add(squoidSeparationAcceleration, wSquoidSeparationVelocity);

            if (distanceToSquoidSqr < (this->repelRadius * this->repelRadius) + (squoid.squoidSize * squoid.squoidSize))
            {
                cohesionAcceleration = Vector2Zero();
                separationAcceleration = Vector2Zero();
            }

            if (distanceToSquoidSqr < (squoid.squoidSize * squoid.squoidSize))
            {
                this->isAlive = false;
            }
        }
    }

    cohesionAcceleration *= cohesionStrength;
    alignmentAcceleration *= alignmentStrength;
    separationAcceleration *= separationStrength;
    squoidSeparationAcceleration *= squoidSeparationStrength;

    totalBoidAcceleration.x += cohesionAcceleration.x + alignmentAcceleration.x + separationAcceleration.x + squoidSeparationAcceleration.x;

    totalBoidAcceleration.y += cohesionAcceleration.y + alignmentAcceleration.y + separationAcceleration.y + squoidSeparationAcceleration.y;

    this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidAcceleration), 0, maxSpeed);

    this->position = Vector2Add(this->position, this->velocity);
    this->rotation = atan2f(this->velocity.y, this->velocity.x);
}