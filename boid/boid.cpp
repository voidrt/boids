using namespace std;
#include "boid.h"
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

void Boid::MoveBoid(const int &WorldWidth, const int &WorldHeight)
{
    this->position += this->velocity;
    this->rotation = atan2f(this->velocity.y, this->velocity.x);

    if (this->position.x > WorldWidth)
        this->position.x = -WorldWidth;
    else if (this->position.x < -WorldWidth)
        this->position.x = WorldWidth;

    if (this->position.y > WorldHeight)
        this->position.y = -WorldHeight;
    else if (this->position.y < -WorldHeight)
        this->position.y = WorldHeight;
}

void Boid::SteerBoid(const Boid flock[], int boidCount, const unordered_map<string, vector<int>> &worldGrid)
{
    if (!this->isAlive)
    {
        return;
    }

    int currentCellX = (int)floor(this->position.x / this->perceptionRadius);
    int currentCellY = (int)floor(this->position.y / this->perceptionRadius);

    int boidsInRange = 0;
    Vector2 separationAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 alignmentAcceleration = (Vector2){0.0f, 0.0f};
    Vector2 cohesionAcceleration = (Vector2){0.0f, 0.0f};

    Vector2 totalBoidAcceleration = {0.0f, 0.0f};

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            string neighborCellHash = to_string(x + currentCellX) + "," + to_string(y + currentCellY);

            auto cell = worldGrid.find(neighborCellHash);

            if (cell != worldGrid.end())
            {
                for (int neighbor : cell->second)
                {
                    if (!flock[neighbor].isAlive || this->id == neighbor)
                    {
                        continue;
                    }

                    float distanceToFlock = Vector2Distance(flock[neighbor].position, this->position) - this->size - flock[neighbor].size;

                    if (distanceToFlock < this->perceptionRadius && distanceToFlock > 1)
                    {
                        boidsInRange++;

                        Vector2 cohesionDirection = Vector2Normalize(Vector2Subtract(flock[neighbor].position, this->position));

                        if (distanceToFlock < this->repelRadius && distanceToFlock > 1)
                        {
                            Vector2 separationDirection = Vector2Scale(cohesionDirection, -1);
                            Vector2 wSeparationAccel = Vector2Scale(separationDirection, 1 / distanceToFlock);

                            separationAcceleration = Vector2Add(separationAcceleration, wSeparationAccel);
                        }
                        else if (distanceToFlock <= 1)
                        {
                            float jitter = this->maxSpeed / 2;
                            Vector2 randomMovement = (Vector2){(float)GetRandomValue(-jitter, jitter), (float)GetRandomValue(-jitter, jitter)};

                            this->velocity += Vector2Add(randomMovement, Vector2Scale(flock[neighbor].velocity, 1 / 5));
                        }

                        alignmentAcceleration = Vector2Normalize(Vector2Add(alignmentAcceleration, flock[neighbor].velocity));

                        cohesionAcceleration = Vector2Add(cohesionAcceleration, cohesionDirection);
                    }
                }
            }
        }
    }

    if (boidsInRange >= 1)
    {
        alignmentAcceleration = Vector2Scale(alignmentAcceleration, 1 / boidsInRange);
        cohesionAcceleration = Vector2Scale(cohesionAcceleration, 1 / boidsInRange);
    }

    cohesionAcceleration *= cohesionStrength;
    alignmentAcceleration *= alignmentStrength;
    separationAcceleration *= separationStrength;

    totalBoidAcceleration.x += cohesionAcceleration.x + alignmentAcceleration.x + separationAcceleration.x;

    totalBoidAcceleration.y += cohesionAcceleration.y + alignmentAcceleration.y + separationAcceleration.y;

    this->velocity = Vector2ClampValue(Vector2Add(this->velocity, totalBoidAcceleration), 0, maxSpeed);
}