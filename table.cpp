
#include "table.h"

D3DXVECTOR2 animeTable[4][3] =
{
		{//���F�@������
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(0.125f, 0.0f),
			D3DXVECTOR2(0.25f, 0.0f),
		},
		{//���F�@������
			D3DXVECTOR2(0.0f, 0.125f),
			D3DXVECTOR2(0.125f, 0.125f),
			D3DXVECTOR2(0.25f, 0.125f),
		},
		{//���F�@�E����
			D3DXVECTOR2(0.0f, 0.25f),
			D3DXVECTOR2(0.125f, 0.25f),
			D3DXVECTOR2(0.25f, 0.25f),
		},
		{//���F�@�����
			D3DXVECTOR2(0.0f, 0.375f),
			D3DXVECTOR2(0.125f, 0.375f),
			D3DXVECTOR2(0.25f, 0.375f),
		}
};

D3DXVECTOR2 GetAnimTbl(int muki, int anim)
{
	return animeTable[muki][anim];
}
