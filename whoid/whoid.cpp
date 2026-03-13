#include "whoid.h"
#include <raymath.h>

void Whoid::DrawWhoid()
{
    Rectangle whoidBody = {
        this->position.x,
        this->position.y,
        WHOID_SIZE,
        WHOID_SIZE / 3,
    };
    DrawRectanglePro(whoidBody, (Vector2){whoidBody.width / 2, whoidBody.height / 2}, this->rotation, {100, 110, 147, 255});
}

void Whoid::MoveWhoid()
{
    this->position += this->velocity;
    this->rotation = atan2f(this->velocity.y, this->velocity.x) * RAD2DEG;
}

void Whoid::SteerWhoid(const SpatialGrid &worldGrid, std::array<Whoid, MAX_WHOIDS> whoidGroup)
{
}