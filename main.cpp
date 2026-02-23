#include <raylib.h>
#include <raymath.h>
#include "boid/boid.h"
#include <iostream>
#include <map>

#define MAX_BOIDS 500
#define BOID_SPEED 4.0f
#define BOID_BASE_SIZE 30.0f
#define BOID_SEPARATION_RADIUS 80.0f
#define BOID_PERCEPTION_RADIUS 150.0f
#define BOID_SEPARATION_STRENGTH 1000.0f
#define BOID_ALIGNMENT_STRENGTH 5.0f
#define BOID_COHESION_STRENGTH 10.5f

static Boid boidsArray[MAX_BOIDS] = {0};
static Camera2D camera = Camera2D();
static float screenWidth = 1100.0f;
static float screenHeight = 600.0f;

void HandleCameraControl(Camera2D &camera)
{

    if (IsKeyDown(KEY_RIGHT))
        camera.rotation -= 2.0f;
    if (IsKeyDown(KEY_LEFT))
        camera.rotation += 2.0f;

    camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.5f));

    if (IsKeyDown(KEY_D))
    {
        camera.offset.x -= 10.0f;
    }
    if (IsKeyDown(KEY_A))
    {
        camera.offset.x += 10.0f;
    }
    if (IsKeyDown(KEY_W))
    {
        camera.offset.y += 10.0f;
    }
    if (IsKeyDown(KEY_S))
    {
        camera.offset.y -= 10.0f;
    }

    if (camera.zoom > 10.0f)
    {
        camera.zoom = 10.0f;
    }
    else if (camera.zoom < 0.07f)
    {
        camera.zoom = 0.07f;
    }
}

void HandleBoidsOnScreenEdge(Boid &boid)
{
    if (boid.position.x > 3000)
        boid.position.x = -3000;
    else if (boid.position.x < -3000)
        boid.position.x = 3000;

    if (boid.position.y > 1500)
        boid.position.y = -1500;

    else if (boid.position.y < -1500)
        boid.position.y = 1500;
}

void InitWorld()
{
    camera.offset = (Vector2){(screenWidth / 2.0f) + BOID_BASE_SIZE, (screenHeight / 2.0f) + BOID_BASE_SIZE};
    camera.target = (Vector2){0, 0};
    camera.rotation = 0;
    camera.zoom = 1.f;
    InitWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "Boids");
    SetTargetFPS(60);
}

void PopulateWorld()
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    bool isAlive = false;

    for (int i = 0; i < MAX_BOIDS - 1; ++i)
    {

        positionX = GetRandomValue(-3000, 3000);
        positionY = GetRandomValue(-1500, 1500);

        velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);

        while (velocityX == 0 && velocityY == 0)
        {

            velocityX = GetRandomValue(-BOID_SPEED, BOID_SPEED);
            velocityY = GetRandomValue(-BOID_SPEED, BOID_SPEED);
        }

        boidsArray[i].position = (Vector2){positionX, positionY};
        boidsArray[i].velocity = (Vector2){velocityX, velocityY};
        boidsArray[i].maxSpeed = BOID_SPEED;
        boidsArray[i].repelStrength = BOID_SEPARATION_STRENGTH;
        boidsArray[i].alignmentStrength = BOID_ALIGNMENT_STRENGTH;
        boidsArray[i].cohesionStrength = BOID_COHESION_STRENGTH;
        boidsArray[i].repelRadius = BOID_SEPARATION_RADIUS;
        boidsArray[i].perceptionRadius = BOID_PERCEPTION_RADIUS;
        boidsArray[i].boidRadius = BOID_BASE_SIZE;
        boidsArray[i].id = i;
        boidsArray[i].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
    }
}

void DrawWorld()
{

    BeginDrawing();

    ClearBackground((Color){2, 2, 20, 255});

    BeginMode2D(camera);

    for (int i = 0; i < MAX_BOIDS - 1; ++i)
    {

        float facingAngle = atan2f(boidsArray[i].velocity.y, boidsArray[i].velocity.x);

        Vector2 v1, v2, v3;

        v1 = Vector2Add(Vector2Rotate((Vector2){BOID_BASE_SIZE, 0.0f}, facingAngle), boidsArray[i].position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(BOID_BASE_SIZE) / 2, -BOID_BASE_SIZE / 3}, facingAngle), boidsArray[i].position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(BOID_BASE_SIZE) / 2, BOID_BASE_SIZE / 3}, facingAngle), boidsArray[i].position);

        DrawTriangle(v1, v2, v3, boidsArray[i].color);

        boidsArray[i].position.x += boidsArray[i].velocity.x;
        boidsArray[i].position.y += boidsArray[i].velocity.y;

        boidsArray[i].moveBoid(boidsArray, MAX_BOIDS);

        HandleBoidsOnScreenEdge(boidsArray[i]);
    }

    EndMode2D();
    EndDrawing();
}

int main()
{
    InitWorld();
    PopulateWorld();

    if (IsWindowReady())
    {
        while (!WindowShouldClose())
        {
            HandleCameraControl(camera);
            DrawWorld();
            if (IsKeyDown(KEY_G))
            {
                camera.target = boidsArray[0].position;
            }
        }
    }

    CloseWindow();
    return 0;
}