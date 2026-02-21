#include "boid.h"
#include <raylib.h>
#include <raymath.h>

void Boid::moveBoid()
{
    Vector2 targetDirection = Vector2Normalize(Vector2Subtract(GetMousePosition(), position));
    Vector2 targetVelocity = targetDirection * maxSpeed;
    Vector2 targetSteeringForce = Vector2Scale(Vector2Subtract(targetVelocity, velocity), 0.3f);
    Vector2 acceleration = Vector2ClampValue(targetSteeringForce, 0, steerStrenght);
    velocity = Vector2ClampValue(Vector2Add(velocity, Vector2Scale(acceleration, (0.1f + GetFrameTime()))), 0, maxSpeed);

    position = Vector2Add(position, Vector2Scale(velocity, 0.1f + GetFrameTime()));
}