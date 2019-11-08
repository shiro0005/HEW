#include "texture.h"
#include "sprite.h"


#define EXPLOSION_MAX    (128)
#define EXPLOSION_WIDTH  (32)
#define EXPLOSION_HEIGHT (32)
#define EXPLOSION_PATTERN_FRAME (1)
#define EXPLOSION_PATTERN_MAX   (16)
#define EXPLOSION_PATTERN_H_MAX (4)


typedef struct Explosion_tag
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
} Explosion;

static Explosion g_Explosions[EXPLOSION_MAX];

static int g_ExplosionFrameCount = 0;


void Explosion_Initialize(void)
{
	for( int i = 0; i < EXPLOSION_MAX; i++ ) {
		g_Explosions[i].enable = false;
	}
}

void Explosion_Update(void)
{
	for( int i = 0; i < EXPLOSION_MAX; i++ ) {

		if( g_Explosions[i].enable ) {

			int age = g_ExplosionFrameCount - g_Explosions[i].create_frame;

			g_Explosions[i].pattern = age / EXPLOSION_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if( g_Explosions[i].pattern >= EXPLOSION_PATTERN_MAX ) {
				g_Explosions[i].enable = false;
			}		
		}
	}

	g_ExplosionFrameCount++;
}

void Explosion_Draw(void)
{
	for( int i = 0; i < EXPLOSION_MAX; i++ ) {

		if( !g_Explosions[i].enable ) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSION_WIDTH * (g_Explosions[i].pattern % EXPLOSION_PATTERN_H_MAX);
		int ty = EXPLOSION_HEIGHT * (g_Explosions[i].pattern / EXPLOSION_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_EXPLOSION,
			g_Explosions[i].x,
			g_Explosions[i].y,
			tx,
			ty,
			EXPLOSION_WIDTH,
			EXPLOSION_HEIGHT,
			EXPLOSION_WIDTH/2,
			EXPLOSION_HEIGHT/2,
			3.0f,
			3.0f,
			0.0f);
	}
}

void Explosion_Create(float x, float y)
{
	for( int i = 0; i < EXPLOSION_MAX; i++ ) {

		if( g_Explosions[i].enable ) {
			continue;
		}

		g_Explosions[i].x = x;
		g_Explosions[i].y = y;
		g_Explosions[i].create_frame = g_ExplosionFrameCount;
		g_Explosions[i].pattern = 0;
		g_Explosions[i].enable = true;

		break;
	}
}
