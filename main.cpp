#include "config.h"
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <array>

static auto camera = Camera2D();
int debugSelectedBoid = 14;
bool showDebugRadius = false;
bool showDebugGrid = false;

static std::array<Vector2, MAX_BOIDS> boidPositionArray = {0};
static std::array<Vector2, MAX_BOIDS> boidVelocityArray = {0};
int frameCount{};

void HandleCameraControl(Camera2D &cam)
{
    cam.zoom = Clamp(expf(logf(cam.zoom) + ((float)GetMouseWheelMove() * 0.1f)), 0.1f, 0.2f);

    if (IsKeyDown(KEY_D))
        cam.offset.x -= 10.0f;
    if (IsKeyDown(KEY_A))
        cam.offset.x += 10.0f;
    if (IsKeyDown(KEY_W))
        cam.offset.y += 10.0f;
    if (IsKeyDown(KEY_S))
        cam.offset.y -= 10.0f;

    if (IsKeyPressed(KEY_G))
        ++debugSelectedBoid;
    if (IsKeyPressed(KEY_B))
        showDebugRadius = !showDebugRadius;
    if (IsKeyPressed(KEY_H))
        showDebugGrid = !showDebugGrid;
}

unsigned int ReadComputeShader()
{
    char *shaderSourceCode = LoadFileText("/home/void/graphics/boids/Shaders/boid_compute_shader.comp");
    auto shaderData = rlCompileShader(shaderSourceCode, RL_COMPUTE_SHADER);
    auto computeShader = rlLoadComputeShaderProgram(shaderData);
    UnloadFileText(shaderSourceCode);

    return computeShader;
}

void InitWorld()
{
    camera.offset = (Vector2){(SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f)};
    camera.target = (Vector2){WORLD_WIDTH / 2, (WORLD_HEIGHT / 2) - 250};
    camera.zoom = 0.1f;
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow((SCREEN_WIDTH), (SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);
}

void PopulateWorld()
{

    float positionX{};
    float positionY{};
    float velocityX{};
    float velocityY{};

    for (int i{}; i < MAX_BOIDS; ++i)
    {

        positionX = static_cast<float>(GetRandomValue(0, WORLD_WIDTH));
        positionY = static_cast<float>(GetRandomValue(0, WORLD_HEIGHT));
        velocityX = static_cast<float>(GetRandomValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));
        velocityY = static_cast<float>(GetRandomValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));


        boidPositionArray[i] = (Vector2){positionX, positionY};
        boidVelocityArray[i] = (Vector2){velocityX, velocityY};
    }
}

void UpdateFrame()
{
    ++frameCount;
    HandleCameraControl(camera);

}



int main()
{

    InitWorld();
    PopulateWorld();

    Vector2 worldSpace = (Vector2){WORLD_WIDTH, WORLD_HEIGHT};

    unsigned int computeShader = ReadComputeShader();

    unsigned int position0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int velocity0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidVelocityArray.data(), RL_DYNAMIC_COPY);
    unsigned int position1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int velocity1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidVelocityArray.data(), RL_DYNAMIC_COPY);

    while (!WindowShouldClose())
    {
        float frameTime = GetFrameTime();
        {
            rlEnableShader(computeShader);

            rlSetUniform(0, &MAX_BOIDS, SHADER_UNIFORM_INT, 1);
            rlSetUniform(1, &BOID_ALIGNMENT_STRENGTH, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(2, &BOID_COHESION_STRENGTH, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(3, &BOID_SEPARATION_STRENGTH, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(4, &BOID_PERCEPTION_RADIUS, RL_SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(5, &BOID_SEPARATION_RADIUS, RL_SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(6, &BOID_MAX_SPEED, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(7, &BOID_MIN_SPEED, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(8, &frameTime, SHADER_UNIFORM_FLOAT, 1);
            rlSetUniform(9, &worldSpace, SHADER_UNIFORM_VEC2, 1);
            if (frameCount % 2 == 0)
            {
                rlBindShaderBuffer(position0, 0); // in pos
                rlBindShaderBuffer(velocity0, 1); // in vel
                rlBindShaderBuffer(position1, 2); // out pos
                rlBindShaderBuffer(velocity1, 3); // out vel
            }
            else
            {
                rlBindShaderBuffer(position1, 0); // in pos
                rlBindShaderBuffer(velocity1, 1); // in vel
                rlBindShaderBuffer(position0, 2); // out pos
                rlBindShaderBuffer(velocity0, 3); // out vel
            }

            rlComputeShaderDispatch(MAX_BOIDS / 256, 1, 1);

            if (frameCount % 2 == 0)
            {
                rlReadShaderBuffer(position1, boidPositionArray.data(), MAX_BOIDS * sizeof(Vector2), 0);
                rlReadShaderBuffer(velocity1, boidVelocityArray.data(), MAX_BOIDS * sizeof(Vector2), 0);
            } else
            {
                rlReadShaderBuffer(position0, boidPositionArray.data(), MAX_BOIDS * sizeof(Vector2), 0);
                rlReadShaderBuffer(velocity0, boidVelocityArray.data(), MAX_BOIDS * sizeof(Vector2), 0);
            }

            rlDisableShader();
        }
        {
            BeginDrawing();
            ClearBackground((Color){10, 10, 20, 255});
            BeginMode2D(camera);


            DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 200, RAYWHITE);
            DrawText(TextFormat("Frame count: %d", frameCount), SCREEN_WIDTH, 1 - 500, 200, RAYWHITE);
            DrawText(TextFormat("Boid count: %d", MAX_BOIDS), WORLD_WIDTH - SCREEN_WIDTH, 1 - 500, 200, RAYWHITE);

            for (int i{}; i < MAX_BOIDS; ++i)
            {
                Vector2 velocity = boidVelocityArray[i];
                Vector2 position = boidPositionArray[i];

                Vector2 v1 = Vector2Add((Vector2){10, 0.0f}, position);
                Vector2 v2 = Vector2Add((Vector2){-(10) / 1.5f, -10 / 1.5f}, position);
                Vector2 v3 = Vector2Add((Vector2){-(10) / 1.5f, 10 / 1.5f}, position);

                DrawTriangle(v1, v2, v3, RAYWHITE);
            }

            EndMode2D();
            EndDrawing();
        }
        UpdateFrame();
    }
    CloseWindow();

    return 0;
}
