#include "StdAfx.h"

#include "../misc.h"

#include "../script/script.h"

#include "phys/phys_misc.h"

#include "../render/texture.h"
#include "animation.h"
#include "sprite.h"
#include "proto.h"
#include "objects/object.h"
#include "objects/object_sprite.h"
#include "objects/object_player.h"

#include "phys/sap/SweepAndPrune.h"

Opcode::ArraySAP *asap = NULL;

#include "camera.h"
#include "ribbon.h"

#include "game.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

extern CHAR path_levels[MAX_PATH];
extern bool keys[256];

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

game::GameStates game_state = game::GAME_NONE;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void game::FreeGame()
{
	//game_state = game::GAME_DESTROYING;
	RemoveAllObjects();

	DeleteAllRibbons();

	RemoveAllProto();

	DELETESINGLE(asap);
	game_state = game::GAME_NONE;
}

void game::InitGame()
{
	//game_state = game::GAME_DESTROYING;
	RemoveAllObjects();
	DeleteAllRibbons();

	DELETESINGLE(asap);
	asap = new Opcode::ArraySAP();

	//game_state = game::GAME_INITIALIZING;

	CameraMoveToPos(0,0);

	game_state = game::GAME_RUNNNIG;

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void game::HandleKeyboard()
{
	if(keys[VK_ESCAPE])
		if(game_state != game::GAME_PAUSED)
			game_state = game::GAME_PAUSED;
		else
			game_state = game::GAME_RUNNNIG;






}

void game::UpdateGame()
{

	if (game_state == GAME_RUNNNIG)
	{

#ifdef TWO_CHANGABLE_PLAYERS
		PlayerChanger();
#endif // TWO_CHANGABLE_PLAYERS
		
		ProcessAllObjects();
		
	}
}

void game::DrawGame()
{
	if (game_state == GAME_RUNNNIG)
	{
		CameraUpdatePosition();

		DrawRibbons();

		DrawAllObjects();
	}
}

