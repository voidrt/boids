#include <raylib.h>
#include <raymath.h>
#include "boid/boid.h"
#include "squoid/squoid.h"

#define MAX_BOIDS 1000
#define BOID_SPEED 12.0f
#define BOID_BASE_SIZE 20.0f
#define BOID_SEPARATION_RADIUS 80.0f
#define BOID_PERCEPTION_RADIUS 150.0f
#define BOID_SEPARATION_STRENGTH 10.0f
#define BOID_TO_SQUOID_SEPARATION_STRENGTH 50.0f
#define BOID_ALIGNMENT_STRENGTH 35.5f
#define BOID_COHESION_STRENGTH 15.0f

#define MAX_SQUOIDS 10
#define SQUOID_SPEED 4.0f
#define SQUOID_BASE_SIZE 80.0f
#define SQUOID_PERCEPTION_RADIUS 640.0f
#define SQUOID_SEPARATION_STRENGTH 15.0f

static Boid boidsArray[MAX_BOIDS] = {0};
static Squoid squoidsArray[MAX_SQUOIDS] = {0};
static Camera2D camera = Camera2D();
static float screenWidth = 1366.0f;
static float screenHeight = 768.0f;
static float worldWidth = 6000;
static float worldHeight = 3000;

void HandleCameraControl(Camera2D &camera)
{

    if (IsKeyDown(KEY_RIGHT))
        camera.rotation -= 2.0f;
    if (IsKeyDown(KEY_LEFT))
        camera.rotation += 2.0f;

    camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

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
    else if (camera.zoom < 0.1f)
    {
        camera.zoom = 0.1f;
    }
}

void HandleBoidsOnScreenEdge(Boid &boid)
{
    if (boid.position.x > worldWidth)
        boid.position.x = -worldWidth;
    else if (boid.position.x < -worldWidth)
        boid.position.x = worldWidth;

    if (boid.position.y > worldHeight)
        boid.position.y = -worldHeight;

    else if (boid.position.y < -worldHeight)
        boid.position.y = worldHeight;
}

void HandleSquoidsOnScreenEdge(Squoid &squoid)
{
    if (squoid.position.x > worldWidth)
        squoid.position.x = -worldWidth;
    else if (squoid.position.x < -worldWidth)
        squoid.position.x = worldWidth;

    if (squoid.position.y > worldHeight)
        squoid.position.y = -worldHeight;

    else if (squoid.position.y < -worldHeight)
        squoid.position.y = worldHeight;
}

void InitWorld()
{
    camera.offset = (Vector2){(screenWidth / 2.0f) + BOID_BASE_SIZE, (screenHeight / 2.0f) + BOID_BASE_SIZE};
    camera.target = (Vector2){0, 0};
    camera.rotation = 0;
    camera.zoom = .1f;
    InitWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "Boids");
    SetTargetFPS(60);
}

void PopulateWorld()
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;

    for (int i = 0; i < MAX_BOIDS; ++i)
    {

        positionX = GetRandomValue(-worldWidth, worldWidth);
        positionY = GetRandomValue(-worldHeight, worldHeight);

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
        boidsArray[i].separationStrength = BOID_SEPARATION_STRENGTH;
        boidsArray[i].squoidSeparationStrength = BOID_TO_SQUOID_SEPARATION_STRENGTH;
        boidsArray[i].alignmentStrength = BOID_ALIGNMENT_STRENGTH;
        boidsArray[i].cohesionStrength = BOID_COHESION_STRENGTH;
        boidsArray[i].repelRadius = BOID_SEPARATION_RADIUS;
        boidsArray[i].perceptionRadius = BOID_PERCEPTION_RADIUS;
        boidsArray[i].boidRadius = BOID_BASE_SIZE;
        boidsArray[i].id = i;
        boidsArray[i].isAlive = true;
        boidsArray[i].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
    }

    for (int j = 0; j < MAX_SQUOIDS; ++j)
    {
        positionX = GetRandomValue(-worldWidth, worldWidth);
        positionY = GetRandomValue(-worldHeight, worldHeight);

        velocityX = GetRandomValue(-(SQUOID_SPEED), SQUOID_SPEED);
        velocityY = GetRandomValue(-(SQUOID_SPEED), SQUOID_SPEED);

        while (velocityX == 0.f && velocityY == 0.f)
        {
            velocityX = GetRandomValue(-SQUOID_SPEED, SQUOID_SPEED);
            velocityY = GetRandomValue(-SQUOID_SPEED, SQUOID_SPEED);
        }

        squoidsArray[j].position = (Vector2){positionX, positionY};
        squoidsArray[j].velocity = (Vector2){velocityX, velocityY};
        squoidsArray[j].maxSpeed = SQUOID_SPEED;
        squoidsArray[j].perceptionRadius = SQUOID_PERCEPTION_RADIUS;
        squoidsArray[j].squoidRadius = SQUOID_BASE_SIZE;
        squoidsArray[j].separationStrength = SQUOID_SEPARATION_STRENGTH;
        squoidsArray[j].rotation = 0;
        squoidsArray[j].id = j + 101;
        squoidsArray[j].color = (Color){93, 110, 126, 255};
    }
}

void DrawEntities()
{

    for (int i = 0; i < MAX_BOIDS - 1; ++i)
    {
        if (!boidsArray[i].isAlive)
        {
            continue;
        }
        Vector2 v1, v2, v3;

        v1 = Vector2Add(Vector2Rotate((Vector2){BOID_BASE_SIZE, 0.0f}, boidsArray[i].rotation), boidsArray[i].position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(BOID_BASE_SIZE) / 2, -BOID_BASE_SIZE / 3}, boidsArray[i].rotation), boidsArray[i].position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(BOID_BASE_SIZE) / 2, BOID_BASE_SIZE / 3}, boidsArray[i].rotation), boidsArray[i].position);

        DrawTriangle(v1, v2, v3, boidsArray[i].color);

        boidsArray[i].SteerBoid(boidsArray, MAX_BOIDS, squoidsArray, MAX_SQUOIDS);

        HandleBoidsOnScreenEdge(boidsArray[i]);
    }

    for (int i = 0; i < MAX_SQUOIDS; ++i)
    {
        squoidsArray[i].MoveSquoid(squoidsArray, MAX_SQUOIDS);

        Rectangle squoidBody = {
            squoidsArray[i].position.x,
            squoidsArray[i].position.y,
            squoidsArray[i].squoidRadius * 2,
            squoidsArray[i].squoidRadius / 1.5};

        DrawRectanglePro(squoidBody, (Vector2){squoidBody.width / 2, squoidBody.height / 2}, squoidsArray[i].rotation, squoidsArray[i].color);

        HandleSquoidsOnScreenEdge(squoidsArray[i]);
    }
}

int main()
{
    InitWorld();
    PopulateWorld();

    if (IsWindowReady())
    {
        while (!WindowShouldClose())
        {
            int deadBoids = 0;
            HandleCameraControl(camera);
            for (int d = 0; d < MAX_BOIDS; ++d)
            {
                if (!boidsArray[d].isAlive)
                {
                    deadBoids++;
                }
            }

            if (IsKeyDown(KEY_G))
                camera.target = boidsArray[0].position;
            else if (IsKeyDown(KEY_B))
                camera.target = squoidsArray[0].position;

            BeginDrawing();

            ClearBackground((Color){2, 2, 20, 255});

            BeginMode2D(camera);

            DrawEntities();
            DrawText(TextFormat("Boids currently alive: %02i", (MAX_BOIDS - deadBoids)), -worldWidth, -worldHeight - 500, 200, RAYWHITE);

            EndMode2D();
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
