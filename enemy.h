#pragma once


#include "collision.h"


#define ENEMY_COUNT 10

void Enemy_Initialize(void);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
void Enemy_Destroy(int index);
bool Enemy_IsEnable(int index);
const CIRCLE* Enemy_GetCollision(int index);
