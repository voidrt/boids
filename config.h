#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include <array>
#include <vector>

#define MAX_BOIDS 3000
#define BOID_SPEED 18
#define BOID_BASE_SIZE 25
#define BOID_SEPARATION_RADIUS 90
#define BOID_PERCEPTION_RADIUS 320
#define BOID_SEPARATION_STRENGTH 2.8f
#define BOID_ALIGNMENT_STRENGTH 1.f
#define BOID_COHESION_STRENGTH .3f

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define WORLD_WIDTH 12800.0
#define WORLD_HEIGHT 6400.0

constexpr int CELL_SIZE = BOID_PERCEPTION_RADIUS;
constexpr int ROWS = (WORLD_HEIGHT + CELL_SIZE - 1) / CELL_SIZE;
constexpr int COLUMNS = (WORLD_WIDTH + CELL_SIZE - 1) / CELL_SIZE;
constexpr int ESTIMATE_BOID_DISTRIBUTION = (MAX_BOIDS / (ROWS * COLUMNS)) * 4;

using SpatialGrid = std::array<std::array<std::vector<int>, COLUMNS>, ROWS>;

#endif
