using namespace std;

#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include "boid/boid.h"
#include "squoid/squoid.h"

#define MAX_BOIDS 1000
#define BOID_SPEED 18.0f
#define BOID_BASE_SIZE 20.0f
#define BOID_SEPARATION_RADIUS 40.0f
#define BOID_PERCEPTION_RADIUS 340.0f
#define BOID_TO_SQUOID_SEPARATION_STRENGTH 60.0f
#define BOID_SEPARATION_STRENGTH 35.0f
#define BOID_ALIGNMENT_STRENGTH 15.5f
#define BOID_COHESION_STRENGTH 17.5f

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define WORLD_WIDTH 6000
#define WORLD_HEIGHT 3000

static unordered_map<string, vector<int>> worldGrid;
static float cellSize = BOID_PERCEPTION_RADIUS;

static Boid boidsArray[MAX_BOIDS] = {0};
static Camera2D camera = Camera2D();
static int selectedBoid;
static bool showDebugRadius;
static bool showWorldGrid;
static bool isPausedSimulation;

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

    if (IsKeyPressed(KEY_G))
        ++selectedBoid;
    if (IsKeyPressed(KEY_B))
        showDebugRadius = !showDebugRadius;
    if (IsKeyPressed(KEY_H))
        showWorldGrid = !showWorldGrid;
}

void InitWorld(void)
{
    camera.offset = (Vector2){(SCREEN_WIDTH / 2.0f) + BOID_BASE_SIZE, (SCREEN_HEIGHT / 2.0f) + BOID_BASE_SIZE};
    camera.target = (Vector2){0, 0};
    camera.zoom = 1.f;

    selectedBoid = 0;
    showDebugRadius = false;
    showWorldGrid = true;
    isPausedSimulation = true;

    InitWindow(static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);

    worldGrid = {};
}

void PopulateWorld(void)
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

        int cellIndexX = (int)floor(boidsArray[i].position.x / boidsArray[i].perceptionRadius);
        int cellIndexY = (int)floor(boidsArray[i].position.y / boidsArray[i].perceptionRadius);

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
        boidsArray[i].size = size;
        boidsArray[i].repelRadius = BOID_SEPARATION_RADIUS + size;
        boidsArray[i].id = i;
        boidsArray[i].color = (Color){(unsigned char)(GetRandomValue(50, 255)), (unsigned char)(GetRandomValue(50, 255)), (unsigned char)(GetRandomValue(50, 255)), 255};
    }
}

void UpdateGame(void)
{
    worldGrid.clear();

    for (int i = 0; i < MAX_BOIDS; i++)
    {
        if (!boidsArray[i].isAlive)
        {
            continue;
        }

        int cellIndexX = (int)floor(boidsArray[i].position.x / boidsArray[i].perceptionRadius);
        int cellIndexY = (int)floor(boidsArray[i].position.y / boidsArray[i].perceptionRadius);

        string cellHash = to_string(cellIndexX) + "," + to_string(cellIndexY);

        worldGrid[cellHash].push_back(i);
    }

    for (int i = 0; i < MAX_BOIDS; i++)
    {
        if (!boidsArray[i].isAlive)
        {
            continue;
        }
        boidsArray[i].SteerBoid(boidsArray, MAX_BOIDS, worldGrid);
    }
    for (int i = 0; i < MAX_BOIDS; i++)
    {
        boidsArray[i].MoveBoid(WORLD_WIDTH, WORLD_HEIGHT);
    }
}

void DrawGame(void)
{
    BeginDrawing();
    ClearBackground((Color){2, 2, 20, 255});
    BeginMode2D(camera);

    DrawText(TextFormat("FPS: %d", GetFPS()), -WORLD_WIDTH, -WORLD_HEIGHT - 500, 200, RAYWHITE);

    if (showWorldGrid)
    {
        for (const auto &cell : worldGrid)
        {
            string cellName = cell.first;
            const vector<int> &boidIDs = cell.second;

            size_t commaPosition = cellName.find(',');
            int gridX = std::stoi(cellName.substr(0, commaPosition));
            int gridY = std::stoi(cellName.substr(commaPosition + 1));
            Vector2 cellPosition = (Vector2){gridX * BOID_PERCEPTION_RADIUS, gridY * BOID_PERCEPTION_RADIUS};

            std::cout << gridX << ' ' << gridY << endl;

            Rectangle cellRect = (Rectangle){cellPosition.x, cellPosition.y, cellSize, cellSize};

            DrawRectangleLinesEx(cellRect, 12, RAYWHITE);
        }
    }

    for (int i = 0; i < MAX_BOIDS; ++i)
    {
        if (!boidsArray[i].isAlive)
            continue;

        Vector2 v1, v2, v3;
        v1 = Vector2Add(Vector2Rotate((Vector2){boidsArray[i].size, 0.0f}, boidsArray[i].rotation), boidsArray[i].position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(boidsArray[i].size) / 2.0f, boidsArray[i].size / 1.5f}, boidsArray[i].rotation), boidsArray[i].position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(boidsArray[i].size) / 2.0f, -boidsArray[i].size / 1.5f}, boidsArray[i].rotation), boidsArray[i].position);

        if (boidsArray[i].id == selectedBoid && showDebugRadius)
        {
            DrawCircleV(boidsArray[i].position, boidsArray[i].perceptionRadius * 2, {30, 241, 35, 200});

            DrawCircleV(boidsArray[i].position, boidsArray[i].perceptionRadius, {200, 151, 55, 200});

            DrawCircleV(boidsArray[i].position, boidsArray[i].repelRadius, {231, 41, 55, 200});

            DrawTriangle(v1, v2, v3, boidsArray[i].color);
        }
        else
        {
            DrawTriangle(v1, v2, v3, boidsArray[i].color);
        }
    }

    EndMode2D();
    EndDrawing();
}

void UpdateAndDrawGame(void)
{
    UpdateGame();
    DrawGame();
}

int main()
{
    InitWorld();
    PopulateWorld();

    while (!WindowShouldClose())
    {
        HandleCameraControl(camera);
        UpdateAndDrawGame();
    }

    CloseWindow();

    return 0;
}
