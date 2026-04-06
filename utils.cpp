#include "utils.h"
#include "config.h"
#include <raylib.h>
#include <raymath.h>


void HandleCameraControl(Camera3D& cam)
{
    UpdateCamera(&cam, CAMERA_FREE);
    cam.target = (Vector3){WORLD_WIDTH / 2, WORLD_HEIGHT / 2, WORLD_DEPTH / 2};
}

float GetRandomFValue(float min, float max)
{
    return min + ((max - min) * static_cast<float>((GetRandomValue(1, INT_MAX)) / static_cast<float>(INT_MAX)));
}
