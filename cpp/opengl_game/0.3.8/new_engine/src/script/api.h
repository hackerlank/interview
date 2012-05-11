#ifndef __SCRIPT_API_H_
#define __SCRIPT_API_H_

#include "script.h"

namespace scriptApi
{
	void RegisterAPI(lua_State* L);

	int LoadConfig (lua_State* L);
	int LoadTexture (lua_State* L);
	int LoadFont(lua_State* L);
	int LoadPrototype(lua_State* L);

	int InitNewGame(lua_State* L);

	int CreateSprite(lua_State* L);
	int CreatePlayer(lua_State* L);
	int CreateSpatialTrigger(lua_State* L);

	int AddTimerEvent(lua_State* L);

	int Log(lua_State* L);

	int SetCamAttachedObj(lua_State* L);
	int CamMoveToPos(lua_State* L);
	int SetCamFocusOnObjPos(lua_State* L);
	int SetCamObjOffset(lua_State* L);
	int SetCamAttachedAxis(lua_State* L);

	int CreateRibbon(lua_State* L);

//////////////////////////////////////////////////////////////////////////

	int GetPlayer(lua_State* L);
	int GetObject(lua_State* L);

//////////////////////////////////////////////////////////////////////////

	int SetObjPos(lua_State* L);
	int SetDynObjAcc(lua_State* L);
	int SetDynObjVel(lua_State* L);
	int SetDynObjMovement(lua_State* L);
	int SetDynObjMoveDirX(lua_State* L);
	int SetDynObjJumpVel(lua_State* L);
	int SetObjSpriteMirrored(lua_State* L);
	int SetObjAnim(lua_State* L);

	int EnablePlayerControl(lua_State* L);
	
	int SetObjProcessor(lua_State* L);

//////////////////////////////////////////////////////////////////////////

	int SetOnChangePlayerProcessor(lua_State* L);
	int BlockPlayerChange(lua_State* L);
	int UnblockPlayerChange(lua_State* L);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Для файла threads.cpp
// Функции создания отдельных "потоков" на основе coroutines, которые
// можно приостанавливать.

	int NewThread(lua_State* L);
	int Wait(lua_State* L);	
	int Resume(lua_State* L);


}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// game.cpp
#include "../game/game.h"
extern game::GameStates game_state;
// 
#define CHECKGAME if (game_state != game::GAME_RUNNNIG) { lua_pop(L, lua_gettop(L)); lua_pushstring(L, "GAME not running"); return 1; } 


#endif // __SCRIPT_API_H_