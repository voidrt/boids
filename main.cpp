#include "boid/boid.h"
#include "whoid/whoid.h"
#include <raymath.h>
#include <algorithm>
#include <iostream>

static Camera2D camera = Camera2D();
int debugSelectedBoid = 14;
int debugSelectedWhoid = 3005;
bool showDebugRadius = false;
bool showDebugGrid = false;

static std::array<Boid, MAX_BOIDS> boidsArray = {0};
static std::array<Whoid, MAX_WHOIDS> whoidsArray = {0};
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

    for (Boid &boid : boidsArray)
    {
        size = GetRandomValue(BOID_BASE_SIZE - 5, BOID_BASE_SIZE + 5);

        positionX = GetRandomValue(0, WORLD_WIDTH);
        positionY = GetRandomValue(0, WORLD_HEIGHT);

        velocityX = GetRandomValue(-(BOID_SPEED), BOID_SPEED);
        velocityY = GetRandomValue(-(BOID_SPEED), BOID_SPEED);

        boid.isAlive = true;
        boid.position = (Vector2){positionX, positionY};
        boid.velocity = (Vector2){velocityX, velocityY};
        boid.size = size;
        boid.identifier = &boid - &boidsArray[0];
        boid.color = (Color){(unsigned char)(GetRandomValue(20, 230)), (unsigned char)(GetRandomValue(20, 230)), (unsigned char)(GetRandomValue(20, 230)), 255};
    }

    for (Whoid &whoid : whoidsArray)
    {

        velocityX = GetRandomValue(-WHOID_SPEED, WHOID_SPEED);
        velocityY = GetRandomValue(-WHOID_SPEED, WHOID_SPEED);

        while (fabs(velocityX) < WHOID_SPEED || fabs(velocityY) < WHOID_SPEED)
        {
            velocityX = GetRandomValue(-WHOID_SPEED, WHOID_SPEED);
            velocityY = GetRandomValue(-WHOID_SPEED, WHOID_SPEED);
        }

        positionX = GetRandomValue(100, WORLD_WIDTH - 100);
        positionY = GetRandomValue(100, WORLD_HEIGHT - 100);

        whoid.identifier = (&whoid - &whoidsArray[0]) + MAX_BOIDS;
        whoid.velocity = (Vector2){velocityX, velocityY};
        whoid.position = (Vector2){positionX, positionY};
        whoid.rotation = 0;
    }

    for (auto &row : worldGrid)
    {
        for (auto &cell : row)
        {
            cell.reserve(ESTIMATE_BOID_DISTRIBUTION);
        }
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

    for (Whoid &whoid : whoidsArray)
    {
        int currentCell = (whoid.position.x / CELL_SIZE);
        int currentRow = (whoid.position.y / CELL_SIZE);

        currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
        currentRow = std::clamp(currentRow, 0, ROWS - 1);

        worldGrid[currentRow][currentCell].push_back(whoid.identifier);
    }

    for (Whoid &whoid : whoidsArray)
    {
        whoid.SteerWhoid(worldGrid, whoidsArray);
    }

    for (Whoid &whoid : whoidsArray)
    {
        whoid.MoveWhoid();
    }

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)

            continue;

        int currentCell = (boid.position.x / CELL_SIZE);
        int currentRow = (boid.position.y / CELL_SIZE);

        currentCell = std::clamp(currentCell, 0, COLUMNS - 1);
        currentRow = std::clamp(currentRow, 0, ROWS - 1);

        worldGrid[currentRow][currentCell].push_back(boid.identifier);
    }

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)
            continue;
        boid.SteerBoid(boidsArray, whoidsArray, worldGrid);
    }

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)
            continue;
        boid.MoveBoid();
    }
}

void DrawFrame(void)
{

    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground((Color){5, 5, 10, 255});

    DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 200, RAYWHITE);
    DrawText(TextFormat("Grid count: %d", COLUMNS * ROWS), SCREEN_WIDTH, 1 - 500, 200, RAYWHITE);

    if (showDebugGrid)
    {

        for (int y = 0; y < ROWS; y++)
        {
            for (int x = 0; x < COLUMNS; x++)
            {
                float currentCellX = x * BOID_PERCEPTION_RADIUS;
                float currentCellY = y * BOID_PERCEPTION_RADIUS;

                Rectangle cellRect = {currentCellX, currentCellY, BOID_PERCEPTION_RADIUS, BOID_PERCEPTION_RADIUS};

                DrawRectangleLinesEx(cellRect, 5, {255, 255, 255, 50});
            }
        }
    }

    if (showDebugGrid)
    {
        Boid &mainBoid = boidsArray[debugSelectedBoid];

        int currentCell = (mainBoid.position.x / CELL_SIZE);
        int currentRow = (mainBoid.position.y / CELL_SIZE);

        for (int deltaY = -2; deltaY <= 2; deltaY++)
        {
            for (int deltaX = -2; deltaX <= 2; deltaX++)
            {
                int deltaCellX = (currentCell + deltaX);
                int deltaCellY = (currentRow + deltaY);

                deltaCellX = (deltaCellX + COLUMNS) % COLUMNS;
                deltaCellY = (deltaCellY + ROWS) % ROWS;

                deltaCellX *= CELL_SIZE;
                deltaCellY *= CELL_SIZE;
                Color gridColor = {255, 255, 16, 255}; // orange

                Rectangle cellRect = {(float)deltaCellX, (float)deltaCellY, CELL_SIZE, CELL_SIZE};

                mainBoid.DrawBoid();
                if (abs(deltaX) <= 1 && abs(deltaY) <= 1)
                {
                    gridColor = {255, 143, 10, 255};
                    DrawRectangleLinesEx(cellRect, 20, gridColor);
                }

                if (deltaX == 0 && deltaY == 0)
                {
                    gridColor = {255, 5, 5, 255}; // red
                    DrawRectangleLinesEx(cellRect, 20, gridColor);
                }
                DrawRectangleLinesEx(cellRect, 10, gridColor);
            }
        }
    }

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)
            continue;

        boid.DrawBoid();
        if (boid.identifier == debugSelectedBoid && showDebugRadius)
        {
            DrawCircleV(boid.position, BOID_TO_WHOID_PERCEPTION_RADIUS, {100, 255, 100, 255});
            DrawCircleV(boid.position, BOID_PERCEPTION_RADIUS, {255, 195, 2, 255});
            DrawCircleV(boid.position, BOID_SEPARATION_RADIUS, {255, 5, 5, 255});
        }
    }

    for (Whoid &whoid : whoidsArray)
    {
        if (showDebugGrid && whoid.identifier == debugSelectedWhoid)
        {
            DrawCircleV(whoid.position, WHOID_PERCEPTION_RADIUS, {255, 195, 5, 50});
        }
        whoid.DrawWhoid();
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
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    InitWorld();
    PopulateWorld();

    while (!WindowShouldClose())
    {
        HandleCameraControl(camera);
        UpdateDrawFrame();
    }
    CloseWindow();

    return 0;
}
