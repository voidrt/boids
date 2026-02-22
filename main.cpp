#include <raylib.h>
#include <raymath.h>
#include "boid.h"

#define MAX_BOIDS 100
#define BOID_BASE_SIZE 10.0f
#define BOID_SPEED 5.0f
#define BOID_REPEL_STRENGTH 2.0f
#define BOID_REPEL_RADIUS 20.0f

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
    if (boid.position.x > screenWidth * 2.0f)
            boid.position.x = -(screenHeight / 2);
        else if (boid.position.x < -(screenWidth / 2))
            boid.position.x = (screenWidth * 2.0f);

        if (boid.position.y > screenHeight * 2.0f)
            boid.position.y = -(screenHeight / 2);

        else if (boid.position.y < -(screenHeight / 2))
            boid.position.y = (screenHeight * 2.0f);
}

void InitWorld()
{
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.target = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
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

    for (int i = 0; i < MAX_BOIDS; ++i)
    {

        positionX = GetRandomValue(-(screenWidth / 2), screenWidth * 2);
        positionY = GetRandomValue(-(screenHeight / 2), screenHeight * 2);

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
        boidsArray[i].id = i;
        boidsArray[i].repelStrength = BOID_REPEL_STRENGTH;
        boidsArray[i].boidRadius = BOID_BASE_SIZE;
    }
}

void DrawWorld()
{
    //!! ---- DRAWING
    BeginDrawing();
    ClearBackground((Color){10, 10, 20, 100});
    BeginMode2D(camera);
 
    for (int i = 0; i < MAX_BOIDS; ++i)
    {
        DrawCircleV(boidsArray[i].position, boidsArray[i].boidRadius, RED);
        
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