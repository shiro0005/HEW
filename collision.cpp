#include "game.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "explosion.h"
#include "scene.h"
#include "tilemap.h"
#include <d3dx9.h>
#include "stage.h"


static bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
void Collision_Player_vs_Enemy(void);
void Collision_Bullet_vs_Enemy(void);
void Collision_Player_vs_EnemyBullet(void);
void Collision_Player_vs_EnemyLaser(void);

void Collision_Player_vs_Boss(void);
void Collision_Bullet_vs_Boss(void);
void Collision_Player_vs_BossBullet(void);
void Collision_Player_vs_BossLaser(void);




bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
	//引数からベクトル型の変数を作る
	D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
	D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
	//二点間のベクトルを作る
	D3DXVECTOR2 distance = dst2 - dst1;
	//作ったベクトルの長さを求める
	float length = D3DXVec2Length(&distance);
	//お互いの半径を足した値を求める
	float size = p_circle_a->r + p_circle_b->r;

	//ベクトルの長さとお互いの半径を足した値を比較する
	//→ベクトルの長さの方が小さければヒットしている！
	if (length < size){
		return true;
	}
	//ヒットしていなかった
	return false;
}

/*
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
//引数からベクトル型の変数を作る
D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
//二点間のベクトルを作る
D3DXVECTOR2 distance = dst2 - dst1;
//作ったベクトルの長さを求める
float length = D3DXVec2LengthSq(&distance);
//お互いの半径を足した値を求める
float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

//ベクトルの長さとお互いの半径を足した値を比較する
//→ベクトルの長さの方が小さければヒットしている！
if (length < (p_circle_a->r + p_circle_b->r)){
return true;
}
//ヒットしていなかった
return false;
}
*/

bool HitCupsule(const CIRCLE* p_circle, const CUPSULE* p_cupsule)
{
	//円とカプセルの位置
	D3DXVECTOR2 cir_pos = D3DXVECTOR2(p_circle->cx, p_circle->cy);
	D3DXVECTOR2 cup_pos = D3DXVECTOR2(p_cupsule->x, p_cupsule->y);
	D3DXVECTOR2 cup_end = D3DXVECTOR2(p_cupsule->ex, p_cupsule->ey);

	D3DXVECTOR2 distance = cir_pos - cup_pos;

	float t = 0.0f;

	//交点を求める
	t = (cup_end.x * distance.x + cup_end.y * distance.y) /
		(cup_end.x * cup_end.x  + cup_end.y * cup_end.y);

	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;

	D3DXVECTOR2 pos_cross;
	pos_cross.x = (cup_end.x * t) + cup_pos.x;
	pos_cross.y = (cup_end.y * t) + cup_pos.y;

	//交点までの長さとお互いの半径を足したものの大きさを比べる
	float cross_len = (pos_cross.x - cir_pos.x) * (pos_cross.x - cir_pos.x) +
                	  (pos_cross.y - cir_pos.y) * (pos_cross.y - cir_pos.y);

	float size = (p_circle->r + p_cupsule->r);

	//→ベクトルの長さの方が小さければヒットしている！
	if (cross_len < size * size)
	{
		return true;
	}

	//ヒットしていなかった
	return false;
}

void Collision_Initialize(void)
{
}

void Collision_Update(void)
{
//	Collision_Player_vs_Enemy();
	Collision_Bullet_vs_Enemy();
	Collision_Player_vs_EnemyBullet();
	Collision_Player_vs_EnemyLaser();
	Collision_Bullet_vs_Boss();
	Collision_Player_vs_BossBullet();
	Collision_Player_vs_BossLaser();
}

void Collision_Player_vs_Enemy(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++) {

		// エネミーは有効か？
		if( !Enemy_IsEnable(i) ) {
			continue;
		}

		// プレイヤーのコリジョンとエネミーのコリジョン
		if (HitCircle(Player_GetCollision(), Enemy_GetCollision(i)))
		{
			// 当たってる
			Enemy_Destroy(i);
		}
	}
}

