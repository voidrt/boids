using namespace std;
#include "boid.h"
#include "../squoid/squoid.h"
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <string>
#include <vector>

void Boid::MoveBoid()
{
    this->position += this->velocity;
}

void Boid::SteerBoid(const Boid flock[], int boidCount, const Squoid squoids[], int squoidCount, const unordered_map<string, vector<int>> &worldGrid)
{
    if (!this->isAlive)
    {
        return;
    }

    int currentCellX = (int)floor(this->position.x / this->perceptionRadius);
    int currentCellY = (int)floor(this->position.y / this->perceptionRadius);

    for (int x_cells = -1; x_cells <= 1; x_cells++)
    {
        for (int y_cells = -1; y_cells <= 1; y_cells++)
        {
            string cellName = to_string(currentCellX + y_cells) + "," + to_string(currentCellY + y_cells);
            auto gridCell = worldGrid.find(cellName);
        }
    }
}