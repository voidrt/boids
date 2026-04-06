#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

inline constexpr int MAX_BOIDS = 64 * 480;
inline constexpr float BOID_MAX_SPEED = 0.2f;
inline constexpr float BOID_MIN_SPEED = 0.14f;
inline constexpr float BOID_BASE_SIZE = 3.0f;
inline constexpr float SCALE = 0.1;
inline constexpr float BOID_SEPARATION_RADIUS = 4.0f;
inline constexpr float BOID_PERCEPTION_RADIUS = 8.0f;
inline constexpr float BOID_SEPARATION_STRENGTH = 4.8f;
inline constexpr float BOID_ALIGNMENT_STRENGTH = 1.5f;
inline constexpr float BOID_COHESION_STRENGTH = .5f;
inline constexpr float SCREEN_WIDTH = 1920.0f;
inline constexpr float SCREEN_HEIGHT = 1080.0f;
inline constexpr float WORLD_WIDTH = 80.0f;
inline constexpr float WORLD_DEPTH = 80.0f;
inline constexpr float WORLD_HEIGHT = 80.5f;

#endif