void Collision_Player_vs_EnemyBullet(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++) {

		// エネミーは有効か？
		if (!EnemyBullet_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとエネミーのコリジョン
		if (HitCircle(Player_GetCollision(), EnemyBullet_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(30);

			// 爆発エフェクトの作成
			Explosion_Create(EnemyBullet_GetCollision(i)->cx, EnemyBullet_GetCollision(i)->cy);

			// 弾の消滅処理
			EnemyBullet_Destroy(i);
		}
	}
}

void Collision_Player_vs_EnemyLaser(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		// エネミーは有効か？
		if (!EnemyLaser_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとレーザーのコリジョン
		if (HitCupsule(Player_GetCollision(), EnemyLaser_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(10);

			// 爆発エフェクトの作成
			Explosion_Create(Player_GetCollision()->cx, Player_GetCollision()->cy);
		}
	}
}

void Collision_Bullet_vs_Enemy(void) 
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		// 弾は有効か？
		if (!Bullet_IsEnable(i)) {
			continue;
		}

		for (int j = 0; j < ENEMY_COUNT; j++)
		{
			// エネミーは有効か？
			if (!Enemy_IsEnable(j)) {
				continue;
			}

			// 弾のコリジョンとエネミーのコリジョン
			if (HitCircle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
			{
				// 当たってる

				// 得点の追加・敵の消滅カウントの追加

				
 				Game_AddScore(100);
				Game_AddKillCount();

				// 爆発エフェクトの作成
				Explosion_Create(Enemy_GetCollision(j)->cx, Enemy_GetCollision(j)->cy);

				// 敵の消滅処理
				Enemy_Destroy(j);

				// 弾の消滅処理
				Bullet_Destroy(i);

				// この弾の処理は終了
				break;
			}
		}
	}
}


bool Collision_HitCheck_TileMap(D3DXVECTOR2 dst, D3DXVECTOR2* pOut)
{
	TILE_DATA tile;
	bool hit = false;

	//横の移動チェック
	tile = GetTileInfo(dst.x + pOut->x, dst.y);
	if (tile.isWalk == 0)
	{
		pOut->x = 0.0f;
		hit = true;
	}

	//縦の移動チェック
	tile = GetTileInfo(dst.x, dst.y + pOut->y);
	//歩けないマップだったらヒット
	if (tile.isWalk == 0)
	{
		pOut->y = 0.0f;
		hit = true;
	}

	return hit;
}

void Collision_Player_vs_Boss(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		// ボスは有効か？
		if (!Boss_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとボスのコリジョン
		if (HitCircle(Player_GetCollision(), Boss_GetCollision(i)))
		{
			// 当たってる
			Boss_Destroy(i);
		}
	}
}

void Collision_Player_vs_BossBullet(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		// ボスは有効か？
		if (!BossBullet_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとボスのコリジョン
		if (HitCircle(Player_GetCollision(), BossBullet_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(30);

			// 爆発エフェクトの作成
			Explosion_Create(BossBullet_GetCollision(i)->cx, BossBullet_GetCollision(i)->cy);

			// 弾の消滅処理
			BossBullet_Destroy(i);
		}
	}
}

void Collision_Player_vs_BossLaser(void)
{
	for (int i = 0; i < BOSSLASER_MAX; i++) {

		// ボスは有効か？
		if (!BossLaser_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとレーザーのコリジョン
		if (HitCupsule(Player_GetCollision(), BossLaser_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(10);

			// 爆発エフェクトの作成
			Explosion_Create(Player_GetCollision()->cx, Player_GetCollision()->cy);
		}
	}
}

void Collision_Bullet_vs_Boss(void)
{

	int bosslife = 0;
	for (int i = 0; i < BULLET_MAX; i++)
	{
		
		// 弾は有効か？
		if (!Bullet_IsEnable(i)) {
			continue;
		}

		for (int j = 0; j < BOSS_COUNT; j++)
		{
			// ボスは有効か？
			if (!Boss_IsEnable(j)) {
				continue;
			}

			// 弾のコリジョンとボスのコリジョン
			if (HitCircle(Bullet_GetCollision(i), Boss_GetCollision(j)))
			{
				// 当たってる


				Game_AddScore(100);


				// 得点の追加・敵の消滅カウントの追加

				bosslife = Boss_AddDamage(500);

				if (bosslife <= 0) {
					Game_AddKillBossCount();


					// 爆発エフェクトの作成
					Explosion_Create(Enemy_GetCollision(j)->cx, Boss_GetCollision(j)->cy);

					// 敵の消滅処理
					Boss_Destroy(j);


				}

				// 弾の消滅処理
				Bullet_Destroy(i);


				// この弾の処理は終了
				break;
			

			}
		}
	}
}

