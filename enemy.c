//
// Created by Koishie on 25-5-29.
//

#include "enemy.h"
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

static Enemy enemies[MAX_ENEMIES];               // 敌机数组
static EnemyBullet bullets[MAX_ENEMY_BULLETS];   // 敌机子弹数组
static int frameCount = 0;                       // 帧计数器
static int currentLevel = 1;                     // 当前关卡
static int enemiesSpawned = 0;                   // 已生成的敌机数量
static int enemiesDefeated = 0;                  // 已击败的敌机数量
static float spawnTimer = 0;                     // 生成计时器

// 关卡配置
static const LevelData levelData[] = {
    // 第1关 - 仅有普通敌机
    {1, 10, 0, false, 1.0f, 1.0f, 0.0f, 0.0f, 1, 0, 0},
    
    // 第2关 - 普通敌机，生成速度更快
    {2, 15, 0, false, 0.8f, 0.9f, 0.0f, 0.0f, 1, 0, 0},
    
    // 第3关 - 引入精英敌机
    {3, 12, 3, false, 0.7f, 0.9f, 0.7f, 0.0f, 1, 2, 0},
    
    // 第4关 - 更多精英敌机
    {4, 10, 5, false, 0.6f, 0.8f, 0.6f, 0.0f, 1, 3, 0},
    
    // 第5关 - 首个BOSS关卡
    {5, 8, 4, true, 0.6f, 0.8f, 0.6f, 2.0f, 1, 3, 50},
    
    // 第6关 - 更难的普通和精英敌机
    {6, 15, 8, false, 0.5f, 0.7f, 0.5f, 0.0f, 2, 4, 0},
    
    // 第7关 - 更具攻击性的敌机
    {7, 12, 10, false, 0.4f, 0.6f, 0.4f, 0.0f, 2, 5, 0},
    
    // 第8关 - 第二个BOSS关卡，比第一个更难
    {8, 10, 8, true, 0.4f, 0.6f, 0.4f, 1.5f, 2, 5, 80},
    
    // 第9关 - 非常具有挑战性的关卡
    {9, 15, 12, false, 0.3f, 0.5f, 0.3f, 0.0f, 3, 6, 0},
    
    // 第10关 - 最终BOSS关卡
    {10, 8, 10, true, 0.3f, 0.5f, 0.3f, 1.0f, 3, 6, 120}
};

void InitEnemies(void) {
    // 初始化敌机数组
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }

    // 初始化子弹数组
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        bullets[i].active = false;
    }
    
    // 重置关卡统计数据
    frameCount = 0;
    enemiesSpawned = 0;
    enemiesDefeated = 0;
    spawnTimer = 0;
}

void SetCurrentLevel(int level) {
    // 设置当前关卡（确保在有效范围内）
    if (level > 0 && level <= 10) {
        currentLevel = level;
        InitEnemies();
    }
}

int GetCurrentLevel(void) {
    return currentLevel;
}

void SpawnEnemy(EnemyType type, Vector2 position) {
    // 在敌机数组中寻找一个未激活的位置来生成新的敌机
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].position = position;
            enemies[i].initialPosition = position;
            enemies[i].active = true;
            enemies[i].type = type;
            enemies[i].movementTimer = 0;
            enemies[i].attackTimer = 0;
            enemies[i].movementParam = (float)rand() / RAND_MAX * 2.0f * PI;
            
            // 根据敌机类型设置属性
            switch (type) {
                case ENEMY_NORMAL:  // 普通敌机
                    enemies[i].health = levelData[currentLevel-1].normalHealth;
                    enemies[i].speed = 2.0f;
                    enemies[i].fireRate = levelData[currentLevel-1].normalAttackRate;
                    enemies[i].movementPattern = MOVE_STRAIGHT;
                    enemies[i].attackPattern = ATTACK_SINGLE;
                    enemies[i].color = RED;
                    enemies[i].size = 1.0f;
                    break;
                    
                case ENEMY_ELITE:   // 精英敌机
                    enemies[i].health = levelData[currentLevel-1].eliteHealth;
                    enemies[i].speed = 3.0f;
                    enemies[i].fireRate = levelData[currentLevel-1].eliteAttackRate;
                    enemies[i].movementPattern = MOVE_RANDOM;
                    enemies[i].attackPattern = ATTACK_SPREAD;
                    enemies[i].color = PURPLE;
                    enemies[i].size = 1.2f;
                    break;
                    
                case ENEMY_BOSS:    // BOSS敌机
                    enemies[i].health = levelData[currentLevel-1].bossHealth;
                    enemies[i].speed = 1.5f;
                    enemies[i].fireRate = levelData[currentLevel-1].bossAttackRate;
                    enemies[i].movementPattern = MOVE_SINE_WAVE;
                    enemies[i].attackPattern = ATTACK_CIRCULAR;
                    enemies[i].color = GOLD;
                    enemies[i].size = 2.5f;
                    break;
            }
            
            enemiesSpawned++;
            break;
        }
    }
}

