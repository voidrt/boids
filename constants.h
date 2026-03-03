#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "config.h"
#include <array>
#include <vector>

constexpr int CELL_SIZE = BOID_PERCEPTION_RADIUS;
constexpr int ROWS = (WORLD_HEIGHT + CELL_SIZE - 1) / CELL_SIZE;
constexpr int COLUMNS = (WORLD_WIDTH + CELL_SIZE - 1) / CELL_SIZE;
constexpr int ESTIMATE_BOID_DISTRIBUTION = (MAX_BOIDS / (ROWS * COLUMNS)) * 4;

using SpatialGrid = std::array<std::array<std::vector<int>, COLUMNS>, ROWS>;

#endif