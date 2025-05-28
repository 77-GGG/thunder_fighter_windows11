//
// Created by Koishie on 25-5-29.
//

#ifndef SHOOTER_ENEMY_H
#define SHOOTER_ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 50         // 最大敌机数量(haha
#define MAX_ENEMY_BULLETS 500  // 最大敌机子弹数量
#define MAX_BOSS_BULLETS 200   // 最大BOSS子弹数量

// 敌机类型，具有不同的行为模式
typedef enum {
    ENEMY_NORMAL,    // 基本类型：直线移动
    ENEMY_ELITE,     // 精英类型：随机方向变化，发射更多子弹
    ENEMY_BOSS       // BOSS类型：复杂的移动和子弹模式
} EnemyType;

// 移动模式
typedef enum {
    MOVE_STRAIGHT,   // 直线移动
    MOVE_RANDOM,     // 随机方向变化
    MOVE_SINE_WAVE,  // 正弦波移动模式
    MOVE_CIRCLE,     // 圆形移动模式
    MOVE_ZIGZAG      // 之字形移动模式
} MovementPattern;

// 攻击模式
typedef enum {
    ATTACK_SINGLE,   // 单发子弹
    ATTACK_SPREAD,   // 扇形散射子弹
    ATTACK_BURST,    // 短时间内发射多发子弹
    ATTACK_TRACKING, // 跟踪玩家的子弹
    ATTACK_CIRCULAR  // 环形散射子弹
} AttackPattern;

// 关卡数据结构
typedef struct {
    int levelNumber;         // 关卡编号
    int normalEnemyCount;    // 普通敌机数量
    int eliteEnemyCount;     // 精英敌机数量
    bool hasBoss;            // 是否有BOSS
    float enemySpawnRate;    // 敌机生成速率
    float normalAttackRate;  // 普通敌机攻击速率
    float eliteAttackRate;   // 精英敌机攻击速率
    float bossAttackRate;    // BOSS攻击速率
    int normalHealth;        // 普通敌机生命值
    int eliteHealth;         // 精英敌机生命值
    int bossHealth;          // BOSS生命值
} LevelData;

// 增强型敌机结构
typedef struct {
    Vector2 position;            // 位置
    Vector2 velocity;            // 速度
    Vector2 initialPosition;     // 初始位置
    bool active;                 // 是否激活
    EnemyType type;              // 敌机类型
    MovementPattern movementPattern; // 移动模式
    AttackPattern attackPattern;  // 攻击模式
    int health;                  // 生命值
    float speed;                 // 速度
    float fireRate;              // 开火速率
    float attackTimer;           // 攻击计时器
    float movementParam;         // 移动参数，用于参数化移动模式
    float movementTimer;         // 移动计时器，用于控制移动变化
    Color color;                 // 颜色
    float size;                  // 尺寸乘数，用于绘制
} Enemy;

// 增强型子弹结构
typedef struct {
    Vector2 position;        // 位置
    Vector2 velocity;        // 速度
    bool active;             // 是否激活
    float speed;             // 速度
    float angle;             // 角度
    bool isTracking;         // 是否为跟踪子弹
    Vector2 targetPosition;  // 目标位置
    Color color;             // 颜色
    float size;              // 尺寸
    int damage;              // 伤害值
} EnemyBullet;


void InitEnemies(void);                      // 初始化敌机
void SetCurrentLevel(int level);             // 设置当前关卡
int GetCurrentLevel(void);                   // 获取当前关卡
void SpawnEnemy(EnemyType type, Vector2 position); // 生成敌机
void SpawnBoss(void);                        // 生成BOSS
void UpdateEnemies(Texture2D enemyTexture, Vector2 playerPosition); // 更新敌机
void DrawEnemies(Texture2D normalEnemyTexture, Texture2D eliteEnemyTexture, Texture2D bossTexture); // 绘制敌机
void FireEnemyBullet(Vector2 pos, AttackPattern pattern, Vector2 playerPos); // 发射敌机子弹
void UpdateEnemyBullets(Vector2 playerPosition); // 更新敌机子弹
void DrawEnemyBullets(void);                 // 绘制敌机子弹
bool IsLevelComplete(void);                  // 判断关卡是否完成
int GetActiveEnemyCount(void);               // 获取活跃敌机数量
void HitEnemy(int enemyIndex, int damage);   // 敌机受击
bool CheckBulletEnemyCollision(Vector2 bulletPos, float bulletRadius); // 检测子弹与敌机碰撞
bool CheckPlayerEnemyBulletCollision(Vector2 playerPos, float playerRadius); // 检测玩家与敌机子弹碰撞

#endif //SHOOTER_ENEMY_H
