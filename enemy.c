//
// Created by qiguo on 25-5-25.
//
#include "enemy.h"
#include <stdlib.h>
Texture2D enemy_texture;


void LoadEnemyTexture(void) {
    enemy_texture = LoadTexture("../rsc/enemy.png");
}

void UnloadEnemyTexture(void) {
    UnloadTexture(enemy_texture);
}
void InitEnemies(Enemy *enemies) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].position = (Vector2){rand() % 700 + 50, rand() % 200};
        enemies[i].hp = 3;//敌机的生命值
        enemies[i].active = true;
    }
}

void UpdateEnemies(Enemy *enemies) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].position.y += 1; // 慢慢向下
        }
    }
}

void DrawEnemies(Enemy *enemies) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // DrawCircleV(enemies[i].position, 32, BLUE);
            DrawTexture(enemy_texture,enemies[i].position.x,enemies[i].position.y,WHITE);
        }
    }
}
