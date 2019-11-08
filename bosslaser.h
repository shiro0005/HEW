#pragma once

#include "collision.h"
#include <d3dx9.h>

//íeÇÃç≈ëÂï\é¶êî
#define BOSSLASER_MAX (10)


void BossLaser_Initialize(void);
void BossLaser_Update(void);
void BossLaser_Draw(void);

void BossLaser_Create(float x, float y, D3DXVECTOR2 dir);

void BossLaser_Destroy(int index);
bool BossLaser_IsEnable(int index);
const CUPSULE* BossLaser_GetCollision(int index);