void SpawnBoss(void) {
    // 在屏幕中央生成BOSS
    SpawnEnemy(ENEMY_BOSS, (Vector2){400, 100});
}

void FireEnemyBullet(Vector2 pos, AttackPattern pattern, Vector2 playerPos) {
    // 根据不同的攻击模式发射子弹
    switch (pattern) {
        case ATTACK_SINGLE: {
            // 寻找一个未激活的子弹
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].position = (Vector2){pos.x, pos.y + 30};
                    bullets[i].velocity = (Vector2){0, 5};
                    bullets[i].active = true;
                    bullets[i].speed = 5.0f;
                    bullets[i].isTracking = false;
                    bullets[i].color = RED;
                    bullets[i].size = 5.0f;
                    bullets[i].damage = 1;
                    break;
                }
            }
            break;
        }
        
        case ATTACK_SPREAD: {
            // 发射扇形散射的3颗子弹
            float angles[3] = {-PI/6, 0, PI/6}; // -30°, 0°, 30°
            for (int j = 0; j < 3; j++) {
                for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].position = (Vector2){pos.x, pos.y + 30};
                        bullets[i].angle = angles[j];
                        bullets[i].velocity = (Vector2){
                            sinf(angles[j]) * 4.0f,
                            cosf(angles[j]) * 4.0f
                        };
                        bullets[i].active = true;
                        bullets[i].speed = 4.0f;
                        bullets[i].isTracking = false;
                        bullets[i].color = PURPLE;
                        bullets[i].size = 4.0f;
                        bullets[i].damage = 1;
                        break;
                    }
                }
            }
            break;
        }
        
        case ATTACK_BURST: {
            // 发射连发子弹（短时间内多发）
            // 通常应该由计时器处理，但为简化，我们发射2颗速度略有不同的子弹
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].position = (Vector2){pos.x, pos.y + 30};
                        bullets[i].velocity = (Vector2){0, 4.0f + j * 2.0f};
                        bullets[i].active = true;
                        bullets[i].speed = 4.0f + j * 2.0f;
                        bullets[i].isTracking = false;
                        bullets[i].color = ORANGE;
                        bullets[i].size = 4.0f;
                        bullets[i].damage = 1;
                        break;
                    }
                }
            }
            break;
        }
        
        case ATTACK_TRACKING: {
            // 发射跟踪子弹，追踪玩家
            for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].position = (Vector2){pos.x, pos.y + 30};
                    bullets[i].targetPosition = playerPos;
                    bullets[i].active = true;
                    bullets[i].speed = 3.0f;
                    bullets[i].isTracking = true;
                    bullets[i].color = BLUE;
                    bullets[i].size = 6.0f;
                    bullets[i].damage = 2;
                    break;
                }
            }
            break;
        }
        
        case ATTACK_CIRCULAR: {
            // 发射环形散射的子弹（8个方向）
            int numBullets = 8;
            for (int j = 0; j < numBullets; j++) {
                float angle = j * (2 * PI / numBullets);
                for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].position = (Vector2){pos.x, pos.y};
                        bullets[i].angle = angle;
                        bullets[i].velocity = (Vector2){
                            sinf(angle) * 3.0f,
                            cosf(angle) * 3.0f
                        };
                        bullets[i].active = true;
                        bullets[i].speed = 3.0f;
                        bullets[i].isTracking = false;
                        bullets[i].color = GOLD;
                        bullets[i].size = 5.0f;
                        bullets[i].damage = 2;
                        break;
                    }
                }
            }
            break;
        }
    }
}

