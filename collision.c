//
// Created by q on 25-5-30.
//
#include "collision.h"
#include "utils.h"
#include <stdlib.h>





void CheckBulletEnemyCollisions(Bullet *playerBullets, Enemy *enemies, int *score) {
    //增加得分参数
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (!playerBullets[j].active) continue;

            if (CheckCollisionCircle(enemies[i].position, 32, playerBullets[j].position, 10)) {
                playerBullets[j].active = false;
                enemies[i].hp--;
                if (enemies[i].hp <= 0) {
                    enemies[i].active = false;
                    //敌机被摧毁时增加得分
                    if (score) *score += 10;
                }
            }
        }
    }
}

bool CheckPlayerHit(Bullet *enemyBullets, Vector2 playerPos, int *hp) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!enemyBullets[i].active) continue;

        if (CheckCollisionCircle(playerPos, 32, enemyBullets[i].position, 10)) {
            enemyBullets[i].active = false;
            (*hp)--;
            return true;
        }
    }
    return false;
}
