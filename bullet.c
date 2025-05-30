//
// Created by qiguo on 25-5-24.
//
#include "bullet.h"

void InitBullets(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void FireBullet(Bullet *bullets, Vector2 position, Vector2 velocity) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = position;
            bullets[i].velocity = velocity;
            bullets[i].active = true;
            break;
        }
    }
}

void UpdateBullets(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.x += bullets[i].velocity.x;
            bullets[i].position.y += bullets[i].velocity.y;

            if (bullets[i].position.y < 0 || bullets[i].position.y > 1600)
                bullets[i].active = false;
        }
    }
}

void DrawBullets(Bullet *bullets) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            DrawCircleV(bullets[i].position, 10, RED);
        }
    }
}