#include "squoid.h"
#include <raylib.h>
#include <raymath.h>

void Squoid::MoveSquoid(
    const Squoid squoids[], int squoidCount)
{
    float squoidsInRange;
    Vector2 squoidSeparationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cursorTargetDirection;
    Vector2 cursorTargetVelocity;
    Vector2 totalSquoidAcceleration = (Vector2){0.0f, 0.0f};

    for (int k = 0; k < squoidCount; ++k)
    {
        float distanceToSquoid = Vector2Distance(this->position, squoids[k].position) - this->size - squoids[k].size;

        if (distanceToSquoid < this->perceptionRadius)
        {
            Vector2 squoidSeparationDirection = Vector2Normalize(Vector2Subtract(this->position, squoids[k].position));

            Vector2 wSquoidSeparationVelocity = squoidSeparationDirection * (1 / distanceToSquoid * 10);

            squoidSeparationAcceleration = Vector2Add(squoidSeparationAcceleration, wSquoidSeparationVelocity);
        }
    }

    totalSquoidAcceleration.x = (squoidSeparationAcceleration.x * separationStrength);
    totalSquoidAcceleration.y = (squoidSeparationAcceleration.y * separationStrength);

    this->velocity = Vector2Add(this->velocity, totalSquoidAcceleration);

    this->velocity = Vector2Scale(Vector2Normalize(this->velocity), this->maxSpeed);
    this->position = Vector2Add(this->position, this->velocity);

    this->rotation = atan2f(this->velocity.y, this->velocity.x) * RAD2DEG;
}
