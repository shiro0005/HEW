
#include "enemy.h"
#include "main.h"
#include "sprite.h"
#include "table.h"
#include "player.h"
#include "explosion.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ENEMY_WIDTH				(32)
#define ENEMY_HEIGHT			(32)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define ENEMY_SEARCH_RADIUS		(200.0f)
#define ENEMY_SEARCH_ANGLE		(0.85f)
#define ENEMY_CHASE_SPEED		(0.4f)
#define ENEMY_RETURN_SPEED		(1.0f)

typedef struct
{
	int			enable;	//生存フラグ

	D3DXVECTOR2 pos;	//位置
	float		rot;	//回転
	int			muki;	//向き（移動方向）

	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト

	CIRCLE		collision;

	int			frame;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 pos_return;

}ENEMY_DATA;

ENEMY_DATA enemy[ENEMY_COUNT];


typedef enum
{
	ENEMY_STATE_INIT,		//初期化
	ENEMY_STATE_CHASE,		//追跡
	ENEMY_STATE_DEAD,		//死亡状態

	ENEMY_STATE_MAX

}ENEMY_STATE;

void Enemy_StateInit(int index);
void Enemy_StateChase(int index);


void Enemy_Initialize(void)
{
	//敵の初期化
	for (int i = 0; i < ENEMY_COUNT; i++) {
		Enemy_StateInit(i);
		enemy[i].state = ENEMY_STATE_CHASE;
	}
	
}

void Enemy_Finalize(void)
{

}

void Enemy_Update(void)
{
	for (int i = 0; i<ENEMY_COUNT; i++){

		//当たり判定用座標の更新
		enemy[i].collision.cx = enemy[i].pos.x;
		enemy[i].collision.cy = enemy[i].pos.y;

		//スキップするフレーム値を超えたら
		if (++enemy[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//アニメパターンを進める(最大値を超えたらリセット)
			if (++enemy[i].animePattern >= ANIME_PATTERN_MAX)
				enemy[i].animePattern = 0;
			//フレームは元に戻す
			enemy[i].animeFrame = 0;

			enemy[i].pos.x -= Player_Getdir()*10;//エネミーをプレイヤーの移動と合わせる
		}


		switch (enemy[i].state)
		{
		case ENEMY_STATE_INIT:
			Enemy_StateInit(i);
			break;
		case ENEMY_STATE_CHASE:
			Enemy_StateChase(i);
			break;
		default:
			break;
		}
	}
}

void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++){

		//生存フラグがTRUEの敵のみ処理を行う
		if (!enemy[i].enable) {
			continue;
		}

		Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
			enemy[i].pos.x,
			enemy[i].pos.y,
			GetAnimTbl(enemy[i].muki, enemy[i].animePattern).x * 256,
			GetAnimTbl(enemy[i].muki, enemy[i].animePattern).y * 256,
			32,
			32,
			16,
			16,
			2.0f,
			2.0f,
			enemy[i].rot);
	}
}

void Enemy_Destroy(int index)
{
	enemy[index].state = ENEMY_STATE_DEAD;
	enemy[index].enable = false;
}

bool Enemy_IsEnable(int index)
{
	return enemy[index].enable;
}

const CIRCLE* Enemy_GetCollision(int index)
{
	return &enemy[index].collision;
}

void Enemy_StateInit(int index)//エネミー出現情報
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemy[index].rot = 0;

		enemy[index].muki = 0;
		enemy[index].enable = TRUE;

		enemy[index].collision.cx = enemy[index].pos.x;
		enemy[index].collision.cy = enemy[index].pos.y;
		enemy[index].collision.r = ENEMY_WIDTH * 0.8f;

		enemy[index].frame = 0;
	}
	enemy[0].pos = { 1000,400 };
	enemy[1].pos = { 1500,400 };
	enemy[2].pos = { 2000,400 };
}



void Enemy_StateChase(int index)
{
	//フレームを進める
	enemy[index].frame++;

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->cx, Player_GetCollision()->cy);

	D3DXVECTOR2 dir = pl_pos - enemy[index].pos;
	D3DXVec2Normalize(&dir, &dir);
	dir *= ENEMY_CHASE_SPEED;

	enemy[index].pos.x += dir.x;
}

