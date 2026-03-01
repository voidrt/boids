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

void Boid::UpdateVelocity(const std::array<Boid, MAX_BOIDS> &flock, const std::array<std::vector<int>, TOTAL_CELLS> &worldGrid, int gridColumns, int gridRows)
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

    int positionInGridY = std::floor(this->position.y / BOID_PERCEPTION_RADIUS);
    int positionInGridX = std::floor(this->position.x / BOID_PERCEPTION_RADIUS);
    int worldGridIndex = (positionInGridY * gridColumns) + positionInGridX;

    for (int delta_x = -1; delta_x <= 1; delta_x++)
    {
        for (int delta_y = -1; delta_y <= 1; delta_y++)
        {
            int neighborCellsX = positionInGridX + delta_x;
            int neighborCellsY = positionInGridY + delta_y;

            if (neighborCellsX < 0 || neighborCellsX >= gridColumns || neighborCellsY < 0 || neighborCellsY >= gridRows)
                continue;

            int neighborCellIndex = (neighborCellsY * gridColumns) + neighborCellsX;
            float distanceToNeighbor = 0.0f;
            Vector2 cohesionDirection = {0.0f, 0.0f};

            for (int neighborIdentifier : worldGrid[worldGridIndex])
            {
                if (this->identifier == neighborIdentifier || !flock[neighborIdentifier].isAlive)
                    continue;

                distanceToNeighbor = Vector2Distance(this->position, flock[neighborIdentifier].position);

                if (distanceToNeighbor < BOID_PERCEPTION_RADIUS && distanceToNeighbor > (this->size / 2))
                {
                    boidsInRange++;

                    cohesionDirection += Vector2Subtract(flock[neighborIdentifier].position, this->position);
                    alignmentVelocity += (flock[neighborIdentifier].velocity);

                    if (distanceToNeighbor < BOID_SEPARATION_RADIUS && distanceToNeighbor > (this->size / 2))
                    {
                        Vector2 wSeparationDirection = Vector2Scale(cohesionDirection, -1);
                        wSeparationDirection /= distanceToNeighbor;
                    }
                    else if (distanceToNeighbor <= (this->size / 2))
                    {
                        float jitter = BOID_SPEED / 2;
                        Vector2 randomMovement = (Vector2){(float)GetRandomValue(-jitter, jitter), (float)GetRandomValue(-jitter, jitter)};

                        this->velocity += Vector2Add(randomMovement, Vector2Scale(flock[neighborIdentifier].velocity, 1 / 5));
                    }
                }
            }
        }
    }

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