//
// Created by qiguo on 25-5-25.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 1
#define MAX_ENEMY_BULLETS 50

typedef struct Enemy {
    Vector2 position;
    bool active;
} Enemy;

typedef struct EnemyBullet {
    Vector2 position;
    bool active;
} EnemyBullet;

void InitEnemies(void);
void UpdateEnemies(Texture2D enemyTexture);
void DrawEnemies(Texture2D enemyTexture);
void UpdateEnemyBullets(void);
void DrawEnemyBullets(void);

#endif
