//
// Created by qiguo on 25-5-24.
//
#include "raylib.h"
#include "movement.h"
#include "bullet.h"
#include "enemy.h"

int main(void) {
    const int screen_width = 800;//屏幕宽度
    const int screen_height = 1000;//屏幕高度
    int plane_position_x = 50;//初始战机横坐标
    int plane_position_y = 50;//初始战机纵坐标
    int enemy_position_x = 100;
    int enemy_position_y = 100;


    InitWindow(screen_width, screen_height, "test");
    SetTargetFPS(60);
    Texture plane_texture = LoadTexture("../rsc/plane.png");
    Texture enemy_texture = LoadTexture("../rsc/enemy.png");

    InitBullets();//初始化炮弹系统
    InitEnemies();//初始化敌机系统

    while (!WindowShouldClose()) {
        plane_movement(&plane_position_x, &plane_position_y);//控制战机移动

        if (IsKeyPressed(KEY_J)) {
            FireBullet(plane_position_x, plane_position_y);  // 发射一颗新炮弹
        }
        UpdateEnemies(enemy_texture);
        UpdateEnemyBullets();

        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(plane_texture,plane_position_x,plane_position_y,WHITE);
        UpdateAndDrawBullets();  // 更新并绘制所有炮弹

        DrawEnemies(enemy_texture);
        DrawEnemyBullets();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
