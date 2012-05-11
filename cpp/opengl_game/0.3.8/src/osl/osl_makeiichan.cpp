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
extern ii* iichan;

extern string OSL_CURRENT_PARAMS;
extern string OSL_CURRENT_FUNCTION;
BOOL TERMINATE_IICHAN = FALSE;


bool CreatePlayer(string p1, string p2, string p3, string p4,
				  string p5, string p6)
{
	OSL_CURRENT_FUNCTION = "CreatePlayer";
	OSL_CURRENT_PARAMS = p1 + ", " + p2 + ", " + p3 + ", " + p4 + ", " + p5 + ", " + p6;

	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) &&  osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) &&  osl_IsIntConstant(p4) &&  
			osl_IsIntConstant(p5) && osl_IsIntConstant(p6)))
		{
			osl_Error("incompatible parametres");
			return false;
		}
		else
		{
			char* name = NULL;
			string s = osl_RemoveQuotesStr(p1);
			name = (char*)s.c_str();
			RESOURCE* resource = ResourceByName(name);
			if(resource)
			{
				int health = osl_StrToInt(p2);
				int ammo = osl_StrToInt(p3);
				int score = osl_StrToInt(p4);
				int x = osl_StrToInt(p5);
				int y = osl_StrToInt(p6);
				assert(iichan->GetItemAllocator() != NULL);
				iichan->GetItemAllocator()->CreatePlayer(resource, health, ammo, score, x, y);
				return true;
			}
			else
			{
				osl_Error("Resource not found");
			}
		}
	}
	else
	{
		osl_Error("Game is not created");
	}
	return false;
}
// Имя ресурса(тип), count, x, y
bool CreatePowerup(string p1, string p2, string p3, string p4)
{
	OSL_CURRENT_FUNCTION = "CreatePowerup";
	OSL_CURRENT_PARAMS = p1 + ", " + p2 + ", " + p3 + ", " + p4;

	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) && osl_IsIntConstant(p4) ))
		{
			osl_Error("incompatible parametres");
			return false;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		RESOURCE* resource = ResourceByName(name);
		if(resource)
		{		
			int count = osl_StrToInt(p2);
			int x = osl_StrToInt(p3);
			int y = osl_StrToInt(p4);
			assert(iichan->GetItemAllocator() != NULL);
			iichan->GetItemAllocator()->CreatePowerup(resource, count, x, y);
			return true;
		}
		else
		{
			osl_Error("Resource not found");
		}
	}
	else
	{
		osl_Error("Game is not created");
	}
	return false;
}

bool CreateEnemy(string p1, string p2, string p3, string p4)
{
	OSL_CURRENT_FUNCTION = "CreateEnemy";
	OSL_CURRENT_PARAMS = p1 + ", " + p2 + ", " + p3 + ", " + p4;

	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) && osl_IsIntConstant(p4)))
		{
			osl_Error("incompatible parametres");
			return false;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		RESOURCE* resource = ResourceByName(name);
		if(resource)
		{
			int x = osl_StrToInt(p2);
			int y = osl_StrToInt(p3);
			int health = osl_StrToInt(p4);
			assert(iichan->GetItemAllocator() != NULL);
			iichan->GetItemAllocator()->CreateEnemy(resource, x, y, health);
			return true;
		}
		else
		{
			osl_Error("Resource not found");
		}
	}
	else
	{
		osl_Error("Game is not created");
	}
	return false;
}

bool CreateSprite(string p1, string p2, string p3)
{
	OSL_CURRENT_FUNCTION = "CreateSprite";
	OSL_CURRENT_PARAMS = p1 + ", " + p2 + ", " + p3;
	if(iichan)
	{
		if(!(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3)))
		{
			osl_Error("incompatible parametres");
			return false;
		}
		char* name = NULL;
		string s = osl_RemoveQuotesStr(p1);
		name = (char*)s.c_str();
		RESOURCE* resource = ResourceByName(name);
		if(resource)
		{
			int x = osl_StrToInt(p2);
			int y = osl_StrToInt(p3);
			assert(iichan->GetItemAllocator() != NULL);
			iichan->GetItemAllocator()->CreateSprite(resource, x, y);
			return true;
		}
		else
		{
			osl_Error("Resource not found");
		}
	}
	else
	{
		osl_Error("Game is not created");
	}
	return false;
}

bool CreateEvent(string p1, string p2, string p3, string p4, string p5, string p6)
{
	OSL_CURRENT_FUNCTION = "CreateEvent";
	OSL_CURRENT_PARAMS = p1 + ", " + p2 + ", " + p3 + ", " + p4 + ", " + p5 + ", " + p6;

	if(iichan)
	{
		if(iichan->GetEventAllocator())
		{
			if(!(osl_IsIntConstant(p1) && osl_IsIntConstant(p2) &&
				osl_IsIntConstant(p3) && osl_IsIntConstant(p4) &&
				osl_IsIntConstant(p5) && osl_IsStrConstant(p6)))
			{
				osl_Error("incompatible parametres");
				return false;
			}
			EVENT* e = new EVENT();
			e->condition = osl_StrToInt(p1);
			e->condition_param1 = osl_StrToInt(p2);
			e->condition_param2 = osl_StrToInt(p3);
			e->action = osl_StrToInt(p4);
			e->action_param_int = osl_StrToInt(p5);
			e->action_param_str = osl_RemoveQuotesStr(p6);
	
			assert(iichan->GetEventAllocator() != NULL);

			iichan->GetEventAllocator()->AddEvent(e);

			return true;
		}
		else
			osl_Error("Event allocator is not created");
	}
	else
		osl_Error("Game is not created");

	return false;
}

void SetWorldMaxX(string p1)
{
	OSL_CURRENT_FUNCTION = "SetWorldMaxX";
	OSL_CURRENT_PARAMS = p1;
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
	WriteToLog(DEFAULT_OSL_LOG_NAME, "World Max X set to: " + p1);
}

void SetWorldMinX(string p1)
{
	OSL_CURRENT_FUNCTION = "SetWorldMinX";
	OSL_CURRENT_PARAMS = p1;

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
	WriteToLog(DEFAULT_OSL_LOG_NAME, "World Min X set to: " + p1);
}

void StartBriefing(string p1, string p2, string p3, string p4, string p5,
				   string p6, string p7, string p8)
{
	OSL_CURRENT_FUNCTION = "StartBriefing";
	OSL_CURRENT_PARAMS = p1 + ", " + p2 + ", " + p3 + ", " + p4 + ", " + p5 + ", " + p6 + ", " + p7 + ", " + p8;

	if(iichan)
	{
		Briefing* br = new Briefing();

		LoadBriefing(osl_RemoveQuotesStr(p1).c_str(), br);

		br->position_x = (float)osl_StrToInt(p2);
		br->position_y = (float)osl_StrToInt(p3);
		br->width = osl_StrToInt(p4);
		br->height = osl_StrToInt(p5);
		br->max_line_length = osl_StrToInt(p6);
		br->speed = osl_StrToFloat(p7);
		br->line_dy = osl_StrToInt(p8);

		iichan->GetInstanceAllocator()->StartBriefing(br);
	}
}


bool LoadLevel(string file_name)
{
	if(iichan)
	{
		if(iichan->GetItemAllocator())
			return iichan->GetItemAllocator()->LoadLevel(file_name);
	}
	return false;
}

void TerminateGame()
{
	TERMINATE_IICHAN = TRUE;
}