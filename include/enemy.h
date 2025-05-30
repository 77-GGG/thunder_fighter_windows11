//
// Created by qiguo on 25-5-25.
//

#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "bullet.h"

#define MAX_ENEMIES 5
#define MAX_ENEMY_BULLETS 50
extern Texture2D enemy_texture;

typedef enum {
    ENEMY_NORMAL,   
    ENEMY_LASER,   
    ENEMY_ZIGZAG    
} EnemyType;

typedef struct {
    Vector2 position;
    int hp;           
    bool active;     
    EnemyType type;   
    float timer;     
    float amplitude;  
    float frequency;  
    float baseX;     
} Enemy;

typedef struct EnemyBullet {
    Vector2 position;
    bool active;
} EnemyBullet;

void InitEnemies(Enemy *enemies);
void UpdateEnemies(Enemy *enemies,Bullet *enemyBullets);
void DrawEnemies(Enemy *enemies);
void FireEnemyLaser(Enemy *enemy, Bullet *enemyBullets);

void LoadEnemyTexture(void);      // 加载纹理函数
void UnloadEnemyTexture(void);    // 卸载纹理函数


#endif
