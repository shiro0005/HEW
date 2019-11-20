#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "scene.h"
#include "collision.h"
#include "bullet.h"
#include "boss.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "explosion.h"
#include "score_draw.h"
//#include "tileMap.h"
#include "stage.h"


typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_IN,
	PHASE_INDEX_PLAYER_MUTEKI,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_BOSS,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};

static PHASE_INDEX g_Phase;
static int g_GameFrameCount;

static int g_Score = 0;// 点数
static int g_KillCount = 0;
static int g_Bosskill = 0;

static bool Game_Bossappear(void);

static bool Game_EndCheck(void);



void Game_Initialize(void)
{
	Player_Initialize();
	Field1_Initialize();
	Enemy_Initialize();
	Boss_Initialize();
	Bullet_Initialize();
	EnemyBullet_Initialize();
	EnemyLaser_Initialize();
	BossBullet_Initialize();
	BossLaser_Initialize();
	Explosion_Initialize();


	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	g_Score = 0;
	g_KillCount = 0;
	g_Bosskill = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0,0,0,0));
}

void Game_Finalize(void)
{
	Player_Finalize();
	Enemy_Finalize();
	Boss_Finalize();

}

void Game_Update(void)
{
	switch(g_Phase)
	{
	case PHASE_INDEX_FADE:
		if( !Fade_IsFade() ) {
			g_Phase = PHASE_INDEX_PLAYER_NORMAL;
			// PlaySound(SOUND_LABEL_BGM000);
		}
		break;
	case PHASE_INDEX_PLAYER_IN:
	case PHASE_INDEX_PLAYER_MUTEKI:
	case PHASE_INDEX_PLAYER_NORMAL:
		Player_Update();
		Field1_Update();
		Enemy_Update();
		Bullet_Update();
		EnemyBullet_Update();
		EnemyLaser_Update();
		Explosion_Update();

		//当たり判定は必ずすべてのアップデート処理が終わった後に行う
		Collision_Update();

		//ボス出現
		if (Game_Bossappear())
		{
			Boss_StateInit(0);
			g_Phase = PHASE_INDEX_BOSS;
		}
		break;
	case PHASE_INDEX_BOSS:
 		Player_Update();
		Bullet_Update();
		Field1_Update();
		Boss_Update();
		BossBullet_Draw();
		BossLaser_Update();
		Explosion_Update();

		//当たり判定は必ずすべてのアップデート処理が終わった後に行う
		Collision_Update();

		//ゲームの終了チェック
		if (Game_EndCheck())
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		break;
	case PHASE_INDEX_STAGE_CLEAR:
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_RESULT);
			g_Phase = PHASE_INDEX_END;
		}
		break;
	case PHASE_INDEX_END:
		break;
	}

}

void Game_Draw(void)
{
	//TileMap_Draw();
	Field1_Draw();
	Player_Draw();
	Enemy_Draw();
	Boss_Draw();
	Bullet_Draw();
	EnemyBullet_Draw();
	EnemyLaser_Draw();
	BossBullet_Draw();
	BossLaser_Draw();
	Explosion_Draw();
	Score_Draw(0, 0, Player_GetHitPoint(), 7, true);
}

bool Game_Bossappear(void)
{
	//10匹倒したらボス出現
	if (g_KillCount>=10)
	{
		return true;
	}

	if (Player_GetHitPoint() <= 0)
		return true;

	return false;

}

bool Game_EndCheck(void)
{
	//ボス倒したら終了
	if (g_Bosskill >= 1)
		return true;

	if (Player_GetHitPoint() <= 0)
		return true;

	return false;
}

void Game_AddScore(int score)
{
	g_Score += score;
}

void Game_AddKillCount()
{
	g_KillCount += 1;
	
}

void Game_AddKillBossCount()
{
	g_Bosskill += 1;
}