#include "boss.h"
#include "main.h"
#include "sprite.h"
#include "table.h"
#include "player.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "explosion.h"


#define _USE_MATH_DEFINES
#include <math.h>

#define BOSS_WIDTH				(32)
#define BOSS_HEIGHT			(32)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define BOSS_SEARCH_RADIUS		(200.0f)
#define BOSS_SEARCH_ANGLE		(0.85f)
#define BOSS_CHASE_SPEED		(3.0f)
#define BOSS_RETURN_SPEED		(1.0f)

typedef struct
{
	int			enable;	//生存フラグ

	D3DXVECTOR2 pos;	//位置
	float		rot;	//回転
	int			color;	//色
	int			muki;	//向き（移動方向）

	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト

	CUPSULE		collision;

	int			frame;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_shot;
	D3DXVECTOR2 pos_return;

	int			bosshitpoint;		//ヒットポイント

}BOSS_DATA;

BOSS_DATA boss[BOSS_COUNT];


typedef enum
{
	BOSS_STATE_INIT,		//初期化
	BOSS_STATE_SEARCH,		//周囲をサーチ
	BOSS_STATE_FIND,		//敵を発見
	BOSS_STATE_CHASE,		//追跡
	BOSS_STATE_SHOT,		//ショット発射
	BOSS_STATE_LASER,		//レーザー発射
	BOSS_STATE_COOLDOWN,	//ショット終了
	BOSS_STATE_RETURN,		//定位置に戻る
	BOSS_STATE_DEAD,		//死亡状態

	BOSS_STATE_MAX

}ENEMY_STATE;


int Boss_CalcMuki(D3DXVECTOR2 dir);


void Boss_StateSearch(int index);
void Boss_StateFind(int index);
void Boss_StateChase(int index);
void Boss_StateShot(int index);
void Boss_StateLaser(int index);
void Boss_StateCooldown(int index);
void Boss_StateReturn(int index);


void Boss_Initialize(void)
{
	//敵の初期化
	for (int i = 0; i < BOSS_COUNT; i++) {
//		Boss_StateInit(i);
		boss[i].enable = FALSE;
		boss[i].state = BOSS_STATE_SEARCH;

		boss[i].bosshitpoint = 10000;
	}
}

void Boss_Finalize(void)
{

}

void Boss_Update(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		//当たり判定用座標の更新
		boss[i].collision.x = boss[i].pos.x;
		boss[i].collision.y = boss[i].pos.y;

		//スキップするフレーム値を超えたら
		if (++boss[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//アニメパターンを進める(最大値を超えたらリセット)
			if (++boss[i].animePattern >= ANIME_PATTERN_MAX)
				boss[i].animePattern = 0;
			//フレームは元に戻す
			boss[i].animeFrame = 0;
		}

		switch (boss[i].state)
		{
		case BOSS_STATE_INIT:
			Boss_StateInit(i);
			break;
		case BOSS_STATE_SEARCH:
			Boss_StateSearch(i);
			break;
		case BOSS_STATE_FIND:
			Boss_StateFind(i);
			break;
		case BOSS_STATE_CHASE:
			Boss_StateChase(i);
			break;
		case BOSS_STATE_SHOT:
			Boss_StateShot(i);
			break;
		case BOSS_STATE_LASER:
			Boss_StateLaser(i);
			break;
		case BOSS_STATE_COOLDOWN:
			Boss_StateCooldown(i);
			break;
		case BOSS_STATE_RETURN:
			Boss_StateReturn(i);
			break;
		default:
			break;
		}
	}
}

void Boss_Draw(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		//生存フラグがTRUEの敵のみ処理を行う
		if (!boss[i].enable) {
			continue;
		}

		Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
			boss[i].pos.x,
			boss[i].pos.y,
			GetAnimTbl(boss[i].color, boss[i].muki, boss[i].animePattern).x * 256,
			GetAnimTbl(boss[i].color, boss[i].muki, boss[i].animePattern).y * 256,
			32,
			32,
			16,
			16,
			8.0f,
			8.0f,
			boss[i].rot);
	}
}

