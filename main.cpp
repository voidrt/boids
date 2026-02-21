#include <raylib.h>
#include <raymath.h>
#include "boid.h"

#define MAX_BOIDS 50
#define BOID_BASE_SIZE 10.0f
#define BOID_SPEED 5.0f
#define BOID_STEER_STRENGHT 1.0f

static Boid boidsArray[MAX_BOIDS] = {0};
static Camera2D camera = {0};
static float screenWidth = 800.0f;
static float screenHeight = 550.0f;

void HandleCameraControl(Camera2D &camera)
{
    // Rotation
    if (IsKeyDown(KEY_A))
        camera.rotation -= 2.0f;
    if (IsKeyDown(KEY_D))
        camera.rotation += 2.0f;

    // Zoom
    if (IsKeyDown(KEY_W))
        camera.zoom += 0.1f;
    if (IsKeyDown(KEY_S))
        camera.zoom -= 0.1f;

    if (camera.zoom > 6.0f)
    {
        camera.zoom = 6.0f;
    }
    else if (camera.zoom < 0.2f)
    {
        camera.zoom = 0.2f;
    }
}

void initWorld()
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    bool isAlive = false;

    for (int i = 0; i < MAX_BOIDS; ++i)
    {
        positionX = GetRandomValue(0, screenWidth);
        positionY = GetRandomValue(0, screenHeight);

        velocityX = GetRandomValue(-BOID_SPEED, BOID_SPEED);
        velocityY = GetRandomValue(-BOID_SPEED, BOID_SPEED);

        while (!isAlive)
        {
            if (velocityX < BOID_SPEED / 2 && velocityY < BOID_SPEED / 2)
            {
                velocityX = GetRandomValue(-BOID_SPEED, BOID_SPEED);
                velocityY = GetRandomValue(-BOID_SPEED, BOID_SPEED);
            }
            else
            {
                isAlive = true;
            }
        }

        boidsArray[i].position = (Vector2){positionX, positionY};
        boidsArray[i].velocity = (Vector2){velocityX, velocityY};
    }
}

void drawWorld()
{
    //!! ---- DRAWING
    BeginDrawing();
    ClearBackground((Color){10, 10, 20, 100});
    BeginMode2D(camera);
    for (int i = 0; i < MAX_BOIDS; i++)
    {
        DrawCircleV(boidsArray[i].position, GetRandomValue(BOID_BASE_SIZE - 2, BOID_BASE_SIZE + 2), RED);
    }

    for (int i = 0; i < MAX_BOIDS; ++i)
    {
        boidsArray[i].position.x += boidsArray[i].velocity.x;
        boidsArray[i].position.y += boidsArray[i].velocity.y;
        boidsArray[i].maxSpeed = BOID_SPEED;
        boidsArray[i].steerStrenght = BOID_STEER_STRENGHT;

        boidsArray[i].moveBoid();
    }

    EndMode2D();
    EndDrawing();
    //!! ---- END DRAWING
}

int main()
{

    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.target = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0;
    camera.zoom = 1.0f;

    InitWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "Boids");
    SetTargetFPS(60);

    initWorld();

    if (IsWindowReady())
    {
        while (!WindowShouldClose())
        {
            HandleCameraControl(camera);
            drawWorld();
        }
    }

    CloseWindow();
    return 0;
}