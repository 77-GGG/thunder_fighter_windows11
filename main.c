//
// Created by Koishie on 25-5-29.
//

#include "raylib.h"
#include "movement.h"
#include "bullet.h"
#include "enemy.h"


typedef enum {
    GAME_TITLE,           
    GAME_PLAYING,         
    GAME_LEVEL_COMPLETE,  
    GAME_OVER             
} GameState;

typedef struct {
    int x;                
    int y;                 
    int lives;             
    int score;             
    bool isInvincible;     
    float invincibleTimer; 
} Player;

void DrawGameUI(Player player, int currentLevel, int screen_width);
bool CheckPlayerEnemyCollision(Player player, Texture2D playerTexture);

int main(void) {
    const int screen_width = 800;    // 屏幕宽度
    const int screen_height = 1000;  // 屏幕高度
    
    // 初始化玩家数据
    Player player = {
        .x = 400,              // 初始战机横坐标（屏幕中间）
        .y = screen_height - 100, // 初始战机纵坐标（靠近底部）
        .lives = 3,            // 初始3条命
        .score = 0,            // 初始分数0
        .isInvincible = false, // 初始非无敌状态
        .invincibleTimer = 0   // 无敌计时器初始0
    };

    GameState gameState = GAME_TITLE;
    int currentLevel = 1;
    float levelTimer = 0;
    
    InitWindow(screen_width, screen_height, "mi");
    SetTargetFPS(60);
    
    Texture2D playerTexture = LoadTexture("../rsc/plane.png");
    Texture2D normalEnemyTexture = LoadTexture("../rsc/enemy.png");
    Texture2D eliteEnemyTexture = LoadTexture("../rsc/enemy_elite.png"); // 加载精英敌机材质
    Texture2D bossTexture = LoadTexture("../rsc/boss.png");  // 加载BOSS材质
    Texture2D backgroundTexture = LoadTexture("../rsc/background.png"); // 加载背景材质

    InitBullets();  
    SetCurrentLevel(currentLevel);
    InitEnemies(); 

    while (!WindowShouldClose()) {
        
        switch (gameState) {
            case GAME_TITLE:
                // 标题画面，按空格或回车开始游戏
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                    gameState = GAME_PLAYING;
                }
                break;
                
            case GAME_PLAYING:
                // 控制战机移动
                plane_movement(&player.x, &player.y);
                
                // 按J发射子弹
                if (IsKeyPressed(KEY_J)) {
                    FireBullet(player.x, player.y);
                }
                
                // 更新敌机和敌机子弹
                UpdateEnemies(normalEnemyTexture, (Vector2){player.x, player.y});
                UpdateEnemyBullets((Vector2){player.x, player.y});
                
                // 检测玩家与敌机子弹的碰撞
                if (!player.isInvincible && CheckPlayerEnemyBulletCollision(
                        (Vector2){player.x + playerTexture.width/2, player.y + playerTexture.height/2}, 
                        playerTexture.width/2)) {
                    player.lives--;
                    player.isInvincible = true;
                    player.invincibleTimer = 0;
                    
                    if (player.lives <= 0) {
                        gameState = GAME_OVER;
                    }
                }
                
                // 检测玩家子弹与敌机的碰撞（此处应该由mi实现）
                // 假设已有函数CheckBulletEnemyCollision(...)
                
                // 更新无敌状态
                if (player.isInvincible) {
                    player.invincibleTimer += GetFrameTime();
                    if (player.invincibleTimer >= 2.0f) {
                        player.isInvincible = false;
                    }
                }
                
                // 检查关卡是否完成
                if (IsLevelComplete()) {
                    gameState = GAME_LEVEL_COMPLETE;
                    levelTimer = 0;
                }
                break;
                
            case GAME_LEVEL_COMPLETE:
                // 关卡完成画面
                levelTimer += GetFrameTime();
                
                // 3秒后或按空格/回车键进入下一关
                if (levelTimer > 3.0f || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                    currentLevel++;
                    if (currentLevel > 10) {
                        // 通关，返回游戏结束画面
                        gameState = GAME_OVER;
                    } else {
                        // 设置新关卡并继续游戏
                        SetCurrentLevel(currentLevel);
                        InitEnemies();
                        gameState = GAME_PLAYING;
                    }
                }
                break;
                
            case GAME_OVER:
                // 游戏结束画面，按空格或回车重新开始
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                    // 重置游戏状态
                    player.lives = 3;
                    player.score = 0;
                    currentLevel = 1;
                    SetCurrentLevel(currentLevel);
                    InitEnemies();
                    gameState = GAME_TITLE;
                }
                break;
        }

        // 开始绘制
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        
   
        switch (gameState) {
            case GAME_TITLE:
                // 绘制标题画面
                DrawText("mi", screen_width/2 - MeasureText("mi", 50)/2, screen_height/3, 50, RED);
                DrawText("按空格或回车键开始游戏", screen_width/2 - MeasureText("按空格或回车键开始游戏", 20)/2, screen_height/2, 20, DARKGRAY);
                break;
                
            case GAME_PLAYING:
            case GAME_LEVEL_COMPLETE:
                // 绘制玩家飞机（无敌状态下闪烁）
                if (!player.isInvincible || ((int)(player.invincibleTimer * 10) % 2 == 0)) {
                    DrawTexture(playerTexture, player.x, player.y, WHITE);
                }
                
                UpdateAndDrawBullets();
                
                DrawEnemies(normalEnemyTexture, eliteEnemyTexture, bossTexture);
                
                DrawEnemyBullets();
                
                DrawGameUI(player, currentLevel, screen_width);
                
                if (gameState == GAME_LEVEL_COMPLETE) {
                    DrawRectangle(0, screen_height/2 - 50, screen_width, 100, Fade(BLACK, 0.7f));
                    DrawText("关卡完成!", screen_width/2 - MeasureText("关卡完成!", 40)/2, screen_height/2 - 20, 40, GREEN);
                    DrawText("准备进入下一关...", screen_width/2 - MeasureText("准备进入下一关...", 20)/2, screen_height/2 + 30, 20, WHITE);
                }
                break;
                
            case GAME_OVER:
                DrawRectangle(0, 0, screen_width, screen_height, Fade(BLACK, 0.8f));
                DrawText("游戏结束", screen_width/2 - MeasureText("游戏结束", 50)/2, screen_height/3, 50, RED);
                DrawText(TextFormat("最终得分: %d", player.score), screen_width/2 - MeasureText(TextFormat("最终得分: %d", player.score), 30)/2, screen_height/2, 30, WHITE);
                DrawText(TextFormat("达到关卡: %d", currentLevel), screen_width/2 - MeasureText(TextFormat("达到关卡: %d", currentLevel), 30)/2, screen_height/2 + 40, 30, WHITE);
                DrawText("按空格或回车键重新开始", screen_width/2 - MeasureText("按空格或回车键重新开始", 20)/2, screen_height*2/3, 20, GRAY);
                break;
        }

        EndDrawing();
    }

    // 卸载资源
    UnloadTexture(playerTexture);
    UnloadTexture(normalEnemyTexture);
    UnloadTexture(eliteEnemyTexture);
    UnloadTexture(bossTexture);
    UnloadTexture(backgroundTexture);
    
    CloseWindow();
    return 0;
}