void UpdateEnemies(Texture2D enemyTexture, Vector2 playerPosition) {
    frameCount++;
    spawnTimer += GetFrameTime();
    
    // 检查是否需要生成更多敌机
    LevelData level = levelData[currentLevel-1];
    int totalEnemies = level.normalEnemyCount + level.eliteEnemyCount + (level.hasBoss ? 1 : 0);
    
    // 随时间生成敌机
    if (enemiesSpawned < totalEnemies && spawnTimer >= level.enemySpawnRate) {
        spawnTimer = 0;
        
        // 确定要生成的敌机类型
        if (level.hasBoss && enemiesSpawned >= level.normalEnemyCount + level.eliteEnemyCount) {
            // 生成BOSS
            SpawnBoss();
        } else if (enemiesSpawned < level.normalEnemyCount) {
            // 在随机位置生成普通敌机
            float xPos = 50.0f + (float)(rand() % 700);
            SpawnEnemy(ENEMY_NORMAL, (Vector2){xPos, 50});
        } else {
            // 在随机位置生成精英敌机
            float xPos = 100.0f + (float)(rand() % 600);
            SpawnEnemy(ENEMY_ELITE, (Vector2){xPos, 80});
        }
    }

    // 更新每个激活的敌机
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        
        Enemy *enemy = &enemies[i];
        enemy->movementTimer += GetFrameTime();
        enemy->attackTimer += GetFrameTime();
        
        // 处理移动模式
        switch (enemy->movementPattern) {
            case MOVE_STRAIGHT:
                // 简单的向下移动
                enemy->position.y += enemy->speed;
                if (enemy->position.y > 1600) {
                    enemy->active = false;
                }
                break;
                
            case MOVE_RANDOM:
                // 每秒随机改变方向
                if (enemy->movementTimer >= 1.0f) {
                    enemy->movementTimer = 0;
                    enemy->velocity.x = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * enemy->speed;
                    enemy->velocity.y = ((float)rand() / RAND_MAX) * enemy->speed;
                }
                enemy->position.x += enemy->velocity.x;
                enemy->position.y += enemy->velocity.y;
                
                // 保持在屏幕边界内
                if (enemy->position.x < 0) enemy->position.x = 0;
                if (enemy->position.x > 750) enemy->position.x = 750;
                if (enemy->position.y > 1600) enemy->active = false;
                break;
                
            case MOVE_SINE_WAVE:
                // 正弦波移动
                enemy->movementParam += 0.03f;
                enemy->position.x = enemy->initialPosition.x + sinf(enemy->movementParam) * 200.0f;
                enemy->position.y += enemy->speed * 0.5f;
                
                // 保持在屏幕边界内
                if (enemy->position.x < 0) enemy->position.x = 0;
                if (enemy->position.x > 750) enemy->position.x = 750;
                if (enemy->position.y > 1600) enemy->active = false;
                break;
                
            case MOVE_CIRCLE:
                // 圆形移动
                enemy->movementParam += 0.02f;
                enemy->position.x = enemy->initialPosition.x + cosf(enemy->movementParam) * 150.0f;
                enemy->position.y = enemy->initialPosition.y + sinf(enemy->movementParam) * 100.0f;
                break;
                
            case MOVE_ZIGZAG:
                // 之字形移动
                enemy->movementTimer += 0.1f;
                if ((int)enemy->movementTimer % 2 == 0) {
                    enemy->position.x += enemy->speed;
                } else {
                    enemy->position.x -= enemy->speed;
                }
                enemy->position.y += enemy->speed * 0.5f;
                
                // 保持在屏幕边界内
                if (enemy->position.x < 0) enemy->position.x = 0;
                if (enemy->position.x > 750) enemy->position.x = 750;
                if (enemy->position.y > 1600) enemy->active = false;
                break;
        }
        
        // 根据敌机类型处理攻击模式
        if (enemy->attackTimer >= enemy->fireRate) {
            enemy->attackTimer = 0;
            FireEnemyBullet(enemy->position, enemy->attackPattern, playerPosition);
        }
    }
}

