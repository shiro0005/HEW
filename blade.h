#pragma once
#include "collision.h"
#include <d3dx9.h>

//ƒuƒŒƒCƒh‚Íˆê‚Â‚µ‚©‚È‚¢
#define BLADE_MAX (1)


void Blade_Initialize(void);
void Blade_Update(void);
void Blade_Draw(void);

void Blade_Create(float x, float y, D3DXVECTOR2 dir);

void Blade_Destroy(int index);
bool Blade_IsEnable(int index);
const CIRCLE* Blade_GetCollision(int index);
