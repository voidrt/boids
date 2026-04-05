#include "config.h"
#include "utils.h"
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <array>


static Camera3D camera{};

static std::array<Vector4, MAX_BOIDS> boidPositionArray = {0};
static std::array<Vector4, MAX_BOIDS> boidVelocityArray = {0};
static int frameCount{};

unsigned int ReadComputeShader()
{
    char* shaderSourceCode = LoadFileText("/home/void/graphics/boids/Shaders/boid_compute_shader.comp");
    auto shaderData = rlCompileShader(shaderSourceCode, RL_COMPUTE_SHADER);
    auto computeShader = rlLoadComputeShaderProgram(shaderData);
    UnloadFileText(shaderSourceCode);

    return computeShader;
}

void InitWorld()
{
    camera.position = (Vector3){(WORLD_WIDTH), (WORLD_HEIGHT), (WORLD_DEPTH)};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 90.0f;
    DisableCursor();

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow((SCREEN_WIDTH), (SCREEN_HEIGHT), "Boids swimming");
    SetTargetFPS(60);
}

void PopulateWorld()
{
    float positionX{};
    float positionY{};
    float positionZ{};
    float velocityX{};
    float velocityY{};
    float velocityZ{};

    for (size_t i{}; i < MAX_BOIDS; ++i)
    {
        positionX = (GetRandomFValue(0, WORLD_WIDTH));
        positionY = (GetRandomFValue(0, WORLD_HEIGHT));
        positionZ = (GetRandomFValue(0, WORLD_DEPTH));
        boidPositionArray[i] = (Vector4){positionX, positionY, positionZ, 0.0};
    }

    for (size_t i{}; i < MAX_BOIDS; ++i)
    {
        velocityX = (GetRandomFValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));
        velocityY = (GetRandomFValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));
        velocityZ = (GetRandomFValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));
        boidVelocityArray[i] = (Vector4){velocityX, velocityY, velocityZ, 0.0};
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

    unsigned int computeShader = ReadComputeShader();
    unsigned int position0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector4), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int velocity0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector4), boidVelocityArray.data(), RL_DYNAMIC_COPY);
    unsigned int position1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector4), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int velocity1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector4), boidVelocityArray.data(), RL_DYNAMIC_COPY);

    Vector4 worldSpace = (Vector4){WORLD_WIDTH, WORLD_HEIGHT, WORLD_DEPTH, 0.0};

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
            rlSetUniform(9, &worldSpace, SHADER_UNIFORM_VEC4, 1);
            rlSetUniform(10, &SCALE, SHADER_UNIFORM_FLOAT, 1);
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
            rlComputeShaderDispatch(MAX_BOIDS / 64, 1, 1);

            if (frameCount % 2 == 0)
            {
                rlReadShaderBuffer(position1, boidPositionArray.data(), sizeof(Vector4) * MAX_BOIDS, 0);
                rlReadShaderBuffer(velocity1, boidVelocityArray.data(), sizeof(Vector4) * MAX_BOIDS, 0);
            }
            else
            {
                rlReadShaderBuffer(position0, boidPositionArray.data(), sizeof(Vector4) * MAX_BOIDS, 0);
                rlReadShaderBuffer(velocity0, boidVelocityArray.data(), sizeof(Vector4) * MAX_BOIDS, 0);
            }
            rlDisableShader();
        }
        BeginDrawing();
        ClearBackground((Color){182, 189, 195, 255});

        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 40, 20, DARKGRAY);
        DrawText(TextFormat("Boid count: %d", MAX_BOIDS), SCREEN_WIDTH - 190, 40, 20, DARKGRAY);

        BeginMode3D(camera);
        // DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 350, BLACK);
        // DrawText(TextFormat("Frame count: %d", frameCount), SCREEN_WIDTH, 1 - 500, 350, BLACK);
        // DrawText(TextFormat("Boid count: %d", MAX_BOIDS), WORLD_WIDTH - 2 * SCREEN_WIDTH, 1 - 500, 350, BLACK);
        {
            for (size_t i{}; i < MAX_BOIDS; ++i)
            {
                Vector3 boidPosition = (Vector3){
                    boidPositionArray[i].x, boidPositionArray[i].y, boidPositionArray[i].z
                };
                float boidSize = BOID_BASE_SIZE * SCALE;
                DrawCubeV(boidPosition, {boidSize, boidSize, boidSize}, BLACK);
            }
        }
        DrawCubeWires({WORLD_WIDTH / 2, WORLD_HEIGHT / 2, WORLD_DEPTH / 2}, WORLD_WIDTH, WORLD_HEIGHT,
                      WORLD_DEPTH, DARKGRAY);
        EndMode3D();
        EndDrawing();

        UpdateFrame();
    }
    CloseWindow();

    return 0;
}
