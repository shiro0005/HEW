#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "result.h"
#include "fade.h"
#include <d3d9.h>

void Result_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void Result_Finalize(void)
{
}

void Result_Update(void)
{

}

void Result_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);
	Sprite_Draw(TEXTURE_INDEX_TEN, 900.0f, 100.0f);
	Sprite_Draw(TEXTURE_INDEX_TEN, 900.0f, 200.0f);
	Sprite_Draw(TEXTURE_INDEX_TEN, 900.0f, 400.0f);
}