void Boss_Destroy(int index)
{
	boss[index].state = BOSS_STATE_DEAD;
	boss[index].enable = false;
}

bool Boss_IsEnable(int index)
{
	return boss[index].enable;
}

const CUPSULE* Boss_GetCollision(int index)
{
	return &boss[index].collision;
}

//向きを作る
int Boss_CalcMuki(D3DXVECTOR2 dir)
{
	int muki = 0;

	//移動方向が有効かチェック
	if (D3DXVec2Length(&dir) > 0.01f)
	{
		//X成分とY成分の大きさを比べて、どちらの方が大きいのかを調べる
		//X成分の方が大きい場合
		if (fabs(dir.x) > fabs(dir.y))
		{
			//X成分がプラスの値なのかマイナスの値なのかを調べる
			//マイナス成分の場合は左向き
			if (dir.x < 0.0f)
				muki = 1;
			//そうじゃない（プラス成分）場合は右向き
			else
				muki = 2;
		}
		//Y成分の方が大きい場合
		else
		{
			//Y成分がプラスの値なのかマイナスの値なのかを調べる
			//マイナス成分の場合は上向き
			if (dir.y < 0.0f)
				muki = 3;
			//そうじゃない（プラス成分）場合は上向き
			else
				muki = 0;
		}
	}

	return muki;
}

void Boss_StateInit(int index)//ボス出現情報
{
	boss[index].pos.x = frand() * SCREEN_WIDTH;
	boss[index].pos.y = frand() * SCREEN_HEIGHT;
	boss[index].rot = 0;
	boss[index].color = 1;

	boss[index].muki = rand() % 4;
	boss[index].enable = TRUE;

	boss[index].collision.x = boss[index].pos.x;
	boss[index].collision.y = boss[index].pos.y;
	boss[index].collision.r = BOSS_WIDTH * 0.8f;

	boss[index].frame = 0;

	//ステートをサーチ状態へ移行
	boss[index].state = BOSS_STATE_SEARCH;

}

void Boss_StateSearch(int index)
{
	//フレームを進める
	boss[index].frame++;

	D3DXVECTOR2 dir;

	//向いている方向のベクトルを作る
	switch (boss[index].muki)
	{
	case 0://下向き
		dir = D3DXVECTOR2(0.0f, 1.0f);
		break;
	case 1://左向き
		dir = D3DXVECTOR2(-1.0f, 0.0f);
		break;
	case 2://右向き
		dir = D3DXVECTOR2(1.0f, 0.0f);
		break;
	case 3://上向き
		dir = D3DXVECTOR2(0.0f, -1.0f);
		break;
	}

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);

	//敵とプレイヤーの距離を計算する
	D3DXVECTOR2 vLen = boss[index].pos - pl_pos;
	float length = D3DXVec2Length(&vLen);

	//判定する距離の中に入っているかをチェックする
	if (length < BOSS_SEARCH_RADIUS)
	{
		//敵から見てプレイヤーの座標への方向ベクトルを作る
		D3DXVECTOR2 pl_dir = pl_pos - boss[index].pos;
		D3DXVec2Normalize(&pl_dir, &pl_dir);

		//敵の向きとプレイヤ座標への方向ベクトルで内積を取る
		float dot = D3DXVec2Dot(&dir, &pl_dir);

		//判定する角度内に入っているかをチェックする
		if (dot > BOSS_SEARCH_ANGLE)
		{
			Explosion_Create(boss[index].pos.x, boss[index].pos.y);

			//判定に通ればプレイヤー発見ステートに移行する
			boss[index].state = BOSS_STATE_FIND;
			boss[index].frame = 0;

			//戻って来る座標を保存する
			boss[index].pos_return = boss[index].pos;
		}
	}

	//向きを変化させる
	if (boss[index].frame > 60)
	{
		boss[index].muki = (boss[index].muki + 1) % 4;
		boss[index].frame = 0;
	}

}

