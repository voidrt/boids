#include "boid/boid.h"
#include <raylib.h>
#include <raymath.h>

static std::array<Boid, MAX_BOIDS> boidsArray = {0};
static Camera2D camera = Camera2D();
int selectedBoid = 15;
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
    if (IsKeyPressed(KEY_G))
        ++selectedBoid;
    if (IsKeyPressed(KEY_B))
        showDebugRadius = !showDebugRadius;
}

void InitWorld(void)
{
    camera.offset = (Vector2){(SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f)};
    camera.target = (Vector2){0, 0};
    camera.zoom = 0.1f;

    selectedBoid = 0;
    showDebugRadius = false;

    InitWindow((SCREEN_WIDTH), (SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);
}

void PopulateWorld(void)
{
    float positionX;
    float positionY;
    float velocityX;
    float velocityY;
    float size;

    for (Boid &boid : boidsArray)
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

        boid.position = (Vector2){positionX, positionY};
        boid.velocity = (Vector2){velocityX, velocityY};
        boid.rotation = atan2f(velocityY, velocityX);
        boid.size = size;
        boid.perceptionRadius = BOID_SEPARATION_RADIUS + size;
        boid.id = &boid - &boidsArray[0];
        boid.color = (Color){(unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), 255};
    }
}

void UpdateGame(void)
{
    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)
            continue;
        boid.UpdateVelocity(boidsArray);
        boid.UpdatePosition();
    }
}

void DrawGame(void)
{
    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground((Color){2, 2, 20, 255});

    DrawText(TextFormat("FPS: %d", GetFPS()), -WORLD_WIDTH, -WORLD_HEIGHT - 500, 200, RAYWHITE);

    for (Boid &boid : boidsArray)
    {
        if (!boid.isAlive)

            continue;

        Vector2 v1, v2, v3;

        v1 = Vector2Add(Vector2Rotate((Vector2){boid.size, 0.0f}, boid.rotation), boid.position);
        v2 = Vector2Add(Vector2Rotate((Vector2){-(boid.size) / 1.5f, -boid.size / 1.5f}, boid.rotation), boid.position);
        v3 = Vector2Add(Vector2Rotate((Vector2){-(boid.size) / 1.5f, boid.size / 1.5f}, boid.rotation), boid.position);

        if (boid.id == selectedBoid && showDebugRadius)
        {
            DrawCircleV(boid.position, boid.perceptionRadius, {200, 151, 55, 200});
            DrawCircleV(boid.position, boid.repelRadius, {231, 41, 55, 200});

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
