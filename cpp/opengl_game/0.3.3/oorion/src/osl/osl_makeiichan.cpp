#include "stdafx.h"

#include "../draw/font.h"
#include "../draw/texture.h"
#include "../misc.h"
#include "../config.h"
#include "../draw/draw.h"
#include "../gui/gui.h"

#include "osl_types.h"
#include "osl_lexems.h"
#include "osl_misc.h"


#include "osl_makeiichan.h"

#include "../iichan/iichan.h"
#include "../iichan/iichan_resource.h"


extern GUI* gui;
extern config cfg;
extern IICHAN* iichan;

void CreatePlayer(string p1, string p2, string p3, string p4,
				  string p5, string p6, string p7, string p8)
{
	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) &&  osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) &&  osl_IsIntConstant(p4) &&  
			osl_IsIntConstant(p5) && osl_IsIntConstant(p6) &&
			osl_IsIntConstant(p7) && osl_IsIntConstant(p8)))
		{
			osl_Error("CreatePlayer(): incompatible parametres");
			return;
		}
		else
		{
			char* name = NULL;
			string s = osl_RemoveQuotesStr(p1);
			name = (char*)s.c_str();
			OBJECT_RES* resource = ResourceByName(name);
			int health = osl_StrToInt(p2);
			int ammo = osl_StrToInt(p3);
			int score = osl_StrToInt(p4);

			int x = osl_StrToInt(p5);
			int y = osl_StrToInt(p6);
			ANIMATION_TYPE type = ANIMATION_TYPE(osl_StrToInt(p7));
			bool looped = osl_StrToInt(p8) != 0;

			PLAYER* player = new PLAYER(health, ammo, score);
			player->SetWeight(cfg.hero_weight);

			ANIMATEDSPRITE* player_sprite = new ANIMATEDSPRITE();
			player->SetSprite(player_sprite);
			player_sprite->SetX(x);
			player_sprite->SetY(y);
			player_sprite->SetYStart(y);
			if(!resource)
				osl_Warning("Resource not found");
			player_sprite->LoadFromResource(resource);
			player_sprite->SetCurrentAnimation(type, looped);
			player_sprite->SetZ(PLAYER_Z);
			iichan->GetItemAllocator()->AddItem(player);
		}
	}
	else
	{
		osl_Error("CreatePlayer(): Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Player created");
}

void CreatePowerup(string p1, string p2, string p3, string p4, string p5)
{
	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) && osl_IsIntConstant(p4) &&
			osl_IsIntConstant(p5)))
		{
			osl_Error("CreatePowerup(): incompatible parametres");
			return;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		OBJECT_RES* resource = ResourceByName(name);
		if(!resource)
			osl_Warning("Resource not found");
		POWERUP_TYPE type = POWERUP_TYPE(osl_StrToInt(p2));
		int count = osl_StrToInt(p3);
		int x = osl_StrToInt(p4);
		int y = osl_StrToInt(p5);

		iichan->GetItemAllocator()->CreatePowerup(resource, type, count, x, y);
	}
	else
	{
		osl_Error("CreatePowerup(): Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Powerup created");
}

void CreateEnemy(string p1, string p2, string p3, string p4)
{
	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) && osl_IsIntConstant(p4)))
		{
			osl_Error("CreateEnemy(): incompatible parametres");
			return;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		OBJECT_RES* resource = ResourceByName(name);
		if(!resource)
			osl_Warning("Resource not found");
		int x = osl_StrToInt(p2);
		int y = osl_StrToInt(p3);
		int health = osl_StrToInt(p4);
		iichan->GetItemAllocator()->CreateEnemy(resource, x, y, health);

	}
	else
	{
		osl_Error("CreateEnemy(): Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Enemy created");
}

void CreateSprite(string p1, string p2, string p3)
{
	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3)))
		{
			osl_Error("CreateEnemy(): incompatible parametres");
			return;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		OBJECT_RES* resource = ResourceByName(name);
		if(!resource)
			osl_Warning("Resource not found");

		int x = osl_StrToInt(p2);
		int y = osl_StrToInt(p3);

		SPRITE* spr = new SPRITE();
		spr->SetX(x);
		spr->SetY(y);
		spr->LoadFromResource(resource);

		ITEM* item = NULL;
		item = new ITEM();
		item->SetSprite((ANIMATEDSPRITE*)spr);
		iichan->GetItemAllocator()->AddItem(item);
	}
	else
	{
		osl_Error("CreateSprite(): Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Sprite created");
}

void CreateEvent(string p1, string p2, string p3, string p4, string p5)
{
	if(iichan)
	{
		if(iichan->GetEventAllocator())
		{
			if(!(osl_IsIntConstant(p1) && osl_IsIntConstant(p2) &&
				osl_IsIntConstant(p3) && osl_IsIntConstant(p4) &&
				osl_IsStrConstant(p5)))
			{
				osl_Error("CreateEvent(): incompatible parametres");
				return;
			}
			EVENT* e = new EVENT();
			e->condition = osl_StrToInt(p1);
			e->condition_param = osl_StrToInt(p2);
			e->action = osl_StrToInt(p3);
			e->action_param_int = osl_StrToInt(p4);
			e->action_param_str = osl_RemoveQuotesStr(p5);
			iichan->GetEventAllocator()->AddEvent(e);
		}
		else
			osl_Error("CreateEvent(): Event allocator is not created");
	}
	else
		osl_Error("CreateEvent(): Game is not created");
}

void SetWorldMaxX(string p1)
{
	if(iichan)
	{
		int x = osl_StrToInt(p1);
		iichan->SetWorldMaxX(x);
	}
	else
	{
		osl_Error("Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "World Max X set");
}

void SetWorldMinX(string p1)
{
	if(iichan)
	{
		int x = osl_StrToInt(p1);
		iichan->SetWorldMinX(x);
	}
	else
	{
		osl_Error("Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "World Max Y set");
}