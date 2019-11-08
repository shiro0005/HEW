
#include "player.h"
#include "main.h"
#include "input.h"
#include "sprite.h"
#include "table.h"
#include "bullet.h"
#include "blade.h"
#include "collision.h"
#include <time.h>


#define PLAYER_WIDTH	(32)
#define PLAYER_HEIGHT	(32)

#define ANIME_PATTERN_MAX		3
#define ANIME_PATTERN_SKIPFRAME 8


typedef struct
{
	D3DXVECTOR2 pos;			//位置
	int			muki;			//向き
	int			color;			//色

	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト

	CIRCLE		collision;		//円の当たり判定
	int			hitpoint;		//ヒットポイント
	float rotate;

}PLAYER;

PLAYER player;

void Player_Initialize(void)
{
	srand((unsigned int)time(NULL));
	//プレイヤーの初期化
	player.pos.x = SCREEN_WIDTH / 2;
	player.pos.y = SCREEN_HEIGHT / 2;

	player.color = 0;
	player.muki = 0;

	player.collision.cx = player.pos.x;
	player.collision.cy = player.pos.y;
	player.collision.r = PLAYER_WIDTH * 0.8f;

	player.rotate = rand() % 361;
	player.hitpoint = 10000;
}

void Player_Finalize(void)
{

}


//プレイヤー移動処理
void Player_Update(void)
{
	D3DXVECTOR2 dir = D3DXVECTOR2(0.0f, 0.0f);

	if (Keyboard_IsPress(DIK_UP) || GamePad_IsPress(0, BUTTON_UP)){
		dir.y -= 1.0f;
		player.muki = 3;
	}
	if ((Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_UP)&&GamePad_IsPress(0,BUTTON_Y)){
		dir.y -= 10.0f;
		player.muki = 3;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_DOWN) || GamePad_IsPress(0, BUTTON_DOWN)){
		dir.y += 1.0f;
		player.muki = 0;
	}
	if ((Keyboard_IsPress(DIK_DOWN )&& Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_DOWN) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.y += 10.0f;
		player.muki = 0;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_LEFT) || GamePad_IsPress(0, BUTTON_LEFT)){
		dir.x -= 1.0f;
		player.muki = 1;
	}
	if ((Keyboard_IsPress(DIK_LEFT ) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_LEFT) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.x -= 10.0f;
		player.muki = 1;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_RIGHT) || GamePad_IsPress(0, BUTTON_RIGHT)){
		dir.x += 1.0f;
		player.muki = 2;
	}
	if ((Keyboard_IsPress(DIK_RIGHT) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_RIGHT) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.x += 10.0f;
		player.muki = 2;
		player.rotate += 0.1f;
	}
	


	//スペースが押されたら弾を発射
	if (Keyboard_IsTrigger(DIK_Z) || GamePad_IsPress(0, BUTTON_Y) && GamePad_IsTrigger(0, BUTTON_A)) {
		//カーソルキー入力がされていない場合、向きから発射方向を作成する
		if (D3DXVec2Length(&dir) < 0.01f) {
			switch (player.muki)
			{
			case 0://下向き
				dir.y = 1.0f;
				break;
			case 1://左向き
				dir.x = -1.0f;
				break;
			case 2://右向き
				dir.x = 1.0f;
				break;
			case 3://上向き
				dir.y = -1.0f;
				break;
			}
		}
		Bullet_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}

	//ブレイドで攻撃
	if (Keyboard_IsTrigger(DIK_X) || GamePad_IsPress(0, BUTTON_Y) && GamePad_IsTrigger(0, BUTTON_B)) {
		//カーソルキー入力がされていない場合、向きから方向を作成する
		if (D3DXVec2Length(&dir) < 0.01f) {
			switch (player.muki)
			{
			case 0://下向き
				dir.y = 1.0f;
				break;
			case 1://左向き
				dir.x = -1.0f;
				break;
			case 2://右向き
				dir.x = 1.0f;
				break;
			case 3://上向き
				dir.y = -1.0f;
				break;
			}
		}
	
		Blade_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}

	//マップとのヒットチェック
	Collision_HitCheck_TileMap(player.pos, &dir);

	// 座標の更新処理
	player.pos += dir;

	//当たり判定用座標の更新
	player.collision.cx = player.pos.x;
	player.collision.cy = player.pos.y;

	//スキップするフレーム値を超えたら
	if (++player.animeFrame > ANIME_PATTERN_SKIPFRAME)
	{
		//アニメパターンを進める(最大値を超えたらリセット)
		if (++player.animePattern >= ANIME_PATTERN_MAX)
			player.animePattern = 0;
		//フレームは元に戻す
		player.animeFrame = 0;
	}

}

void Player_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
		player.pos.x,
		player.pos.y,
		GetAnimTbl(player.color, player.muki, player.animePattern).x * 256,
		GetAnimTbl(player.color, player.muki, player.animePattern).y * 256,
		32,
		32,
		16,
		16,
		4.0f,
		6.0f,
		player.rotate);
}

const CIRCLE* Player_GetCollision()
{
	return &player.collision;
}

void Player_AddDamage(int damage)
{
	player.hitpoint -= damage;
	if (player.hitpoint < 0)
		player.hitpoint = 0;
}

int Player_GetHitPoint()
{
	return player.hitpoint;
}
