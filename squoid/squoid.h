#include <raylib.h>

#ifndef SQUOID_H
#define SQUOID_H

struct Squoid
{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float maxSpeed;
    float perceptionRadius;
    float squoidRadius;
    int id;
    Color color;

    void MoveSquoid(const Squoid squoids[], int squoidCount);
};

#endif
