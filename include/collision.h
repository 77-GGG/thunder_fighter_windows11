//
// Created by q on 25-5-30.
//

#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "bullet.h"
#include "enemy.h"
#include "utils.h"


void CheckBulletEnemyCollisions(Bullet *playerBullets, Enemy *enemies);
bool CheckPlayerHit(Bullet *enemyBullets, Vector2 playerPos, int *hp);

#endif