// 绘制游戏UI
void DrawGameUI(Player player, int currentLevel, int screen_width) {
    // 绘制生命值
    DrawText("生命:", 20, 20, 20, WHITE);
    for (int i = 0; i < player.lives; i++) {
        DrawCircle(110 + i * 30, 30, 10, RED);
    }
    
    DrawText(TextFormat("得分: %d", player.score), screen_width - 200, 20, 20, WHITE);
    
    DrawText(TextFormat("关卡: %d", currentLevel), screen_width/2 - 50, 20, 20, WHITE);

    DrawRectangle(0, 60, screen_width, 30, Fade(BLACK, 0.5f));
    
    // 根据关卡显示不同的任务描述
    if (currentLevel <= 2) {
        DrawText("任务: 消灭所有普通敌机", screen_width/2 - MeasureText("任务: 消灭所有普通敌机", 20)/2, 65, 20, YELLOW);
    } else if (currentLevel <= 4) {
        DrawText("任务: 消灭普通敌机和精英敌机", screen_width/2 - MeasureText("任务: 消灭普通敌机和精英敌机", 20)/2, 65, 20, YELLOW);
    } else if (currentLevel % 3 == 0) { // 第3、6、9关
        DrawText("任务: 消灭所有敌机和精英敌机", screen_width/2 - MeasureText("任务: 消灭所有敌机和精英敌机", 20)/2, 65, 20, YELLOW);
    } else if (currentLevel % 5 == 0) { // 第5、10关
        DrawText("任务: 击败BOSS!", screen_width/2 - MeasureText("任务: 击败BOSS!", 20)/2, 65, 20, RED);
    } else {
        DrawText("任务: 消灭所有敌人", screen_width/2 - MeasureText("任务: 消灭所有敌人", 20)/2, 65, 20, YELLOW);
    }
}

// 检测玩家与敌机碰撞
bool CheckPlayerEnemyCollision(Player player, Texture2D playerTexture) {
    // 这里应该实现玩家与敌机的碰撞检测
    // 返回true表示发生碰撞，false表示没有碰撞
    
    return false;
}
