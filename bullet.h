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
    Rectangle rect;
} Bullet;

extern Bullet bullets[MAX_BULLETS];

Rectangle GetBulletRect(int index);
bool IsBulletActive(int index);
void DeactivateBullet(int index);

void InitBullets(void);
void FireBullet(int x, int y);
void UpdateAndDrawBullets(void);


#endif //BULLET_H
