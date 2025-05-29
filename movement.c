//
// Created by qiguo on 25-5-23.
//
#include "movement.h"
#include "raylib.h"

void plane_movement(int *position_x,int *position_y) {
    if (IsKeyDown(KEY_W)) {
        *position_y -= 10;
    }
    if (IsKeyDown(KEY_S)) {
        *position_y += 10;
    }
    if (IsKeyDown(KEY_A)) {
        *position_x -= 10;
    }
    if (IsKeyDown(KEY_D)) {
        *position_x += 10;
    }
}

Rectangle GetPlaneRect()
{
    // 假设战机的矩形定义
    return (Rectangle){100, 100, 50, 50};
}
