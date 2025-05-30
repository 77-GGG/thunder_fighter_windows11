//
// Created by qiguo on 25-5-25.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 5
#define MAX_ENEMY_BULLETS 50
extern Texture2D enemy_texture;

typedef struct Enemy {
    Vector2 position;
    int hp;
    bool active;
} Enemy;

typedef struct EnemyBullet {
    Vector2 position;
    bool active;
} EnemyBullet;

void InitEnemies(Enemy *enemies);
void UpdateEnemies(Enemy *enemies);
void DrawEnemies(Enemy *enemies);

void LoadEnemyTexture(void);      // 加载纹理函数
void UnloadEnemyTexture(void);    // 卸载纹理函数


#endif
