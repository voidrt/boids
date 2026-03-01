#include "boid.h"
#include <raylib.h>
#include <raymath.h>

void Boid::UpdatePosition()
{

    if (this->position.x > WORLD_WIDTH)
        this->position.x = 2;
    else if (this->position.x <= 1)
        this->position.x = WORLD_WIDTH;

    if (this->position.y > WORLD_HEIGHT)
        this->position.y = 2;

    else if (this->position.y <= 1)
        this->position.y = WORLD_HEIGHT;

    this->position = Vector2Add(this->position, this->velocity);
}

void Boid::UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock)
{
    if (!this->isAlive)
    {
        return;
    }

    int boidsInRange = 0;
    Vector2 separationVelocity = (Vector2){0.0f, 0.0f};
    Vector2 alignmentVelocity = (Vector2){0.0f, 0.0f};
    Vector2 cohesionVelocity = (Vector2){0.0f, 0.0f};

    Vector2 totalBoidVelocity = {0};

    // for loop

    if (boidsInRange >= 1)
    {
        alignmentVelocity = Vector2Scale(alignmentVelocity, 1 / boidsInRange);
        cohesionVelocity = Vector2Scale(cohesionVelocity, 1 / boidsInRange);
    }

    cohesionVelocity = Vector2Scale(cohesionVelocity, BOID_COHESION_STRENGTH);
    alignmentVelocity = Vector2Scale(alignmentVelocity, BOID_ALIGNMENT_STRENGTH);
    separationVelocity = Vector2Scale(separationVelocity, BOID_SEPARATION_RADIUS);

    totalBoidVelocity.x += cohesionVelocity.x + alignmentVelocity.x + separationVelocity.x;

    totalBoidVelocity.y += cohesionVelocity.y + alignmentVelocity.y + separationVelocity.y;

    this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidVelocity), 0, BOID_SPEED);
}