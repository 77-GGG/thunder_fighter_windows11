//
// Created by qiguo on 25-5-24.
//
#include "raylib.h"
#include "movement.h"
#include "bullet.h"
#include "include/enemy.h"
#include "collision.h"

int main(void) {
    const int screen_width = 800;
    const int screen_height = 1200;
    InitWindow(screen_width, screen_height, "thunder_fighter");
    SetTargetFPS(60);

    // 玩家相关
    Vector2 playerPos = {400, 600};  // 玩家初始位置
    Texture plane = LoadTexture("../rsc/plane.png");
    int playerHp = 3;//玩家的生命值

    Texture enemy_texture = LoadTexture("../rsc/enemy.png");
    // 初始化模块
    Bullet playerBullets[MAX_BULLETS];
    Bullet enemyBullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    LoadEnemyTexture();
    InitBullets(playerBullets);
    InitBullets(enemyBullets);
    InitEnemies(enemies);

    while (!WindowShouldClose()) {
        // 玩家移动
        movement(&playerPos.x, &playerPos.y);

        // 玩家发射炮弹
        if (IsKeyPressed(KEY_J)) {
            Vector2 bulletVel = {0, -10};  // 向上移动
            Vector2 bulletPos = {playerPos.x + 64, playerPos.y};  // 飞机中部
            FireBullet(playerBullets, bulletPos, bulletVel);
        }

        // 敌机发射炮弹（简单随机）
        for (int i = 0; i < MAX_ENEMIES; i++) {
             if (enemies[i].active && enemies[i].type == ENEMY_NORMAL && GetRandomValue(0, 100) < 1) {
                Vector2 bulletVel = {0, 5};
                Vector2 bulletPos = {enemies[i].position.x, enemies[i].position.y + 20};
                FireBullet(enemyBullets, bulletPos, bulletVel);
            }
        }

        // 更新模块
        UpdateBullets(playerBullets);
        UpdateBullets(enemyBullets);
        UpdateEnemies(enemies, enemyBullets);
        CheckBulletEnemyCollisions(playerBullets, enemies);
        CheckPlayerHit(enemyBullets, playerPos, &playerHp);

        // 绘制
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(plane, playerPos.x, playerPos.y, WHITE);
        DrawBullets(playerBullets);
        DrawBullets(enemyBullets);
        DrawEnemies(enemies);

        DrawText(TextFormat("HP: %d", playerHp), 10, 10, 20, RED);
        if (playerHp <= 0) {
            DrawText("GAME OVER", screen_width / 2 - 100, screen_height / 2, 40, BLACK);
        }

        EndDrawing();
    }
    UnloadEnemyTexture();
    CloseWindow();
    return 0;
}
