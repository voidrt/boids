using namespace std;
#include <raylib.h>
#include "../squoid/squoid.h"
#include <unordered_map>
#include <string>
#include <cmath>
#include <vector>

#ifndef BOID_H
#define BOID_H

struct Boid
{
    bool isAlive = true;

    Vector2 position;
    Vector2 velocity;
    float rotation;
    float maxSpeed;
    float squoidSeparationStrength;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float repelRadius;
    float perceptionRadius;
    float size;
    int id;
    Color color;

    void SteerBoid(const Boid flock[], const int boidCount, const Squoid squoids[], int squoidCount, const unordered_map<string, vector<int>> &worldGrid);

    void MoveBoid();
};

#endif