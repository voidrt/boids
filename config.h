#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include <array>
#include <vector>

inline constexpr int MAX_BOIDS = 256 * 20;
inline constexpr float BOID_MAX_SPEED = 18.0f;
inline constexpr float BOID_MIN_SPEED = 12.0f;
inline constexpr float BOID_BASE_SIZE = 10.0f;
inline constexpr float BOID_SEPARATION_RADIUS = 90.0f;
inline constexpr float BOID_PERCEPTION_RADIUS = 200.0f;
inline constexpr float BOID_SEPARATION_STRENGTH = 2.8f;
inline constexpr float BOID_ALIGNMENT_STRENGTH = 1.f;
inline constexpr float BOID_COHESION_STRENGTH = .3f;
inline constexpr float SCREEN_WIDTH = 1920.0f;
inline constexpr float SCREEN_HEIGHT = 1080.0f;
inline constexpr float WORLD_WIDTH = 18000.0f;
inline constexpr float WORLD_HEIGHT = 9000.0f;

constexpr int CELL_SIZE = BOID_PERCEPTION_RADIUS;
constexpr int ROWS = (WORLD_HEIGHT + CELL_SIZE - 1) / CELL_SIZE;
constexpr int COLUMNS = (WORLD_WIDTH + CELL_SIZE - 1) / CELL_SIZE;
constexpr int ESTIMATE_BOID_DISTRIBUTION = (MAX_BOIDS / (ROWS * COLUMNS)) * 4;

using SpatialGrid = std::array<std::array<std::vector<int>, COLUMNS>, ROWS>;

#endif