void DrawEnemies(Texture2D normalEnemyTexture, Texture2D eliteEnemyTexture, Texture2D bossTexture) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // 根据敌机类型绘制不同的纹理
            switch (enemies[i].type) {
                case ENEMY_NORMAL:
                    DrawTexture(normalEnemyTexture, enemies[i].position.x, enemies[i].position.y, WHITE);
                    break;
                case ENEMY_ELITE:
                    DrawTexture(eliteEnemyTexture, enemies[i].position.x, enemies[i].position.y, WHITE);
                    break;
                case ENEMY_BOSS:
                    DrawTexture(bossTexture, enemies[i].position.x, enemies[i].position.y, WHITE);
                    break;
            }
            
            // 为BOSS和精英敌机绘制血条
            if (enemies[i].type != ENEMY_NORMAL) {
                float healthPercentage = (float)enemies[i].health / 
                    (enemies[i].type == ENEMY_BOSS ? levelData[currentLevel-1].bossHealth : levelData[currentLevel-1].eliteHealth);
                
                DrawRectangle(
                    enemies[i].position.x, 
                    enemies[i].position.y - 10, 
                    60 * enemies[i].size, 
                    5, 
                    DARKGRAY
                );
                
                DrawRectangle(
                    enemies[i].position.x, 
                    enemies[i].position.y - 10, 
                    60 * enemies[i].size * healthPercentage, 
                    5, 
                    RED
                );
            }
        }
    }
}

void UpdateEnemyBullets(Vector2 playerPosition) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            // 根据速度更新非跟踪子弹的位置
            if (!bullets[i].isTracking) {
                bullets[i].position.x += bullets[i].velocity.x;
                bullets[i].position.y += bullets[i].velocity.y;
            } else {
                // 更新跟踪子弹以跟随玩家
                Vector2 direction = {
                    playerPosition.x - bullets[i].position.x,
                    playerPosition.y - bullets[i].position.y
                };
                
                // 归一化方向向量
                float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
                if (length > 0) {
                    direction.x /= length;
                    direction.y /= length;
                }
                
                // 朝向玩家移动
                bullets[i].position.x += direction.x * bullets[i].speed;
                bullets[i].position.y += direction.y * bullets[i].speed;
            }
            
            // 如果子弹超出屏幕，则停用
            if (bullets[i].position.y > 1600 || 
                bullets[i].position.y < -50 || 
                bullets[i].position.x < -50 || 
                bullets[i].position.x > 850) {
                bullets[i].active = false;
            }
        }
    }
}

void DrawEnemyBullets(void) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            DrawCircleV(bullets[i].position, bullets[i].size, bullets[i].color);
        }
    }
}

bool IsLevelComplete(void) {
    // 检查是否所有敌机都被击败，且不需要再生成更多敌机
    LevelData level = levelData[currentLevel-1];
    int totalEnemies = level.normalEnemyCount + level.eliteEnemyCount + (level.hasBoss ? 1 : 0);
    
    return (enemiesDefeated >= totalEnemies && enemiesSpawned >= totalEnemies);
}

int GetActiveEnemyCount(void) {
    int count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            count++;
        }
    }
    return count;
}

// 敌机受击时调用的函数
void HitEnemy(int enemyIndex, int damage) {
    if (enemies[enemyIndex].active) {
        enemies[enemyIndex].health -= damage;
        
        if (enemies[enemyIndex].health <= 0) {
            enemies[enemyIndex].active = false;
            enemiesDefeated++;
        }
    }
}

// 检查子弹是否击中敌机(?
bool CheckBulletEnemyCollision(Vector2 bulletPos, float bulletRadius) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            Rectangle enemyRect = {
                enemies[i].position.x,
                enemies[i].position.y,
                60 * enemies[i].size,
                60 * enemies[i].size
            };
            
            if (CheckCollisionCircleRec(bulletPos, bulletRadius, enemyRect)) {
                HitEnemy(i, 1);
                return true;
            }
        }
    }
    return false;
}

// 检查玩家是否与敌机子弹碰撞(?
bool CheckPlayerEnemyBulletCollision(Vector2 playerPos, float playerRadius) {
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (bullets[i].active) {
            if (CheckCollisionCircles(playerPos, playerRadius, bullets[i].position, bullets[i].size)) {
                bullets[i].active = false;
                return true;
            }
        }
    }
    return false;
}
