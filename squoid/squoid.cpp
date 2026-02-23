#include "squoid.h"
#include <raylib.h>
#include <raymath.h>

void Squoid::MoveSquoid(
    const Squoid squoids[], int squoidCount)
{
    Vector2 squoidToSquoidSeparation;
    Vector2 cursorTargetDirection;
    Vector2 cursorTargetVelocity;

    for (int k = 0; k < squoidCount; ++k)
    {
        Squoid otherSquoid = squoids[k];
        float distanceToOtherSquoid = Vector2Distance(this->position, otherSquoid.position) - this->squoidRadius - otherSquoid.squoidRadius;
    }
}
