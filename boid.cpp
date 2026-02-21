#include "boid.h"
#include <raylib.h>
#include <raymath.h>

void Boid::moveBoid(const Boid flock[], int boidCount)
{
    velocity = Vector2Add(velocity, (Vector2) {0});
    position = Vector2Add(position, Vector2Scale(velocity, 0.1f+ GetFrameTime()));

    // (int i = 0; i < boidCount; ++i)
    // {
    //     Boid otherBoid = flock[i];
    //     if (this->id == otherBoid.id)
    //         continue;
    //     float distanceToOtherBoid = Vector2Distance(this->position, otherBoid.position);

    //     if (distanceToOtherBoid < 60 && distanceToOtherBoid > 0.0f)
    //     {
    //         Vector2 repelDirection = Vector2Normalize(Vector2Subtract(this->position, otherBoid.position));

    //         separationForce = (Vector2){repelDirection.x / distanceToOtherBoid, repelDirection.y / distanceToOtherBoid};
    //     }
    // }
}