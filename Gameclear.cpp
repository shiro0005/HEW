#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "gameclear.h"
#include "fade.h"
#include <d3d9.h>

static bool g_bEnd = false;

void Gameclear_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	g_bEnd = false;
}

void Gameclear_Finalize(void)
{
}

void Gameclear_Update(void)
{
	if (!g_bEnd) {
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsPress(0, BUTTON_C)) {
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
		}
	}
	else {
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_RESULT);
		}
	}
}

void Gameclear_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_GAMECLEAR, 0.0f, 0.0f);
	Sprite_Draw(TEXTURE_INDEX_GAMECLEAR, 0.0f, 0.0f);
}