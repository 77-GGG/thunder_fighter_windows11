//
// Created by qiguo on 25-5-24.
//
#include "raylib.h"
#include "movement.h"
#include "bullet.h"
#include "include/enemy.h"
#include "collision.h"

typedef struct {
    float startTime;       
    float normalEnemyChance; 
    float laserEnemyChance;  
    float zigzagEnemyChance; 
    float spawnRate;         
} GameWave;

void SpawnEnemyBasedOnTime(Enemy *enemies, float gameTime, GameWave *waves, int waveCount) {
    int currentWave = 0;
    for (int i = 0; i < waveCount - 1; i++) {
        if (gameTime >= waves[i].startTime && gameTime < waves[i+1].startTime) {
            currentWave = i;
            break;
        }
    }
    if (gameTime >= waves[waveCount-1].startTime) {
        currentWave = waveCount - 1;
    }
    
    if (GetRandomValue(0, 100) > waves[currentWave].spawnRate) {
        return;
    }
    
    int emptySlot = -1;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            emptySlot = i;
            break;
        }
    }
    
    if (emptySlot == -1) return; 
    
    float totalChance = waves[currentWave].normalEnemyChance + 
                       waves[currentWave].laserEnemyChance + 
                       waves[currentWave].zigzagEnemyChance;
    
    float roll = GetRandomValue(0, 100) / 100.0f * totalChance;
    EnemyType type;
    
    if (roll < waves[currentWave].normalEnemyChance) {
        type = ENEMY_NORMAL;
    } else if (roll < waves[currentWave].normalEnemyChance + waves[currentWave].laserEnemyChance) {
        type = ENEMY_LASER;
    } else {
        type = ENEMY_ZIGZAG;
    }
    
    enemies[emptySlot].position = (Vector2){GetRandomValue(50, 750), -50};
    enemies[emptySlot].active = true;
    enemies[emptySlot].type = type;

    switch (type) {
        case ENEMY_NORMAL:
            enemies[emptySlot].hp = 3;
            break;
        case ENEMY_LASER:
            enemies[emptySlot].hp = 4;
            enemies[emptySlot].timer = 0;
            break;
        case ENEMY_ZIGZAG:
            enemies[emptySlot].hp = 2;
            enemies[emptySlot].amplitude = 50 + (GetRandomValue(0, 50));
            enemies[emptySlot].frequency = 0.02f + ((float)(GetRandomValue(0, 100)) / 1000.0f);
            enemies[emptySlot].baseX = enemies[emptySlot].position.x;
            enemies[emptySlot].timer = (float)(GetRandomValue(0, 628)) / 100.0f;
            break;
    }
}

int main(void) {
    const int screen_width = 800;
    const int screen_height = 1200;
    InitWindow(screen_width, screen_height, "thunder_fighter");
    SetTargetFPS(60);

    // 玩家相关
    Vector2 playerPos = {400, 600};  // 玩家初始位置
    Texture plane = LoadTexture("../rsc/plane.png");
    int playerHp = 3;//玩家的生命值

    float gameTime = 0.0f;

    GameWave waves[] = {
        {0.0f,      100.0f,        0.0f,          0.0f,           20.0f},  
        {30.0f,     80.0f,         20.0f,         0.0f,           30.0f},  
        {60.0f,     60.0f,         20.0f,         20.0f,          40.0f},  
        {90.0f,     40.0f,         30.0f,         30.0f,          50.0f}, 
        {120.0f,    30.0f,         35.0f,         35.0f,          60.0f},  
        {180.0f,    20.0f,         40.0f,         40.0f,          70.0f}, 
    };
    int waveCount = sizeof(waves) / sizeof(waves[0]);
    int currentDisplayWave = 1; 

    Texture enemy_texture = LoadTexture("../rsc/enemy.png");
    // 初始化模块
    Bullet playerBullets[MAX_BULLETS];
    Bullet enemyBullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    LoadEnemyTexture();
    InitBullets(playerBullets);
    InitBullets(enemyBullets);
    InitEnemies(enemies);

     for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }
    

    while (!WindowShouldClose()) {

        gameTime += GetFrameTime();
        
        for (int i = 0; i < waveCount - 1; i++) {
            if (gameTime >= waves[i].startTime && gameTime < waves[i+1].startTime) {
                currentDisplayWave = i + 1;
                break;
            }
        }
        if (gameTime >= waves[waveCount-1].startTime) {
            currentDisplayWave = waveCount;
        }

        SpawnEnemyBasedOnTime(enemies, gameTime, waves, waveCount);

        movement(&playerPos.x, &playerPos.y);

        if (IsKeyPressed(KEY_J)) {
            Vector2 bulletVel = {0, -10};  
            Vector2 bulletPos = {playerPos.x + 64, playerPos.y}; 
            FireBullet(playerBullets, bulletPos, bulletVel);
        }

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

        DrawText(TextFormat("time: %.1f", gameTime), 10, 70, 20, BLACK);
        DrawText(TextFormat("part: %d", currentDisplayWave), 10, 40, 20, BLACK);
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
