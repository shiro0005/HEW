#pragma once

#include "collision.h"


#define BOSS_COUNT 1

void Boss_Initialize(void);
void Boss_Finalize(void);
void Boss_Update(void);
void Boss_Draw(void);
void Boss_Destroy(int index);
bool Boss_IsEnable(int index);

int Boss_AddDamage(int damage);
int Boss_GetHitPoint();
const CIRCLE* Boss_GetCollision(int index);
void Boss_StateInit(int index);