#include "config.h"
#include "utils.h"
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <array>

#include "build/_deps/raylib-src/src/external/glad.h"

static auto camera = Camera2D();
int debugSelectedBoid = 14;
bool showDebugRadius = false;
bool showDebugGrid = false;

static std::array<Vector2, MAX_BOIDS> boidPositionArray = {0};
static std::array<Vector2, MAX_BOIDS> boidVelocityArray = {0};
int frameCount{};

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

    for (size_t i{}; i < MAX_BOIDS; ++i)
    {
        positionX = (GetRandomFValue(0, WORLD_WIDTH));
        positionY = (GetRandomFValue(0, WORLD_HEIGHT));
        velocityX = (GetRandomFValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));
        velocityY = (GetRandomFValue(-(BOID_MAX_SPEED), BOID_MAX_SPEED));

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

    unsigned int computeShader = ReadComputeShader();
    unsigned int position0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int velocity0 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidVelocityArray.data(), RL_DYNAMIC_COPY);
    unsigned int position1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidPositionArray.data(), RL_DYNAMIC_COPY);
    unsigned int velocity1 = rlLoadShaderBuffer(MAX_BOIDS * sizeof(Vector2), boidVelocityArray.data(), RL_DYNAMIC_COPY);

    Vector2 worldSpace = (Vector2){WORLD_WIDTH, WORLD_HEIGHT};

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
            rlComputeShaderDispatch(MAX_BOIDS / 64, 1, 1);

            if (frameCount % 2 == 0)
            {
                rlReadShaderBuffer(position1, boidPositionArray.data(), sizeof(Vector2) * MAX_BOIDS, 0);
                rlReadShaderBuffer(velocity1, boidVelocityArray.data(), sizeof(Vector2) * MAX_BOIDS, 0);
            }
            else
            {
                rlReadShaderBuffer(position0, boidPositionArray.data(), sizeof(Vector2) * MAX_BOIDS, 0);
                rlReadShaderBuffer(velocity0, boidVelocityArray.data(), sizeof(Vector2) * MAX_BOIDS, 0);
            }
            rlDisableShader();
        }
        {
            BeginDrawing();
            ClearBackground((Color){202, 209, 215, 255});
            BeginMode2D(camera);

            for (size_t i{}; i < MAX_BOIDS; ++i)
            {
                Vector2 v1 = {40, 0};
                Vector2 v2 = {-10, -13};
                Vector2 v3 = {-10, 13};
                Vector2 position = boidPositionArray[i];
                Vector2 velocity = boidVelocityArray[i];

                v1 = Vector2Rotate(v1, atan2f(velocity.y, velocity.x));
                v2 = Vector2Rotate(v2, atan2f(velocity.y, velocity.x));
                v3 = Vector2Rotate(v3, atan2f(velocity.y, velocity.x));

                DrawTriangle(Vector2Add(v1, position), Vector2Add(v2, position), Vector2Add(v3, position), BLACK);
            }

            DrawText(TextFormat("FPS: %d", GetFPS()), 1, 1 - 500, 350, BLACK);
            DrawText(TextFormat("Frame count: %d", frameCount), SCREEN_WIDTH, 1 - 500, 350, BLACK);
            DrawText(TextFormat("Boid count: %d", MAX_BOIDS), WORLD_WIDTH - 2 * SCREEN_WIDTH, 1 - 500, 350, BLACK);

            EndMode2D();
            EndDrawing();
        }
        UpdateFrame();
    }
    CloseWindow();

    return 0;
}
