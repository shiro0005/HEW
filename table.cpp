
#include "table.h"

D3DXVECTOR2 animeTable[4][3] =
{
		{//水色　下向き
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(0.125f, 0.0f),
			D3DXVECTOR2(0.25f, 0.0f),
		},
		{//水色　左向き
			D3DXVECTOR2(0.0f, 0.125f),
			D3DXVECTOR2(0.125f, 0.125f),
			D3DXVECTOR2(0.25f, 0.125f),
		},
		{//水色　右向き
			D3DXVECTOR2(0.0f, 0.25f),
			D3DXVECTOR2(0.125f, 0.25f),
			D3DXVECTOR2(0.25f, 0.25f),
		},
		{//水色　上向き
			D3DXVECTOR2(0.0f, 0.375f),
			D3DXVECTOR2(0.125f, 0.375f),
			D3DXVECTOR2(0.25f, 0.375f),
		}
};

D3DXVECTOR2 GetAnimTbl(int muki, int anim)
{
	return animeTable[muki][anim];
}
