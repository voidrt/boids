#include "whoid.h"
#include <raymath.h>
#include <algorithm>

void Whoid::DrawWhoid()
{
    Rectangle whoidBody = {
        this->position.x,
        this->position.y,
        WHOID_SIZE,
        WHOID_SIZE / 3,
    };
    DrawRectanglePro(whoidBody, (Vector2){whoidBody.width / 2, whoidBody.height / 2}, this->rotation, {100, 110, 147, 255});
}

void Whoid::MoveWhoid()
{
    this->position += this->velocity;
    this->rotation = atan2f(this->velocity.y, this->velocity.x) * RAD2DEG;
}

void Whoid::SteerWhoid(const SpatialGrid &worldGrid, std::array<Whoid, MAX_WHOIDS> whoidGroup)
{
    Vector2 separationTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 edgeAvoidanceTargetDirection = (Vector2){0.0f, 0.0f};
    Vector2 edgeAvoidanceAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 totalWhoidAcceleration = (Vector2){0.0f, 0.0f};

    float whoidsInRange{};

    int currentCell = (this->position.x / CELL_SIZE);
    int currentRow = (this->position.y / CELL_SIZE);
    currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
    currentRow = std::clamp(currentRow, 0, ROWS - 1);

    if (this->position.x < WHOID_SIZE * 2)
    {
        edgeAvoidanceTargetDirection.x += EDGE_AVOIDANCE_STRENGTH;
    }
    else if (this->position.x > WORLD_WIDTH - WHOID_SIZE * 2)
    {
        edgeAvoidanceTargetDirection.x -= EDGE_AVOIDANCE_STRENGTH;
    }

    if (this->position.y < WHOID_SIZE * 2)
    {
        edgeAvoidanceTargetDirection.y += EDGE_AVOIDANCE_STRENGTH;
    }
    else if (this->position.y > WORLD_HEIGHT - WHOID_SIZE * 2)
    {
        edgeAvoidanceTargetDirection.y -= EDGE_AVOIDANCE_STRENGTH;
    }

    for (int deltaY = -3; deltaY <= 3; deltaY++)
    {
        for (int deltaX = -3; deltaX <= 3; deltaX++)
        {
            int cellDeltaX = (currentCell + deltaX + COLUMNS) % COLUMNS;
            int cellDeltaY = (currentCell + deltaY + ROWS) % ROWS;

            for (int gridId : worldGrid[currentRow][currentCell])
            {
                if (gridId < MAX_BOIDS)
                    continue;

                Whoid &otherWhoid = whoidGroup[gridId - MAX_BOIDS];

                float distanceToWhoid = Vector2Distance(this->position, otherWhoid.position);

                if (distanceToWhoid <= WHOID_SEPARATION_RADIUS && distanceToWhoid > 1)
                {
                    whoidsInRange++;

                    Vector2 positionDifference = Vector2Normalize(Vector2Subtract(this->position, otherWhoid.position));

                    separationTargetDirection += positionDifference;
                }
            }
        }
    }
    if (whoidsInRange > 0)
    {
        separationTargetDirection /= whoidsInRange;
        separationTargetDirection = Vector2Normalize(separationTargetDirection) * WHOID_SPEED;

        separationAcceleration = Vector2Subtract(separationTargetDirection, this->velocity);
        separationAcceleration *= WHOID_SEPARATION_STRENGTH;
    }

    edgeAvoidanceTargetDirection = Vector2Normalize(edgeAvoidanceTargetDirection) * WHOID_SPEED;
    edgeAvoidanceAcceleration += (edgeAvoidanceTargetDirection)*GetFrameTime() * WHOID_SPEED;

    totalWhoidAcceleration += separationAcceleration + edgeAvoidanceAcceleration;

    this->velocity += totalWhoidAcceleration;
    this->velocity = Vector2ClampValue(this->velocity, WHOID_SPEED / 1.5, WHOID_SPEED);
}