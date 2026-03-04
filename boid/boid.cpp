#include "boid.h"
#include <raymath.h>
#include <algorithm>

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
    this->position += this->velocity;
}

void Boid::UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock, const SpatialGrid &worldGrid)
{
    float boidsInRange{};
    int boidsInSeparationRange{};
    float distanceToFlock{};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 separationTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 totalBoidAcceleration = {0.0f, 0.0f};

    int currentCell = (this->position.x / CELL_SIZE);
    int currentRow = (this->position.y / CELL_SIZE);
    currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
    currentRow = std::clamp(currentRow, 0, ROWS - 1);

    for (int deltaY = -1; deltaY <= 1; deltaY++)
    {
        for (int deltaX = -1; deltaX <= 1; deltaX++)
        {
            int deltaCellX = (currentCell + deltaX + COLUMNS) % COLUMNS;
            int deltaCellY = (currentRow + deltaY + ROWS) % ROWS;

            for (int neighborId : worldGrid[deltaCellY][deltaCellX])
            {
                if (this->identifier == flock[neighborId].identifier)
                    continue;

                float distanceToFlock = Vector2Distance(this->position, flock[neighborId].position);

                if (distanceToFlock < BOID_PERCEPTION_RADIUS && distanceToFlock > 0)
                {
                    boidsInRange++;

                    alignmentAcceleration += flock[neighborId].velocity;

                    cohesionTargetDirection += Vector2Subtract(flock[neighborId].position, this->position);

                    if (distanceToFlock < BOID_SEPARATION_RADIUS && distanceToFlock > 0)
                    {
                        boidsInSeparationRange++;

                        Vector2 positionDifference = Vector2Normalize(Vector2Subtract(this->position, flock[neighborId].position));

                        positionDifference *= (1 / distanceToFlock);

                        separationTargetDirection += positionDifference;
                    }
                }
            }
        }
    }

    if (boidsInRange > 0)
    {
        alignmentAcceleration /= boidsInRange;
        alignmentAcceleration = Vector2Normalize(alignmentAcceleration) * BOID_SPEED;
        alignmentAcceleration = Vector2Subtract(alignmentAcceleration, this->velocity);
        alignmentAcceleration *= BOID_ALIGNMENT_STRENGTH;
        totalBoidAcceleration += alignmentAcceleration;

        cohesionTargetDirection /= boidsInRange;
        cohesionTargetDirection = Vector2Normalize(cohesionTargetDirection) * BOID_SPEED;
        cohesionAcceleration = Vector2Subtract(cohesionTargetDirection, this->velocity);
        cohesionAcceleration *= BOID_COHESION_STRENGTH;

        totalBoidAcceleration += cohesionAcceleration;
        if (boidsInSeparationRange > 0)
        {
            separationTargetDirection /= boidsInSeparationRange;
            separationTargetDirection = Vector2Normalize(separationTargetDirection) * BOID_SPEED;
            separationAcceleration = Vector2Subtract(separationTargetDirection, this->velocity);
            separationAcceleration *= BOID_SEPARATION_STRENGTH;

            totalBoidAcceleration += separationAcceleration;
        }
    }

    this->velocity += (totalBoidAcceleration * GetFrameTime());
    this->velocity = Vector2ClampValue(this->velocity, BOID_SPEED / 1.5, BOID_SPEED);
}
