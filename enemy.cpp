
#include "enemy.h"
#include "main.h"
#include "sprite.h"
#include "table.h"
#include "player.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "explosion.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ENEMY_WIDTH				(32)
#define ENEMY_HEIGHT			(32)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define ENEMY_SEARCH_RADIUS		(200.0f)
#define ENEMY_SEARCH_ANGLE		(0.85f)
#define ENEMY_CHASE_SPEED		(3.0f)
#define ENEMY_RETURN_SPEED		(1.0f)

typedef struct
{
	int			enable;	//生存フラグ

	D3DXVECTOR2 pos;	//位置
	float		rot;	//回転
	int			color;	//色
	int			muki;	//向き（移動方向）

	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト

	CIRCLE		collision;

	int			frame;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_shot;
	D3DXVECTOR2 pos_return;

}ENEMY_DATA;

ENEMY_DATA enemy[ENEMY_COUNT];


typedef enum
{
	ENEMY_STATE_INIT,		//初期化
	ENEMY_STATE_SEARCH,		//周囲をサーチ
	ENEMY_STATE_FIND,		//敵を発見
	ENEMY_STATE_CHASE,		//追跡
	ENEMY_STATE_SHOT,		//ショット発射
	ENEMY_STATE_LASER,		//レーザー発射
	ENEMY_STATE_COOLDOWN,	//ショット終了
	ENEMY_STATE_RETURN,		//定位置に戻る
	ENEMY_STATE_DEAD,		//死亡状態

	ENEMY_STATE_MAX

}ENEMY_STATE;


int Enemy_CalcMuki(D3DXVECTOR2 dir);

void Enemy_StateInit(int index);
void Enemy_StateSearch(int index);
void Enemy_StateFind(int index);
void Enemy_StateChase(int index);
void Enemy_StateShot(int index);
void Enemy_StateLaser(int index);
void Enemy_StateCooldown(int index);
void Enemy_StateReturn(int index);


