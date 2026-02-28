#include "boid.h"
#include <raylib.h>
#include <raymath.h>

void Boid::UpdatePosition()
{

    if (this->position.x >= WORLD_WIDTH)
        this->position.x = -WORLD_WIDTH;
    else if (this->position.x <= -WORLD_WIDTH)
        this->position.x = WORLD_WIDTH;

    if (this->position.y >= WORLD_HEIGHT)
        this->position.y = -WORLD_HEIGHT;

    else if (this->position.y <= -WORLD_HEIGHT)
        this->position.y = WORLD_HEIGHT;

    this->position = Vector2Add(this->position, this->velocity);
    this->rotation = atan2f(this->velocity.y, this->velocity.x);
}

void Boid::UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock)
{
    if (!this->isAlive)
    {
        return;
    }

    int boidsInRange = 0;
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};

    Vector2 totalBoidAcceleration = {0};

    // for loop

    if (boidsInRange >= 1)
    {
        alignmentAcceleration = Vector2Scale(alignmentAcceleration, 1 / boidsInRange);
        cohesionAcceleration = Vector2Scale(cohesionAcceleration, 1 / boidsInRange);
    }

    cohesionAcceleration = Vector2Scale(cohesionAcceleration, cohesionStrength);
    alignmentAcceleration = Vector2Scale(alignmentAcceleration, alignmentStrength);
    separationAcceleration = Vector2Scale(separationAcceleration, separationStrength);

    totalBoidAcceleration.x += cohesionAcceleration.x + alignmentAcceleration.x + separationAcceleration.x;

    totalBoidAcceleration.y += cohesionAcceleration.y + alignmentAcceleration.y + separationAcceleration.y;

    this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidAcceleration), 0, maxSpeed);
}