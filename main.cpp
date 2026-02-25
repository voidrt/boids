#include <raylib.h>
#include <raymath.h>
#include "boid/boid.h"
#include "squoid/squoid.h"

#define MAX_BOIDS 1100
#define BOID_SPEED 12.0f
#define BOID_BASE_SIZE 20.0f
#define BOID_SEPARATION_RADIUS 50.0f
#define BOID_PERCEPTION_RADIUS 480.0f
#define BOID_TO_SQUOID_SEPARATION_STRENGTH 120.0f
#define BOID_SEPARATION_STRENGTH 25.0f
#define BOID_ALIGNMENT_STRENGTH 15.5f
#define BOID_COHESION_STRENGTH 7.5f

#define MAX_SQUOIDS 8
#define SQUOID_SPEED 6.0f
#define SQUOID_BASE_SIZE 80.0f
#define SQUOID_PERCEPTION_RADIUS 640.0f
#define SQUOID_SEPARATION_STRENGTH 35.0f

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define WORLD_WIDTH 6000
#define WORLD_HEIGHT 3000

static Boid boidsArray[MAX_BOIDS] = {0};
static Squoid squoidsArray[MAX_SQUOIDS] = {0};
static Camera2D camera = Camera2D();
int selectedBoid = 0;
bool showDebugRadius = false;

void HandleCameraControl(Camera2D &camera)
{
    camera.zoom = Clamp(expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f)), 0.1f, 0.2f);

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
}

void HandleBoidsOnScreenEdge(Boid &boid)
{
    if (boid.position.x >= WORLD_WIDTH)
        boid.position.x = -WORLD_WIDTH;
    else if (boid.position.x <= -WORLD_WIDTH)
        boid.position.x = WORLD_WIDTH;

    if (boid.position.y >= WORLD_HEIGHT)
        boid.position.y = -WORLD_HEIGHT;

    else if (boid.position.y <= -WORLD_HEIGHT)
        boid.position.y = WORLD_HEIGHT;
}

void HandleSquoidsOnScreenEdge(Squoid &squoid)
{
    if (squoid.position.x >= WORLD_WIDTH)
        squoid.position.x = -WORLD_WIDTH;
    else if (squoid.position.x <= -WORLD_WIDTH)
        squoid.position.x = WORLD_WIDTH;

    if (squoid.position.y >= WORLD_HEIGHT)
        squoid.position.y = -WORLD_HEIGHT;

    else if (squoid.position.y <= -WORLD_HEIGHT)
        squoid.position.y = WORLD_HEIGHT;
}

void InitWorld()
{
    camera.offset = (Vector2){(SCREEN_WIDTH / 2.0f) + BOID_BASE_SIZE, (SCREEN_HEIGHT / 2.0f) + BOID_BASE_SIZE};
    camera.target = (Vector2){0, 0};
    camera.zoom = .1f;

    InitWindow(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);
}

void PopulateWorld()
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    float size;

    for (int i = 0; i < MAX_BOIDS; ++i)
    {
        size = GetRandomValue(BOID_BASE_SIZE - 5, BOID_BASE_SIZE + 5);

        positionX = GetRandomValue(-WORLD_WIDTH, WORLD_WIDTH);
        positionY = GetRandomValue(-WORLD_HEIGHT, WORLD_HEIGHT);

        velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);

        while (velocityX == 0 && velocityY == 0)
        {
            velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
            velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        }

        boidsArray[i].position = (Vector2){positionX, positionY};
        boidsArray[i].velocity = (Vector2){velocityX, velocityY};
        boidsArray[i].maxSpeed = BOID_SPEED;
        boidsArray[i].squoidSeparationStrength = BOID_TO_SQUOID_SEPARATION_STRENGTH;
        boidsArray[i].separationStrength = BOID_SEPARATION_STRENGTH;
        boidsArray[i].alignmentStrength = BOID_ALIGNMENT_STRENGTH;
        boidsArray[i].cohesionStrength = BOID_COHESION_STRENGTH;
        boidsArray[i].perceptionRadius = BOID_PERCEPTION_RADIUS;
        boidsArray[i].boidSize = size;
        boidsArray[i].repelRadius = BOID_SEPARATION_RADIUS + size;
        boidsArray[i].id = i;
        boidsArray[i].color = (Color){(unsigned char)(GetRandomValue(50, 255)), (unsigned char)(GetRandomValue(50, 255)), (unsigned char)(GetRandomValue(50, 255)), 255};
    }

    for (int j = 0; j < MAX_SQUOIDS; ++j)
    {
        positionX = GetRandomValue(-WORLD_WIDTH, WORLD_WIDTH);
        positionY = GetRandomValue(-WORLD_HEIGHT, WORLD_HEIGHT);

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
        squoidsArray[j].id = j + MAX_BOIDS;
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

        v1 = Vector2Add(Vector2Rotate((Vector2){boidsArray[i].boidSize, 0.0f}, boidsArray[i].rotation), boidsArray[i].position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(boidsArray[i].boidSize) / 1.5f, -boidsArray[i].boidSize / 1.5f}, boidsArray[i].rotation), boidsArray[i].position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(boidsArray[i].boidSize) / 1.5f, boidsArray[i].boidSize / 1.5f}, boidsArray[i].rotation), boidsArray[i].position);

        if (boidsArray[i].id == selectedBoid && showDebugRadius)
        {
            DrawCircleV(boidsArray[i].position, boidsArray[i].perceptionRadius, {200, 151, 55, 200});
            DrawCircleV(boidsArray[i].position, boidsArray[i].repelRadius, {231, 41, 55, 200});

            DrawTriangle(v1, v2, v3, boidsArray[i].color);
        }
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
            squoidsArray[i].squoidRadius * 2.f,
            squoidsArray[i].squoidRadius / 1.5f,
        };

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
            HandleCameraControl(camera);

            if (IsKeyPressed(KEY_G))
                ++selectedBoid;
            else if (IsKeyPressed(KEY_B))
                showDebugRadius = !showDebugRadius;

            BeginDrawing();

            ClearBackground((Color){2, 2, 20, 255});

            BeginMode2D(camera);

            DrawEntities();
            DrawText(TextFormat("FPS: %d", GetFPS()), -WORLD_WIDTH, -WORLD_HEIGHT - 500, 200, RAYWHITE);

            EndMode2D();
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
