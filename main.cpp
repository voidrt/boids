#include "boid/boid.h"
#include <rlgl.h>

#include <algorithm>

static Camera2D camera = Camera2D();
int debugSelectedBoid = 14;
bool showDebugRadius = false;
bool showDebugGrid = false;

static std::array<Boid, MAX_BOIDS> boidsArray = {0};
static std::array<Vector2, MAX_BOIDS> boidPositionArray = {0};
static std::array<Vector2, MAX_BOIDS> boidVelocityArray = {0};

void HandleCameraControl(Camera2D &camera)
{
    camera.zoom = Clamp(expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f)), 0.1f, 0.2f);

    if (IsKeyDown(KEY_D))
        camera.offset.x -= 10.0f;
    if (IsKeyDown(KEY_A))
        camera.offset.x += 10.0f;
    if (IsKeyDown(KEY_W))
        camera.offset.y += 10.0f;
    if (IsKeyDown(KEY_S))
        camera.offset.y -= 10.0f;

    if (IsKeyPressed(KEY_G))
        ++debugSelectedBoid;
    if (IsKeyPressed(KEY_B))
        showDebugRadius = !showDebugRadius;
    if (IsKeyPressed(KEY_H))
        showDebugGrid = !showDebugGrid;
}

unsigned int ReadComputeShader(void)
{
    char *shaderSourceCode = LoadFileText("/home/void/graphics/boids/Shaders/boid_compute_shader.comp");
    auto shaderData = rlCompileShader(shaderSourceCode, RL_COMPUTE_SHADER);
    auto computeShader = rlLoadComputeShaderProgram(shaderData);
    UnloadFileText(shaderSourceCode);

    return computeShader;
}

void InitWorld(void)
{
    camera.offset = (Vector2){(SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f)};
    camera.target = (Vector2){WORLD_WIDTH / 2, (WORLD_HEIGHT / 2) - 250};
    camera.zoom = 0.1f;
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow((SCREEN_WIDTH), (SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);
}

void PopulateWorld(void)
{

    float positionX{};
    float positionY{};
    float velocityX{};
    float velocityY{};
    int sizeDeviation{};

    for (int i{}; i < MAX_BOIDS; ++i)
    {
        sizeDeviation = GetRandomValue(-6, 6);

        positionX = GetRandomValue(0, WORLD_WIDTH);
        positionY = GetRandomValue(0, WORLD_HEIGHT);
        velocityX = GetRandomValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED);
        velocityY = GetRandomValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED);

        boidsArray[i].size = BOID_BASE_SIZE + sizeDeviation;
        boidsArray[i].position = (Vector2){positionX, positionY};
        boidsArray[i].velocity = (Vector2){velocityX, velocityY};
        boidsArray[i].identifier = i;
        boidsArray[i].color = (Color){(unsigned char)(GetRandomValue(20, 230)), (unsigned char)(GetRandomValue(20, 230)), (unsigned char)(GetRandomValue(20, 230)), 255};

        boidPositionArray[i] = (Vector2){positionX, positionY};
        boidVelocityArray[i] = (Vector2){velocityX, velocityY};
    }
}

void UpdateFrame(void)
{
    HandleCameraControl(camera);

    for (int i{}; i < MAX_BOIDS; ++i)
    {
        boidsArray[i].position = boidPositionArray[i];
        boidsArray[i].velocity = boidVelocityArray[i];
    }
}

void DrawFrame(void)
{

    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground((Color){10, 10, 20, 255});

    DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 200, RAYWHITE);
    DrawText(TextFormat("Grid count: %d", COLUMNS * ROWS), SCREEN_WIDTH, 1 - 500, 200, RAYWHITE);
    DrawText(TextFormat("Boid count: %d", MAX_BOIDS), WORLD_WIDTH - SCREEN_WIDTH, 1 - 500, 200, RAYWHITE);

    for (int i{}; i < MAX_BOIDS; i++)
    {
        boidsArray[i].DrawBoid(boidVelocityArray[i], boidPositionArray[i]);
        if (i == debugSelectedBoid && showDebugRadius)
        {
            DrawCircleV(boidPositionArray[i], BOID_PERCEPTION_RADIUS, {255, 195, 2, 255});
            DrawCircleV(boidPositionArray[i], BOID_SEPARATION_RADIUS, {255, 5, 5, 255});
        }
    }

    EndMode2D();
    EndDrawing();
}

void UpdateDrawFrame(void)
{
    UpdateFrame();
    DrawFrame();
}

int main()
{

    InitWorld();
    PopulateWorld();

    float frametime = GetFrameTime();
    Vector2 worldSpace = (Vector2){WORLD_WIDTH, WORLD_HEIGHT};

    unsigned int computeShader = ReadComputeShader();

    unsigned int ssbo0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int ssbo1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidVelocityArray.data(), RL_DYNAMIC_COPY);

    while (!WindowShouldClose())
    {
        {
            rlEnableShader(computeShader);

            rlSetUniform(0, &MAX_BOIDS, SHADER_UNIFORM_INT, 1);
            rlSetUniform(1, &BOID_ALIGNMENT_STRENGTH, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(2, &BOID_COHESION_STRENGTH, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(3, &BOID_SEPARATION_STRENGTH, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(4, &BOID_MAX_SPEED, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(5, &BOID_MIN_SPEED, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(6, &frametime, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(7, &worldSpace, SHADER_UNIFORM_VEC2, 1);

            rlBindShaderBuffer(ssbo0, 0);
            rlBindShaderBuffer(ssbo1, 1);

            rlComputeShaderDispatch(MAX_BOIDS / 256, 1, 1);

            // boidPositionArray[debugSelectedBoid] = (Vector2){-100000.0f, -100000.0f};

            rlReadShaderBuffer(ssbo0, boidPositionArray.data(), MAX_BOIDS * sizeof(Vector2), 0);
            rlReadShaderBuffer(ssbo1, boidVelocityArray.data(), MAX_BOIDS * sizeof(Vector2), 0);

            rlDisableShader();
        }
        UpdateDrawFrame();
    }
    CloseWindow();

    return 0;
}
