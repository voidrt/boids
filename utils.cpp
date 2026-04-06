#include "utils.h"
#include "config.h"
#include <raylib.h>
#include <raymath.h>

#include "rcamera.h"


void HandleSimulationControl(Camera3D& cam, bool &isPaused)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        isPaused = !isPaused;
    }

    const float cameraMoveSpeed = 50.0f * GetFrameTime();
    constexpr float zoomSpeed = 5.0f;

    Vector3 movement{};
    float zoom{};

    if (IsKeyDown(KEY_W)) movement.x += cameraMoveSpeed;
    if (IsKeyDown(KEY_S)) movement.x -= cameraMoveSpeed;
    if (IsKeyDown(KEY_D)) movement.y += 2 * cameraMoveSpeed;
    if (IsKeyDown(KEY_A)) movement.y -= 2 * cameraMoveSpeed;
    if (IsKeyDown(KEY_LEFT_SHIFT)) movement.z += cameraMoveSpeed;
    if (IsKeyDown(KEY_LEFT_CONTROL)) movement.z -= cameraMoveSpeed;

    zoom = -GetMouseWheelMove() * zoomSpeed;

    UpdateCameraPro(&cam, movement, {}, zoom);
    cam.target = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2, WORLD_DEPTH / 2};
}

float GetRandomFValue(float min, float max)
{
    return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX)));
}