void Enemy_Initialize(void)
{
	//敵の初期化
	for (int i = 0; i < ENEMY_COUNT; i++) {
		Enemy_StateInit(i);
		enemy[i].state = ENEMY_STATE_SEARCH;
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
		}

		switch (enemy[i].state)
		{
		case ENEMY_STATE_INIT:
			Enemy_StateInit(i);
			break;
		case ENEMY_STATE_SEARCH:
			Enemy_StateSearch(i);
			break;
		case ENEMY_STATE_FIND:
			Enemy_StateFind(i);
			break;
		case ENEMY_STATE_CHASE:
			Enemy_StateChase(i);
			break;
		case ENEMY_STATE_SHOT:
			Enemy_StateShot(i);
			break;
		case ENEMY_STATE_LASER:
			Enemy_StateLaser(i);
			break;
		case ENEMY_STATE_COOLDOWN:
			Enemy_StateCooldown(i);
			break;
		case ENEMY_STATE_RETURN:
			Enemy_StateReturn(i);
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
			GetAnimTbl(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
			GetAnimTbl(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
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

//向きを作る
int Enemy_CalcMuki(D3DXVECTOR2 dir)
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

void Enemy_StateInit(int index)//エネミー出現情報
{
	enemy[index].pos.x = frand() * SCREEN_WIDTH;
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;
	enemy[index].rot = 0;
	enemy[index].color = 1;

	enemy[index].muki = rand() % 4;
	enemy[index].enable = TRUE;

	enemy[index].collision.cx = enemy[index].pos.x;
	enemy[index].collision.cy = enemy[index].pos.y;
	enemy[index].collision.r = ENEMY_WIDTH * 0.8f;

	enemy[index].frame = 0;

	//ステートをサーチ状態へ移行
	enemy[index].state = ENEMY_STATE_SEARCH;

}

void Enemy_StateSearch(int index)
{
	//フレームを進める
	enemy[index].frame++;

	D3DXVECTOR2 dir;

	//向いている方向のベクトルを作る
	switch (enemy[index].muki)
	{
	case 0://下向き
		dir = D3DXVECTOR2( 0.0f,  1.0f);
		break;
	case 1://左向き
		dir = D3DXVECTOR2(-1.0f,  0.0f);
		break;
	case 2://右向き
		dir = D3DXVECTOR2( 1.0f,  0.0f);
		break;
	case 3://上向き
		dir = D3DXVECTOR2( 0.0f, -1.0f);
		break;
	}

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->cx, Player_GetCollision()->cy);

	//敵とプレイヤーの距離を計算する
	D3DXVECTOR2 vLen = enemy[index].pos - pl_pos;
	float length = D3DXVec2Length(&vLen);

	//判定する距離の中に入っているかをチェックする
	if (length < ENEMY_SEARCH_RADIUS)
	{
		//敵から見てプレイヤーの座標への方向ベクトルを作る
		D3DXVECTOR2 pl_dir = pl_pos - enemy[index].pos;
		D3DXVec2Normalize(&pl_dir, &pl_dir);

		//敵の向きとプレイヤ座標への方向ベクトルで内積を取る
		float dot = D3DXVec2Dot(&dir, &pl_dir);

		//判定する角度内に入っているかをチェックする
		if (dot > ENEMY_SEARCH_ANGLE)
		{
			Explosion_Create(enemy[index].pos.x, enemy[index].pos.y);

			//判定に通ればプレイヤー発見ステートに移行する
			enemy[index].state = ENEMY_STATE_FIND;
			enemy[index].frame = 0;

			//戻って来る座標を保存する
			enemy[index].pos_return = enemy[index].pos;
		}
	}

	//向きを変化させる
	if (enemy[index].frame > 60)
	{
		enemy[index].muki = (enemy[index].muki + 1) % 4;
		enemy[index].frame = 0;
	}

}

void Enemy_StateFind(int index)
{
	//フレームを進める
	enemy[index].frame++;

	//一定時間経ったら追跡ステートへ移行
	if (enemy[index].frame > 20){

		enemy[index].frame = 0;

		//ステートをサーチ状態へ移行
		enemy[index].state = ENEMY_STATE_CHASE;
	}
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

	enemy[index].pos += dir;

	//移動方向から向きを作る
	enemy[index].muki = Enemy_CalcMuki(dir);

	//一定時間経ったら弾射出ステートへ移行
	if (enemy[index].frame > 120){

		enemy[index].frame = 0;

		//移動方向を保存
		enemy[index].dir_shot = dir;

		//ランダムで「ばら撒き弾」もしくは「レーザー」を射出
		if (rand() % 2)
		{
			//ステートをサーチ状態へ移行
			enemy[index].state = ENEMY_STATE_SHOT;
		}
		else{
			//ステートをサーチ状態へ移行
			enemy[index].state = ENEMY_STATE_LASER;
		}
	}
}

void Enemy_StateShot(int index)
{
	//フレームを進める
	enemy[index].frame++;

	float shot_rad = atan2(enemy[index].dir_shot.y, enemy[index].dir_shot.x);
	shot_rad = (float)(M_PI * 2 / 20)*enemy[index].frame;

	//射出角ベクトルをフレーム値で回転させる
	D3DXVECTOR2 shot_dir;
	shot_dir.x = enemy[index].dir_shot.x*cosf(shot_rad) - enemy[index].dir_shot.y*sinf(shot_rad);
	shot_dir.y = enemy[index].dir_shot.x*sinf(shot_rad) + enemy[index].dir_shot.y*cosf(shot_rad);

	EnemyBullet_Create(enemy[index].pos.x, enemy[index].pos.y, shot_dir);

	//一定時間経ったらクールダウンステートへ移行
	if (enemy[index].frame > 90){

		enemy[index].frame = 0;

		//ステートを帰還状態へ移行
		enemy[index].state = ENEMY_STATE_COOLDOWN;
	}
}

void Enemy_StateLaser(int index)
{
	if (enemy[index].frame == 0){
		EnemyLaser_Create(enemy[index].pos.x, enemy[index].pos.y, enemy[index].dir_shot);
	}

	//フレームを進める
	enemy[index].frame++;


	//一定時間経ったらクールダウンステートへ移行
	if (enemy[index].frame > 90){

		enemy[index].frame = 0;

		//ステートを帰還状態へ移行
		enemy[index].state = ENEMY_STATE_COOLDOWN;
	}
}

void Enemy_StateCooldown(int index)
{
	//フレームを進める
	enemy[index].frame++;

	//一定時間経ったら帰還ステートへ移行
	if (enemy[index].frame > 30){

		enemy[index].frame = 0;

		//ステートを帰還状態へ移行
		enemy[index].state = ENEMY_STATE_RETURN;
	}
}

void Enemy_StateReturn(int index)
{
	//フレームを進める
	enemy[index].frame++;

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->cx, Player_GetCollision()->cy);

	//帰還する座標への向きベクトルを計算する
	D3DXVECTOR2 dir = enemy[index].pos_return - enemy[index].pos;

	//ベクトルの長さを計算する
	float length = D3DXVec2Length(&dir);

	D3DXVec2Normalize(&dir, &dir);
	dir *= ENEMY_CHASE_SPEED;

	enemy[index].pos += dir;

	//移動方向から向きを作る
	enemy[index].muki = Enemy_CalcMuki(dir);

	//帰還する座標に到着したら
	if (length <= ENEMY_CHASE_SPEED){

		enemy[index].frame = 0;

		//ステートをサーチ状態へ移行
		enemy[index].state = ENEMY_STATE_SEARCH;
	}
}
