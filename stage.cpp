#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "stage.h"
#include "player.h"
#include <time.h>
#include <stdlib.h>


#define BGNUM 5
#define UISIZE 270
#define MAXCOUNT 3	//何ステージ進んだらゴール

/*float x1;//スクロールテクスチャ1の位置
float x2;//スクロールテクスチャ2の位置
bool flg1;//スクロールテクスチャ1を動かすフラグ
bool flg2;//スクロールテクスチャ2を動かすフラグ
bool flg_bgchange1, flg_bgchange2;
int bgnum1, bgnum2;*/

static float Playerpos;

typedef struct
{
	D3DXVECTOR2 pos; //スクロールテクスチャの位置

}FIELD_DATA;

FIELD_DATA field[2];

/*

配列で背景の画像を管理して描画させる

*/


void Field1_Initialize(void)
{
	//初期位置を設定
	field[0].pos.x = 0;
	field[1].pos.x = 0;
	field[0].pos.y = 0;
	field[1].pos.y = 0;

	srand((unsigned int)time(NULL));
}


void Field1_Update(void)
{
	field[0].pos.x -= Player_Getdir();//背景をプレイヤーの移動と合わせる
	if (field[0].pos.x != 0) {
		if (field[0].pos.x > 0) {
			field[1].pos.x = field[0].pos.x - 1024.0f;
			//field[1].pos.x -= Player_Getdir();
		}
		else {
			field[1].pos.x = field[0].pos.x + 1024.0f;
			//field[1].pos.x -= Player_Getdir();
		}
	}

	if (field[0].pos.x > 1024.0f) {
		field[0].pos.x = field[1].pos.x - 1024.0f;
	}
	else if (field[0].pos.x < -1024.0f) {
		field[0].pos.x = field[1].pos.x + 1024.0f;
	}



	//count++;
	//if (count == 100)
	//{
	//	StopScroll = 0;
	//}
	//if (field[0].flg_change == 1)
	//{
	//	field[0].type = (rand() % (BGNUM - 2)) + 1;//BG1をランダム抽選
	//	field[0].flg_change = 0;
	//	field1count++;
	//}
	//if (field[1].flg_change == 1)
	//{
	//	field[1].type = (rand() % (BGNUM - 2)) + 1;//BG2をランダム抽選
	//	field[1].flg_change = 0;
	//	field1count++;
	//}
	//if (field1count == MAXCOUNT)
	//{
	//	field[1].type = 4;
	//}
	//if (StopScroll == 0)
	//{
	//	field[0].speed = field[0].speed + GetPlayerSpeed();
	//	field[1].speed = field[1].speed + GetPlayerSpeed();
	//	/*MinuPlayerSpeed(GetPlayerSpeed());*/
	//}

}

void Field1_Finalize(void)
{

}

void Field1_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_BGMAIN, field[0].pos.x, 0.0f);

	Sprite_Draw(TEXTURE_INDEX_BGMAIN, field[1].pos.x, 0.0f);

	/*if (field[0].flg_move == 1)
	{
		if (StopScroll == 0)
			field[0].pos.y = field[0].pos.y + field[0].speed;
		switch (field[0].type)
		{
		case 0:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[0].pos.y);
			break;
		case 1:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[0].pos.y);
			break;
		case 2:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[0].pos.y);
			break;
		case 3:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[0].pos.y);
			break;
		case 4:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[0].pos.y);
			break;
		}
		if (field[0].pos.y > 0 - UISIZE)
		{
			field[1].flg_move = 1;
		}
		if (field[0].pos.y > 1080 - UISIZE)
		{
			field[0].pos.y = -1080 + field[0].speed - UISIZE;
			field[0].flg_change = 1;
			field[0].flg_move = 0;
		}

		if (field[0].type == 4 && field[0].pos.y > 0)
		{
			field[0].pos.y = 0;
			StopScroll = 1;
		}
	}

	if (field[1].flg_move == 1)
	{
		if (StopScroll == 0)
			field[1].pos.y = field[1].pos.y + field[1].speed;
		switch (field[1].type)
		{
		case 0:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[1].pos.y);
			break;
		case 1:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[1].pos.y);
			break;
		case 2:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[1].pos.y);
			break;
		case 3:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[1].pos.y);
			break;
		case 4:Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0, field[1].pos.y);
			break;
		}
		if (field[1].pos.y > 0 - UISIZE)
		{
			field[0].flg_move = 1;
		}
		if (field[1].pos.y > 1080 - UISIZE)
		{
			field[1].pos.y = -1080;
			field[1].flg_change = 1;
			field[1].flg_move = 0;
		}

		if (field[1].type == 4 && field[1].pos.y > 0)
		{
			field[1].pos.y = 0;
			StopScroll = 1;
		}
	}
	*/
}

