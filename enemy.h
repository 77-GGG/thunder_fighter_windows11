//
// Created by qiguo on 25-5-25.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 2
#define MAX_ENEMY_BULLETS 50

typedef struct Enemy {
    Vector2 position;
    bool active;
    Rectangle rect;
    int health;
    Vector2 velocity;  // 新增：敌机的速度向量
    float changeDirectionTimer;  // 新增：改变方向的计时器
    float shootTimer;  // 新增：发射子弹的计时器

} Enemy;

extern Enemy enemies[MAX_ENEMIES];

typedef struct EnemyBullet {
    Vector2 position;
    bool active;
} EnemyBullet;

Rectangle GetEnemyRect();
bool IsEnemyActive();
void DeactivateEnemy();

void InitEnemies(void);
void UpdateEnemies(Texture2D enemyTexture);
void DrawEnemies(Texture2D enemyTexture);
void UpdateEnemyBullets(void);
void DrawEnemyBullets(void);

#endif
