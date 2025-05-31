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
        enemies[i].hp = MAX_ENEMY_HP;//敌机的生命值,使用常量初始化，在.h文件中定义
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

            //绘制血条背景（灰色）
            Rectangle bgBar = {
                .x = enemies[i].position.x,
                .y = enemies[i].position.y - 10,
                .width = 40,
                .height = 5
            };
            DrawRectangleRec(bgBar,GRAY);

            //绘制当前血量（根据血量比例改变颜色）
            float hpRatio = (float)enemies[i].hp / MAX_ENEMY_HP;
            Rectangle hpBar = {
                .x = enemies[i].position.x,
                .y = enemies[i].position.y - 10,
                .width = 40 * hpRatio,
                .height = 5
            };
            Color hpColor = hpRatio > 0.6f ? GREEN :
                            hpRatio > 0.3f ? YELLOW : RED;
            DrawRectangleRec(hpBar, hpColor);
        }
    }
}
