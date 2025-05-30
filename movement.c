//
// Created by qiguo on 25-5-23.
//
#include "movement.h"
#include "raylib.h"


void movement(float *x, float *y) {
    if (IsKeyDown(KEY_W)) *y -= 10;
    if (IsKeyDown(KEY_S)) *y += 10;
    if (IsKeyDown(KEY_A)) *x -= 10;
    if (IsKeyDown(KEY_D)) *x += 10;
}

