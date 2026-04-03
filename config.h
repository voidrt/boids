#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

inline constexpr int MAX_BOIDS = 64 * 480;
inline constexpr float BOID_MAX_SPEED = 18.0f;
inline constexpr float BOID_MIN_SPEED = 12.0f;
inline constexpr float BOID_BASE_SIZE = 10.0f;
inline constexpr float BOID_SEPARATION_RADIUS = 90.0f;
inline constexpr float BOID_PERCEPTION_RADIUS = 200.0f;
inline constexpr float BOID_SEPARATION_STRENGTH = 4.8f;
inline constexpr float BOID_ALIGNMENT_STRENGTH = 1.f;
inline constexpr float BOID_COHESION_STRENGTH = .7f;
inline constexpr float SCREEN_WIDTH = 1920.0f;
inline constexpr float SCREEN_HEIGHT = 1080.0f;
inline constexpr float WORLD_WIDTH = 27000.0f;
inline constexpr float WORLD_HEIGHT = 13500.0f;

#endif
