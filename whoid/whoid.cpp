#include "whoid.h"
#include <raymath.h>

void Whoid::MoveWhoid()
{
    while (this->position.x <= 0.0f || this->position.x >= WORLD_WIDTH)
    {
        this->velocity.x = -this->velocity.x;
    }
  
    while (this->position.y <= 0.0f || this->position.y >= WORLD_HEIGHT)
    {
        this->velocity.y = -this->velocity.y;
    }
  
    this->position += this->velocity;
    this->rotation = atan2f(this->velocity.y, this->velocity.x) * RAD2DEG;
}

void Whoid::SteerWhoid(const SpatialGrid &worldGrid, std::array<Whoid, MAX_WHOIDS> whoidGroup)
{
}