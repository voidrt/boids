#include "boid.h"
#include <raymath.h>
#include <algorithm>
#include <iostream>

void Boid::DrawBoid()
{
    Vector2 v1, v2, v3;

    float size = this->size;

    v1 = Vector2Add(Vector2Rotate((Vector2){size, 0.0f}, atan2f(this->velocity.y, this->velocity.x)), this->position);
    v2 = Vector2Add(Vector2Rotate((Vector2){-(size) / 1.5f, -size / 1.5f}, atan2f(this->velocity.y, this->velocity.x)), this->position);
    v3 = Vector2Add(Vector2Rotate((Vector2){-(size) / 1.5f, size / 1.5f}, atan2f(this->velocity.y, this->velocity.x)), this->position);

    DrawTriangle(v1, v2, v3, this->color);
}

void Boid::MoveBoid()
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

void Boid::SteerBoid(const std::array<Boid, MAX_BOIDS> &flock, const std::array<Whoid, MAX_WHOIDS> &whoidGroup, const SpatialGrid &worldGrid)
{
    float whoidsInRange{};
    float boidsInRange{};
    int boidsInSeparationRange{};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 separationTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 whoidSeparationTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 whoidSeparationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 totalBoidAcceleration = {0.0f, 0.0f};

    int currentCell = (this->position.x / CELL_SIZE);
    int currentRow = (this->position.y / CELL_SIZE);
    currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
    currentRow = std::clamp(currentRow, 0, ROWS - 1);

    for (int deltaY = -1; deltaY <= 1; deltaY++)
    {
        for (int deltaX = -1; deltaX <= 1; deltaX++)
        {
            int cellDeltaX = (currentCell + deltaX + COLUMNS) % COLUMNS;
            int cellDeltaY = (currentRow + deltaY + ROWS) % ROWS;

            for (int gridId : worldGrid[cellDeltaY][cellDeltaX])
            {
                if (gridId >= MAX_BOIDS)
                    continue;

                if (this->identifier == flock[gridId].identifier)
                    continue;
                if (!flock[gridId].isAlive)
                    continue;

                float distanceToFlock = Vector2Distance(this->position, flock[gridId].position);

                if (distanceToFlock < BOID_PERCEPTION_RADIUS && distanceToFlock > 0)
                {
                    boidsInRange++;

                    alignmentAcceleration += flock[gridId].velocity;

                    cohesionTargetDirection += Vector2Subtract(flock[gridId].position, this->position);

                    if (distanceToFlock < BOID_SEPARATION_RADIUS && distanceToFlock > 0)
                    {
                        boidsInSeparationRange++;

                        Vector2 positionDifference = Vector2Normalize(Vector2Subtract(this->position, flock[gridId].position));

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

        cohesionTargetDirection /= boidsInRange;
        cohesionTargetDirection = Vector2Normalize(cohesionTargetDirection) * BOID_SPEED;
        cohesionAcceleration = Vector2Subtract(cohesionTargetDirection, this->velocity);
        cohesionAcceleration *= BOID_COHESION_STRENGTH;

        if (boidsInSeparationRange > 0)
        {
            separationTargetDirection /= boidsInSeparationRange;
            separationTargetDirection = Vector2Normalize(separationTargetDirection) * BOID_SPEED;
            separationAcceleration = Vector2Subtract(separationTargetDirection, this->velocity);
            separationAcceleration *= BOID_SEPARATION_STRENGTH;
        }
    }

    for (int deltaY = -2; deltaY <= 2; deltaY++)
    {
        for (int deltaX = -2; deltaX <= 2; deltaX++)
        {
            int cellDeltaX = (currentCell + deltaX + COLUMNS) % COLUMNS;
            int cellDeltaY = (currentRow + deltaY + ROWS) % ROWS;

            for (int gridId : worldGrid[cellDeltaY][cellDeltaX])
            {
                if (gridId < MAX_BOIDS)
                    continue;

                int whoidId = gridId - MAX_BOIDS;

                float entitySizeSum = (WHOID_SIZE / 2) + this->size;

                float distanceToWhoid = Vector2Distance(this->position, whoidGroup[whoidId].position) - entitySizeSum;

                if (distanceToWhoid < BOID_TO_WHOID_PERCEPTION_RADIUS && distanceToWhoid > WHOID_SIZE)
                {
                    whoidsInRange++;

                    Vector2 positionDifference = Vector2Normalize(Vector2Subtract(this->position, whoidGroup[whoidId].position));

                    positionDifference *= (1 / distanceToWhoid);

                    whoidSeparationTargetDirection += positionDifference;
                }
                else if (distanceToWhoid <= entitySizeSum)
                {
                    this->isAlive = false;
                }
            }
        }
    }

    if (whoidsInRange > 0)
    {
        whoidSeparationTargetDirection *= (1 / whoidsInRange);
        whoidSeparationTargetDirection = Vector2Normalize(whoidSeparationTargetDirection) * BOID_SPEED;
        whoidSeparationAcceleration = Vector2Subtract(whoidSeparationTargetDirection, this->velocity);
        whoidSeparationAcceleration *= BOID_TO_WHOID_SEPARATION_STRENGTH;
    }

    totalBoidAcceleration += whoidSeparationAcceleration + cohesionAcceleration + alignmentAcceleration + separationAcceleration;

    this->velocity += (totalBoidAcceleration * GetFrameTime());
    this->velocity = Vector2ClampValue(this->velocity, BOID_SPEED / 3, BOID_SPEED);
}