void Boss_StateFind(int index)
{
	//フレームを進める
	boss[index].frame++;

	//一定時間経ったら追跡ステートへ移行
	if (boss[index].frame > 20) {

		boss[index].frame = 0;

		//ステートをサーチ状態へ移行
		boss[index].state = BOSS_STATE_CHASE;
	}
}

void Boss_StateChase(int index)
{
	//フレームを進める
	boss[index].frame++;

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);

	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
	D3DXVec2Normalize(&dir, &dir);
	dir *= BOSS_CHASE_SPEED;

	boss[index].pos += dir;

	//移動方向から向きを作る
	boss[index].muki = Boss_CalcMuki(dir);

	//一定時間経ったら弾射出ステートへ移行
	if (boss[index].frame > 120) {

		boss[index].frame = 0;

		//移動方向を保存
		boss[index].dir_shot = dir;

		//ランダムで「ばら撒き弾」もしくは「レーザー」を射出
		if (rand() % 2)
		{
			//ステートをサーチ状態へ移行
			boss[index].state = BOSS_STATE_SHOT;
		}
		else {
			//ステートをサーチ状態へ移行
			boss[index].state = BOSS_STATE_LASER;
		}
	}
}

void Boss_StateShot(int index)
{
	//フレームを進める
	boss[index].frame++;

	float shot_rad = atan2(boss[index].dir_shot.y, boss[index].dir_shot.x);
	shot_rad = (float)(M_PI * 2 / 20)*boss[index].frame;

	//射出角ベクトルをフレーム値で回転させる
	D3DXVECTOR2 shot_dir;
	shot_dir.x = boss[index].dir_shot.x*cosf(shot_rad) - boss[index].dir_shot.y*sinf(shot_rad);
	shot_dir.y = boss[index].dir_shot.x*sinf(shot_rad) + boss[index].dir_shot.y*cosf(shot_rad);

	BossBullet_Create(boss[index].pos.x, boss[index].pos.y, shot_dir);

	//一定時間経ったらクールダウンステートへ移行
	if (boss[index].frame > 90) {

		boss[index].frame = 0;

		//ステートを帰還状態へ移行
		boss[index].state = BOSS_STATE_COOLDOWN;
	}
}

void Boss_StateLaser(int index)
{
	if (boss[index].frame == 0) {
		BossLaser_Create(boss[index].pos.x, boss[index].pos.y, boss[index].dir_shot);
	}

	//フレームを進める
	boss[index].frame++;


	//一定時間経ったらクールダウンステートへ移行
	if (boss[index].frame > 90) {

		boss[index].frame = 0;

		//ステートを帰還状態へ移行
		boss[index].state = BOSS_STATE_COOLDOWN;
	}
}

void Boss_StateCooldown(int index)
{
	//フレームを進める
	boss[index].frame++;

	//一定時間経ったら帰還ステートへ移行
	if (boss[index].frame > 30) {

		boss[index].frame = 0;

		//ステートを帰還状態へ移行
		boss[index].state = BOSS_STATE_RETURN;
	}
}

void Boss_StateReturn(int index)
{
	//フレームを進める
	boss[index].frame++;

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);

	//帰還する座標への向きベクトルを計算する
	D3DXVECTOR2 dir = boss[index].pos_return - boss[index].pos;

	//ベクトルの長さを計算する
	float length = D3DXVec2Length(&dir);

	D3DXVec2Normalize(&dir, &dir);
	dir *= BOSS_CHASE_SPEED;

	boss[index].pos += dir;

	//移動方向から向きを作る
	boss[index].muki = Boss_CalcMuki(dir);

	//帰還する座標に到着したら
	if (length <= BOSS_CHASE_SPEED) {

		boss[index].frame = 0;

		//ステートをサーチ状態へ移行
		boss[index].state = BOSS_STATE_SEARCH;
	}
}

int Boss_AddDamage(int damage)
{
	boss[0].bosshitpoint -= damage;
	return boss[0].bosshitpoint;

	//if (boss[0].bosshitpoint < 0)
	//	boss[0].bosshitpoint = 0;
}



int Boss_GetHitPoint()
{
	return boss[0].bosshitpoint;
}
