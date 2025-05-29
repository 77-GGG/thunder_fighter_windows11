//
// Created by qiguo on 25-5-25.
//
#include "enemy.h"
#include <stdlib.h>

Enemy enemy;
Enemy enemies[MAX_ENEMIES];
static EnemyBullet bullets[MAX_ENEMY_BULLETS];
static int frameCount = 0;

void InitEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].position = (Vector2){100 + i * 120, 100};
        enemies[i].active = true;
    }

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        bullets[i].active = false;
    }
}

void FireEnemyBullet(Vector2 pos) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].position = (Vector2){pos.x + 30, pos.y + 60};
            bullets[i].active = true;
            break;
        }
    }
}

void UpdateEnemies(Texture2D enemyTexture) {
    frameCount++;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        // 敌机左右随机移动
        if (frameCount % 30 == 0) {
            int dir = rand() % 3 - 1; // -1, 0, 1
            enemies[i].position.x += dir * 10;
            if (enemies[i].position.x < 0) enemies[i].position.x = 0;
            if (enemies[i].position.x > 750) enemies[i].position.x = 750;
        }

        // 敌机发射子弹（每隔 60 帧）
        if (frameCount % 60 == 0) {
            FireEnemyBullet(enemies[i].position);
        }
    }
}

void DrawEnemies(Texture2D enemyTexture) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            DrawTexture(enemyTexture, enemies[i].position.x, enemies[i].position.y, WHITE);
        }
    }
}

void UpdateEnemyBullets(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].position.y += 8;
            if (bullets[i].position.y > 1600) {
                bullets[i].active = false;
            }
        }
    }
}

void DrawEnemyBullets(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            DrawCircleV(bullets[i].position, 10, DARKBLUE);
        }
    }
}

Rectangle GetEnemyRect()
{
    return enemy.rect;
}

bool IsEnemyActive()
{
    return enemy.active;
}

void DeactivateEnemy()
{
    enemy.active = false;
}