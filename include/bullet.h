//
// Created by qiguo on 25-5-24.
//

#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_BULLETS 100

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    bool active;
} Bullet;

void InitBullets(Bullet *bullets);
void FireBullet(Bullet *bullets, Vector2 position, Vector2 velocity);
void UpdateBullets(Bullet *bullets);
void DrawBullets(Bullet *bullets);

#endif //BULLET_H
