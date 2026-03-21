#include "boid.h"
#include <algorithm>

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

void Boid::SteerBoid(const std::array<Boid, MAX_BOIDS> &flock, const SpatialGrid &worldGrid)
{
    float boidsInRange{};
    float boidsInSeparationRange{};
    float perceptionRadiusSqr = (BOID_PERCEPTION_RADIUS * BOID_PERCEPTION_RADIUS);
    float separationRadiusSqr = (BOID_SEPARATION_RADIUS * BOID_SEPARATION_RADIUS);

    BoidForce alignment{};
    BoidForce cohesion{};
    BoidForce separation{};

    Vector2 totalBoidAcceleration = {0};

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

                float distanceToFlockSqr = Vector2DistanceSqr(this->position, flock[gridId].position);

                if (distanceToFlockSqr < perceptionRadiusSqr)
                {
                    boidsInRange++;

                    alignment.direction += flock[gridId].velocity;

                    cohesion.direction += Vector2Subtract(flock[gridId].position, this->position);

                    if (distanceToFlockSqr <= separationRadiusSqr && distanceToFlockSqr > 1)
                    {
                        boidsInSeparationRange++;

                        Vector2 positionDifference = Vector2Subtract(this->position, flock[gridId].position) / distanceToFlockSqr;

                        separation.direction += positionDifference;
                    }
                }
            }
        }
    }

    if (boidsInRange > 0)
    {
        float iBoidsInRange = 1.0f / boidsInRange;

        alignment.ComputeForce(iBoidsInRange, this->velocity, BOID_ALIGNMENT_STRENGTH);
        cohesion.ComputeForce(iBoidsInRange, this->velocity, BOID_COHESION_STRENGTH);
    }
    if (boidsInSeparationRange > 0)
    {
        float iBoidsInSeparationRange = 1.0f / boidsInSeparationRange;

        separation.ComputeForce(iBoidsInSeparationRange, this->velocity, BOID_SEPARATION_STRENGTH);
    }
    totalBoidAcceleration += cohesion.acceleration + alignment.acceleration + separation.acceleration;

    this->velocity += (totalBoidAcceleration * GetFrameTime());
    this->velocity = Vector2ClampValue(this->velocity, BOID_SPEED / 2, BOID_SPEED);
}
