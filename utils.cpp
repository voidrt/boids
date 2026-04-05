#include "utils.h"
#include <raylib.h>
#include<raymath.h>

void HandleCameraControl(Camera3D& cam)
{
    UpdateCamera(&cam, CAMERA_FREE);
    if (IsKeyPressed(KEY_Z)) cam.target = (Vector3){ 0.0f, 0.0f, 0.0f };
}

float GetRandomFValue(float min, float max)
{
    return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX)));
}
