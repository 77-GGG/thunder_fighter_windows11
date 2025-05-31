//
// Created by qiguo on 25-5-25.
//
#include "enemy.h"
#include <stdlib.h>
#include <math.h>
#include "bullet.h"

Texture2D enemy_texture;
Texture2D enemy_laser_texture;
Texture2D enemy_zigzag_texture;

void LoadEnemyTexture(void) {
    enemy_texture = LoadTexture("../rsc/enemy.png");
    enemy_laser_texture = LoadTexture("../rsc/enemy_laser.png");  // 需新图
    enemy_zigzag_texture = LoadTexture("../rsc/enemy_zigzag.png"); // 需新图
}

void UnloadEnemyTexture(void) {
    UnloadTexture(enemy_texture);
    UnloadTexture(enemy_laser_texture);
    UnloadTexture(enemy_zigzag_texture);
}

void InitEnemies(Enemy *enemies) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].position = (Vector2){rand() % 700 + 50, rand() % 200};
        enemies[i].hp = MAX_ENEMY_HP;//敌机的生命值,使用常量初始化，在.h文件中定义
        enemies[i].active = true;

        int type = rand() % 3;
        switch (type) {
            case 0:
                enemies[i].type = ENEMY_NORMAL;
                break;
            case 1:
                enemies[i].type = ENEMY_LASER;
                enemies[i].timer = 0;
                enemies[i].hp = 4;
                break;
            case 2:
                enemies[i].type = ENEMY_ZIGZAG;
                enemies[i].amplitude = 50 + (rand() % 50);
                enemies[i].frequency = 0.02f + ((float)(rand() % 100) / 1000.0f);
                enemies[i].baseX = enemies[i].position.x;
                enemies[i].timer = (float)(rand() % 628) / 100.0f;
                enemies[i].hp = 2;
                break;
        }
    }
}

void FireEnemyLaser(Enemy *enemy, Bullet *enemyBullets) {
    Vector2 position = {enemy->position.x + 16, enemy->position.y + 32};
    Vector2 velocity = {0, 5};

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!enemyBullets[i].active) {
            enemyBullets[i].position = position;
            enemyBullets[i].velocity = velocity;
            enemyBullets[i].active = true;
            break;
        }
    }
}

void UpdateEnemies(Enemy *enemies, Bullet *enemyBullets) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            switch (enemies[i].type) {
                case ENEMY_NORMAL:
                    enemies[i].position.y += 1;
                    break;

                case ENEMY_LASER:

                    enemies[i].position.y += 0.7f;

                    enemies[i].timer += GetFrameTime();
                    if (enemies[i].timer >= 2.0f) {
                        FireEnemyLaser(&enemies[i], enemyBullets);
                        enemies[i].timer = 0;
                    }
                    break;

                case ENEMY_ZIGZAG:
                    enemies[i].timer += GetFrameTime();
                    enemies[i].position.y += 1.2f;

                    enemies[i].position.x = enemies[i].baseX +
                                           sin(enemies[i].timer * enemies[i].frequency * 10) *
                                           enemies[i].amplitude;
                    break;
            }

            if (enemies[i].position.y > GetScreenHeight()) {
                enemies[i].active = false;

                if (rand() % 100 < 70) {
                    enemies[i].position = (Vector2){rand() % 700 + 50, -50};
                    enemies[i].active = true;
                }
            }
        }
    }
}

void DrawEnemies(Enemy *enemies) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            switch (enemies[i].type) {
                case ENEMY_NORMAL:
                    DrawTexture(enemy_texture, enemies[i].position.x, enemies[i].position.y, WHITE);
                    break;
                case ENEMY_LASER:
                    DrawTexture(enemy_laser_texture, enemies[i].position.x, enemies[i].position.y, WHITE);
                    break;
                case ENEMY_ZIGZAG:
                    DrawTexture(enemy_zigzag_texture, enemies[i].position.x, enemies[i].position.y, WHITE);
                    break;
            }

            // DrawText(TextFormat("%d", enemies[i].hp), enemies[i].position.x + 16, enemies[i].position.y - 20, 20, RED);
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
