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
        Squoid otherSquoid = squoids[k];
        float distanceToOtherSquoid = Vector2Distance(this->position, otherSquoid.position) - this->squoidRadius - otherSquoid.squoidRadius;

        if (distanceToOtherSquoid < this->perceptionRadius && distanceToOtherSquoid > 0.01f)
        {
            Vector2 squoidSeparationDirection = Vector2Normalize(Vector2Subtract(this->position, otherSquoid.position));

            Vector2 wSquoidSeparationVelocity = Vector2Scale(squoidSeparationDirection, 1 / 
                (distanceToOtherSquoid * 0.1f));

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
