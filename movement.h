//
// Created by qiguo on 25-5-23.
//

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "raylib.h"

Rectangle GetPlaneRect();
void plane_movement(int *position_x,int *position_y);
//传入对象的x,y坐标，使用WASD控制对象移动
#endif //MOVEMENT_H
