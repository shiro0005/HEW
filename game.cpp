#include "texture.h"
#include "fade.h"
#include "scene.h"
#include "score_draw.h"


typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};

static PHASE_INDEX g_Phase;
static int g_GameFrameCount;





void Game_Initialize(void)
{



	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void Game_Finalize(void)
{



	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;

	Fade_Start(false, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
}


void Game_Update(void)
{




	switch (g_Phase)
	{
	case PHASE_INDEX_FADE:
	
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


}



