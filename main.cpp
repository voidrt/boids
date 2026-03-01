#include "boid/boid.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>

static std::array<Boid, MAX_BOIDS> boidsArray = {0};
static Camera2D camera = Camera2D();
int selectedBoid = 15;
bool showDebugRadius = false;
bool showDebugGrid = true;

constexpr int gridColumns = (WORLD_WIDTH / BOID_PERCEPTION_RADIUS) + 1;
constexpr int gridRows = (WORLD_HEIGHT / BOID_PERCEPTION_RADIUS) + 1;
constexpr int totalCells = gridColumns * gridRows;

std::array<std::vector<int>, totalCells> worldGrid = {};

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

    for (Boid &boid : boidsArray)
    {
        size = GetRandomValue(BOID_BASE_SIZE - 5, BOID_BASE_SIZE + 5);

        positionX = GetRandomValue(0, WORLD_WIDTH);
        positionY = GetRandomValue(0, WORLD_HEIGHT);

        velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);

        // while (velocityX == 0 && velocityY == 0)
        // {
        //     velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        //     velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        // }

        boid.position = (Vector2){positionX, positionY};
        boid.velocity = (Vector2){velocityX, velocityY};
        boid.size = size;
        boid.identifier = &boid - &boidsArray[0];
        boid.color = (Color){(unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), 255};

        int positionInGridY = std::floor(positionY / BOID_PERCEPTION_RADIUS);
        int positionInGridX = std::floor(positionX / BOID_PERCEPTION_RADIUS);
        int worldGridIndex = (positionInGridY * gridColumns) + positionInGridX;

        worldGrid[worldGridIndex].push_back(boid.identifier);
    }
}

void UpdateGame(void)
{
    for (int i = 0; i < totalCells; i++)
        worldGrid[i].clear();

    for (Boid &boid : boidsArray)
    {
        int gridYPosition = std::floor(boid.position.x / BOID_PERCEPTION_RADIUS);
        int gridXPosition = std::floor(boid.position.y / BOID_PERCEPTION_RADIUS);

        if (gridXPosition < 0)
            gridXPosition = 0;
        if (gridXPosition >= gridColumns)
            gridXPosition = gridColumns - 1;
        if (gridYPosition < 0)
            gridYPosition = 0;
        if (gridYPosition >= gridRows)
            gridYPosition = gridRows - 1;

        int worldGridIndex = (gridYPosition * gridColumns) + gridXPosition;

        worldGrid[worldGridIndex].push_back(boid.identifier);
    }

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)
            continue;
        boid.UpdateVelocity(boidsArray);
    }

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)
            continue;
        boid.UpdatePosition();
    }
}

void DrawGame(void)
{
    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground((Color){2, 2, 20, 255});

    DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 200, RAYWHITE);

    if (showDebugGrid)
    {
        for (int i = 0; i < totalCells; i++)
        {
            float cellX = (float)((i % gridColumns) * BOID_PERCEPTION_RADIUS);
            float cellY = (float)((i / gridColumns) * BOID_PERCEPTION_RADIUS);

            Rectangle cellRect = (Rectangle){cellX, cellY, BOID_PERCEPTION_RADIUS, BOID_PERCEPTION_RADIUS};

            int debugBoidCellX = std::floor((boidsArray[selectedBoid].position.x) / BOID_PERCEPTION_RADIUS);
            int debugBoidCellY = std::floor((boidsArray[selectedBoid].position.y) / BOID_PERCEPTION_RADIUS);

            int debugBoidArrayIndex = (debugBoidCellY * gridColumns) + debugBoidCellX;

            if (debugBoidArrayIndex == i)
            {
                DrawRectangle(cellRect.x,cellRect.y, cellRect.width, cellRect.height, RED);
            }
            else
            {
                DrawRectangleLinesEx(cellRect, 6, {230, 230, 230, 100});
            }
        }
    }
    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)

            continue;

        Vector2 v1, v2, v3;
        float boidSize = (float)boid.size;

        v1 = Vector2Add(Vector2Rotate((Vector2){boidSize, 0.0f}, atan2f(boid.velocity.y, boid.velocity.x)), boid.position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(boidSize) / 2.f, -boidSize / 2.f}, atan2f(boid.velocity.y, boid.velocity.x)), boid.position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(boidSize) / 2.f, boidSize / 2.f}, atan2f(boid.velocity.y, boid.velocity.x)), boid.position);

        if (boid.identifier == selectedBoid && showDebugRadius)
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

void UpdateDrawGame(void)
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
        while (!WindowShouldClose())
        {
            HandleCameraControl(camera);
            UpdateDrawGame();
        }

        CloseWindow();
        return 0;
    }
}
