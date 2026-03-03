#include "boid/boid.h"
#include "constants.h"
#include <raymath.h>
#include <algorithm>
#include <iostream>

static std::array<Boid, MAX_BOIDS> boidsArray = {0};
static Camera2D camera = Camera2D();
int debugSelectedBoid = 14;
bool showDebugRadius = false;
bool showDebugGrid = true;

SpatialGrid worldGrid;

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
        ++debugSelectedBoid;
    if (IsKeyPressed(KEY_B))
        showDebugRadius = !showDebugRadius;
    if (IsKeyPressed(KEY_H))
        showDebugGrid = !showDebugGrid;
}

void InitWorld(void)
{
    camera.offset = (Vector2){(SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f)};
    camera.target = (Vector2){WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
    camera.zoom = 0.1f;

    InitWindow((SCREEN_WIDTH), (SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);
}

void PopulateWorld(void)
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    int size;

    for (auto &row : worldGrid)
    {
        for (auto &cell : row)
        {
            cell.reserve(ESTIMATE_BOID_DISTRIBUTION);
        }
    }

    for (Boid &boid : boidsArray)
    {
        size = GetRandomValue(BOID_BASE_SIZE - 5, BOID_BASE_SIZE + 5);

        positionX = GetRandomValue(0, WORLD_WIDTH);
        positionY = GetRandomValue(0, WORLD_HEIGHT);

        velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);

        boid.position = (Vector2){positionX, positionY};
        boid.velocity = (Vector2){velocityX, velocityY};
        boid.size = size;
        boid.identifier = &boid - &boidsArray[0];
        boid.color = (Color){(unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), 255};
    }
}

void UpdateGame(void)
{
    for (auto &row : worldGrid)
    {
        for (auto &cell : row)
        {
            cell.clear();
        }
    }

    for (Boid &boid : boidsArray)
    {
        int currentCell = (boid.position.x / CELL_SIZE);
        int currentRow = (boid.position.y / CELL_SIZE);

        currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
        currentRow = std::clamp(currentRow, 0, ROWS - 1);

        worldGrid[currentRow][currentCell].push_back(boid.identifier);
    }

    for (Boid &boid : boidsArray)
    {
        boid.UpdateVelocity(boidsArray, worldGrid);
    }

    for (Boid &boid : boidsArray)
    {
        boid.UpdatePosition();
    }
}

void DrawFrame(void)
{
    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground((Color){5, 5, 10, 255});

    DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 200, RAYWHITE);

    if (showDebugGrid)
    {
        for (int y = 0; y < ROWS; y++)
        {
            for (int x = 0; x < COLUMNS; x++)
            {
                float currentCellX = x * BOID_PERCEPTION_RADIUS;
                float currentCellY = y * BOID_PERCEPTION_RADIUS;

                Rectangle cellRect = {currentCellX, currentCellY, BOID_PERCEPTION_RADIUS, BOID_PERCEPTION_RADIUS};

                DrawRectangleLinesEx(cellRect, 5, RAYWHITE);
            }
        }
    }

    if (showDebugGrid)
    {
        Boid &mainBoid = boidsArray[debugSelectedBoid];

        int currentCell = (mainBoid.position.x / CELL_SIZE);
        int currentRow = (mainBoid.position.y / CELL_SIZE);

        for (int deltaY = -1; deltaY <= 1; deltaY++)
        {
            for (int deltaX = -1; deltaX <= 1; deltaX++)
            {
                int deltaCellX = (currentCell + deltaX);
                int deltaCellY = (currentRow + deltaY);

                deltaCellX = (deltaCellX + COLUMNS) % COLUMNS;
                deltaCellY = (deltaCellY + ROWS) % ROWS;

                deltaCellX *= CELL_SIZE;
                deltaCellY *= CELL_SIZE;

                Rectangle cellRect = {(float)deltaCellX, (float)deltaCellY, CELL_SIZE, CELL_SIZE};

                DrawRectanglePro(cellRect, {0.0f, 0.0f}, 0, RED);
            }
        }
    }

    for (Boid &boid : boidsArray)
    {

        Vector2 v1, v2, v3;

        float size = boid.size;

        v1 = Vector2Add(Vector2Rotate((Vector2){size, 0.0f}, atan2f(boid.velocity.y, boid.velocity.x)), boid.position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(size) / 1.5f, -size / 1.5f}, atan2f(boid.velocity.y, boid.velocity.x)), boid.position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(size) / 1.5f, size / 1.5f}, atan2f(boid.velocity.y, boid.velocity.x)), boid.position);

        if (boid.identifier == debugSelectedBoid && showDebugRadius)
        {
            DrawCircleV(boid.position, BOID_PERCEPTION_RADIUS, {200, 151, 55, 200});
            DrawCircleV(boid.position, BOID_SEPARATION_RADIUS, {231, 41, 55, 200});

            DrawTriangle(v1, v2, v3, boid.color);
        }
        DrawTriangle(v1, v2, v3, boid.color);
    }

    EndMode2D();
    EndDrawing();
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawFrame();
}

int main()
{
    InitWorld();
    PopulateWorld();

    while (!WindowShouldClose())
    {
        while (!WindowShouldClose())
        {
            HandleCameraControl(camera);
            UpdateDrawFrame();
        }

        CloseWindow();
        return 0;
    }
}
