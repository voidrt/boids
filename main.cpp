#include <raylib.h>
#include <raymath.h>
#include "boid.h"
#include <iostream>
#include <map>


#define MAX_BOIDS 300
#define BOID_BASE_SIZE 10.0f
#define BOID_SPEED 5.0f
#define BOID_REPEL_STRENGTH 5.0f
#define BOID_REPEL_RADIUS 60.0f

static Boid boidsArray[MAX_BOIDS] = {0};
static Camera2D camera = {0};
static float screenWidth = 1100.0f;
static float screenHeight = 600.0f;

void HandleCameraControl(Camera2D &camera)
{

    // Rotation
    if (IsKeyDown(KEY_A))
        camera.rotation -= 2.0f;
    if (IsKeyDown(KEY_D))
        camera.rotation += 2.0f;

    // Zoom
    if (IsKeyPressed(KEY_W))
        camera.zoom += .1f;
    if (IsKeyPressed(KEY_S))
        camera.zoom -= .1f;
    // Pan
    if (IsKeyDown(KEY_RIGHT)) {
        camera.offset.x -= 2.0f;
    } 
    if (IsKeyDown(KEY_LEFT)) {
        camera.offset.x += 2.0f;
    }
    if (IsKeyDown(KEY_UP)) {
        camera.offset.y += 2.0f;
    } 
    if (IsKeyDown(KEY_DOWN)) {
        camera.offset.y -= 2.0f;
    }
    


    if (camera.zoom > 10.0f)
    {
        camera.zoom = 10.0f;
    }
    else if (camera.zoom < 0.2f)
    {
        camera.zoom = 0.2f;
    }
}

void HandleBoidsOnScreenEdge(Boid &boid) {
    if (boid.position.x > 900)
            boid.position.x = -900;
        else if (boid.position.x < -900)
            boid.position.x = 900;

        if (boid.position.y > 900)
            boid.position.y = -900;

        else if (boid.position.y < -900)
            boid.position.y = 900;
}

void InitWorld()
{
    camera.offset = (Vector2){900/2.0f, 900/2.0f};
    camera.target = (Vector2){0,0};
    camera.rotation = 0;
    camera.zoom = 1.0f;

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

        positionX = GetRandomValue(-900, 900);
        positionY = GetRandomValue(-900, 900);

        velocityX = GetRandomValue(-BOID_SPEED, BOID_SPEED);
        velocityY = GetRandomValue(-BOID_SPEED, BOID_SPEED);

        while (fabs(velocityX) != BOID_SPEED && fabs(velocityY) != BOID_SPEED)
        {

            velocityX = GetRandomValue(-BOID_SPEED, BOID_SPEED);
            velocityY = GetRandomValue(-BOID_SPEED, BOID_SPEED);
        }

        boidsArray[i].position = (Vector2){positionX, positionY};
        boidsArray[i].velocity = (Vector2){velocityX, velocityY};
        boidsArray[i].maxSpeed = BOID_SPEED;
        boidsArray[i].repelRadius = BOID_REPEL_RADIUS;
        boidsArray[i].repelStrength = BOID_REPEL_STRENGTH;
        boidsArray[i].boidRadius = BOID_BASE_SIZE;
        boidsArray[i].id = i;
        boidsArray[i].color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
    }
}

void DrawWorld()
{
    //!! ---- DRAWING
    BeginDrawing();
    ClearBackground((Color){10, 10, 20, 100});
    BeginMode2D(camera);
 
    for (int i = 0; i < MAX_BOIDS - 1; ++i)
    {
        DrawCircleV(boidsArray[i].position, boidsArray[i].boidRadius, boidsArray[i].color);
        
        boidsArray[i].position.x += boidsArray[i].velocity.x;
        boidsArray[i].position.y += boidsArray[i].velocity.y;

        boidsArray[i].moveBoid(boidsArray, MAX_BOIDS);

        HandleBoidsOnScreenEdge(boidsArray[i]);
        
    }

    EndMode2D();
    EndDrawing();
    //!! ---- END DRAWING
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
        }
    }

    CloseWindow();
    return 0;
}