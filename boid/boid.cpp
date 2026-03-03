#include "boid.h"
#include <raymath.h>

void Boid::UpdatePosition()
{

    while (this->position.x < 0.0f)
    {
        this->position.x += WORLD_WIDTH;
    }
    while (this->position.x >= WORLD_WIDTH)
    {
        this->position.x -= WORLD_WIDTH;
    }

    while (this->position.y < 0.0f)
    {
        this->position.y += WORLD_HEIGHT;
    }
    while (this->position.y >= WORLD_HEIGHT)
    {
        this->position.y -= WORLD_HEIGHT;
    }
    this->position = Vector2Add(this->position, this->velocity);
}

void Boid::UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock, const SpatialGrid &worldGrid)
{
    float boidsInRange{};
    float distanceToFlock{};
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 squoidSeparationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 totalBoidAcceleration = {0.0f, 0.0f};

    // int currentCell = (this->position.x / CELL_SIZE);
    // int currentRow = (this->position.y / CELL_SIZE);
    // currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
    // currentRow = std::clamp(currentRow, 0, ROWS - 1);

    // for (int deltaY = -1; deltaY <= 1; deltaY++)
    // {
    //     for (int deltaX = -1; deltaX <= 1; deltaX++)
    //     {
    //         float deltaCellX = (currentCell + deltaX) * BOID_PERCEPTION_RADIUS;
    //         float deltaCellY = (currentRow + deltaY) * BOID_PERCEPTION_RADIUS;

    //         Rectangle cellRect = {deltaCellX, deltaCellY, BOID_PERCEPTION_RADIUS, BOID_PERCEPTION_RADIUS};

    //         DrawRectangleLinesEx(cellRect, 10, RED);
    //     }
    // }

    if (boidsInRange >= 1)
    {
        alignmentAcceleration = Vector2Scale(alignmentAcceleration, 1 / boidsInRange);
        cohesionAcceleration = Vector2Scale(cohesionAcceleration, 1 / boidsInRange);
    }

    cohesionAcceleration = Vector2Scale(cohesionAcceleration, BOID_COHESION_STRENGTH);
    alignmentAcceleration = Vector2Scale(alignmentAcceleration, BOID_ALIGNMENT_STRENGTH);
    separationAcceleration = Vector2Scale(separationAcceleration, BOID_SEPARATION_STRENGTH);

    totalBoidAcceleration = cohesionAcceleration + alignmentAcceleration + separationAcceleration + squoidSeparationAcceleration;

    this->velocity = Vector2ClampValue((this->velocity + totalBoidAcceleration), 0, BOID_SPEED);
}