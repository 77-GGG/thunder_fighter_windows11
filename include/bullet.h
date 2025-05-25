//
// Created by qiguo on 25-5-24.
//

#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

#define MAX_BULLETS 100

typedef struct Bullet {
    Vector2 position;
    bool active;
} Bullet;

void InitBullets(void);
void FireBullet(int x, int y);
void UpdateAndDrawBullets(void);


#endif //BULLET_H
