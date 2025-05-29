//
// Created by qiguo on 25-5-24.
//
#include "raylib.h"
#include "movement.h"
#include "bullet.h"
#include "enemy.h"
#include <stdio.h>

#define PLANE_MAX_HEALTH 100
#define ENEMY_MAX_HEALTH 50

int main(void) {
    const int screen_width = 1600;//屏幕宽度
    const int screen_height = 1500;//屏幕高度
    int plane_position_x = 50;//初始战机横坐标
    int plane_position_y = 50;//初始战机纵坐标
    int enemy_position_x = 100;
    int enemy_position_y = 100;
    int planeHealth = PLANE_MAX_HEALTH;
    int enemyHealth = ENEMY_MAX_HEALTH;
    int score = 0;


    InitWindow(screen_width, screen_height, "test");
    SetTargetFPS(60);
    const Texture plane_texture = LoadTexture("../rsc/plane.png");
    const Texture enemy_texture = LoadTexture("../rsc/enemy.png");

    InitBullets();//初始化炮弹系统
    InitEnemies();//初始化敌机系统

    while (!WindowShouldClose()) {
        plane_movement(&plane_position_x, &plane_position_y);//控制战机移动

        if (IsKeyPressed(KEY_J)) {
            FireBullet(plane_position_x, plane_position_y);  // 发射一颗新炮弹
        }
        UpdateEnemies(enemy_texture);
        UpdateEnemyBullets();

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (IsBulletActive(i) && IsEnemyActive())
            {
                if (CheckCollisionRecs(GetBulletRect(i), GetEnemyRect()))
                {
                    DeactivateBullet(i);
                    enemyHealth -= 10; // 减少敌机血量
                    if (enemyHealth <= 0)
                    {
                        enemyHealth = ENEMY_MAX_HEALTH; // 重置敌机血量
                        score += 100; // 增加得分
                    }
                }
            }
        }

        // 检查敌机与战机的碰撞
        if (IsEnemyActive() && CheckCollisionRecs(GetPlaneRect(), GetEnemyRect()))
        {
            planeHealth -= 20; // 减少战机血量
            if (planeHealth <= 0)
            {
                // 游戏结束逻辑
                planeHealth = 0;
            }
        }

        // 绘制
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(plane_texture,plane_position_x,plane_position_y,WHITE);
        UpdateAndDrawBullets();  // 更新并绘制所有炮弹

        DrawEnemies(enemy_texture);
        DrawEnemyBullets();

        //显示血量和得分
        char planeHealthText[50];
        sprintf(planeHealthText, "Plane Health: %d", planeHealth);
        DrawText(planeHealthText, 10, 10, 20, RED);

        char enemyHealthText[50];
        sprintf(enemyHealthText, "Enemy Health: %d", enemyHealth);
        DrawText(enemyHealthText, 10, 40, 20, RED);

        char scoreText[50];
        sprintf(scoreText, "Score: %d", score);
        DrawText(scoreText, 10, 70, 20, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
