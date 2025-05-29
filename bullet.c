//
// Created by qiguo on 25-5-24.
//
#include "bullet.h"

Bullet bullets[MAX_BULLETS];  // 模块私有变量

void InitBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void FireBullet(int x, int y) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = (Vector2){x + 127, y + 10};
            bullets[i].active = true;
            break;
        }
    }
}

void UpdateAndDrawBullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.y -= 10;
            if (bullets[i].position.y < 0) {
                bullets[i].active = false;
            }
            DrawCircleV(bullets[i].position, 10, RED);
        }
    }
}

Rectangle GetBulletRect(int index)
{
    return bullets[index].rect;
}

bool IsBulletActive(int index)
{
    return bullets[index].active;
}

void DeactivateBullet(int index)
{
    bullets[index].active = false;
}
