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
				  string p5, string p6)
{
	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) &&  osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) &&  osl_IsIntConstant(p4) &&  
			osl_IsIntConstant(p5) && osl_IsIntConstant(p6)))
		{
			osl_Error("CreatePlayer(): incompatible parametres");
			return;
		}
		else
		{
			char* name = NULL;
			string s = osl_RemoveQuotesStr(p1);
			name = (char*)s.c_str();
			RESOURCE* resource = ResourceByName(name);
			if(!resource)
				osl_Warning("Resource not found");
			int health = osl_StrToInt(p2);
			int ammo = osl_StrToInt(p3);
			int score = osl_StrToInt(p4);
			int x = osl_StrToInt(p5);
			int y = osl_StrToInt(p6);

			assert(iichan->GetItemAllocator() != NULL);

			iichan->GetItemAllocator()->CreatePlayer(resource, health, ammo, score, x, y);
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
		RESOURCE* resource = ResourceByName(name);
		if(!resource)
			osl_Warning("Resource not found");
		POWERUP_TYPE type = POWERUP_TYPE(osl_StrToInt(p2));
		int count = osl_StrToInt(p3);
		int x = osl_StrToInt(p4);
		int y = osl_StrToInt(p5);

		assert(iichan->GetItemAllocator() != NULL);

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
		RESOURCE* resource = ResourceByName(name);
		if(!resource)
			osl_Warning("Resource not found");
		int x = osl_StrToInt(p2);
		int y = osl_StrToInt(p3);
		int health = osl_StrToInt(p4);

		assert(iichan->GetItemAllocator() != NULL);

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
			osl_Error("CreateSprite(): incompatible parametres");
			return;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		RESOURCE* resource = ResourceByName(name);
		if(!resource)
			osl_Warning("Resource not found");

		int x = osl_StrToInt(p2);
		int y = osl_StrToInt(p3);

		assert(iichan->GetItemAllocator() != NULL);

		iichan->GetItemAllocator()->CreateSprite(resource, x, y);
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
	
			assert(iichan->GetEventAllocator() != NULL);

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