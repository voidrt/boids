#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include <array>
#include <vector>

#define MAX_BOIDS 5800
#define BOID_SPEED 18
#define BOID_BASE_SIZE 20
#define BOID_SEPARATION_RADIUS 50
#define BOID_PERCEPTION_RADIUS 360
#define BOID_SEPARATION_STRENGTH 1.4f
#define BOID_ALIGNMENT_STRENGTH 1.f
#define BOID_COHESION_STRENGTH 1.f

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
