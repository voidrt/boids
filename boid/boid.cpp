#include "boid.h"
#include <algorithm>

void Boid::DrawBoid(Vector2 velocity, Vector2 position)
{
    Vector2 v1, v2, v3;

    float size = this->size;

    v1 = Vector2Add(Vector2Rotate((Vector2){size, 0.0f}, atan2f(velocity.y, velocity.x)), position);
    v2 = Vector2Add(Vector2Rotate((Vector2){-(size) / 1.5f, -size / 1.5f}, atan2f(velocity.y, velocity.x)), position);
    v3 = Vector2Add(Vector2Rotate((Vector2){-(size) / 1.5f, size / 1.5f}, atan2f(velocity.y, velocity.x)), position);

    DrawTriangle(v1, v2, v3, this->color);
}
