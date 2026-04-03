#include "utils.h"
#include <raylib.h>
#include<raymath.h>

void HandleCameraControl(Camera2D& cam)
{
    cam.zoom = Clamp(expf(logf(cam.zoom) + ((float)GetMouseWheelMove() * 0.1f)), 0.05f, 0.2f);

    if (IsKeyDown(KEY_D))
        cam.offset.x -= 10.0f;
    if (IsKeyDown(KEY_A))
        cam.offset.x += 10.0f;
    if (IsKeyDown(KEY_W))
        cam.offset.y += 10.0f;
    if (IsKeyDown(KEY_S))
        cam.offset.y -= 10.0f;
}

float GetRandomFValue(float min, float max)
{
    return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX)));
}
