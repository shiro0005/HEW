#pragma once

#include "collision.h"
#include <d3dx9.h>

//íeÇÃç≈ëÂï\é¶êî
#define BOSSBULLET_MAX (1400)


void BossBullet_Initialize(void);
void BossBullet_Update(void);
void BossBullet_Draw(void);

void BossBullet_Create(float x, float y, D3DXVECTOR2 dir);

void BossBullet_Destroy(int index);
bool BossBullet_IsEnable(int index);
const CIRCLE* BossBullet_GetCollision(int index);
