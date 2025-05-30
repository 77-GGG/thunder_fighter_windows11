//
// Created by q on 25-5-30.
//
#include "utils.h"
#include <math.h>

bool CheckCollisionCircle(Vector2 center1,float radius1,Vector2 center2,float radius2) {
    float dx = center1.x - center2.x;
    float dy = center1.y - center2.y;
    float distance = sqrtf(dx*dx + dy*dy);
    return distance <(radius1+radius2);
}
