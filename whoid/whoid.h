#ifndef WHOID_H
#define WHOID_H

#include "../config.h"
#include <raylib.h>

struct Whoid {
    Vector2 position;
    Vector2 velocity;
    int identifier;
    float rotation;
    
    void MoveWhoid();
    void SteerWhoid(const SpatialGrid &worldGrid, std::array<Whoid, MAX_WHOIDS> whoidGroup);
};

#endif
