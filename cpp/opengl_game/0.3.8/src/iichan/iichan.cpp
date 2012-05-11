#include "stdafx.h"
#include "../osl/osl.h"
#include "iichan.h"
#include "iichan_briefing.h"
#include "../sound/snd.h"
#include "../gui/gui.h"

extern SND* snd;
extern GUI* gui;


// --------------------------------------------------------------------------
unsigned int current_state;
unsigned int game_time = 0;

string dbgTime;
#ifdef _DEBUG
string dbgWorldX;
string dbgWorldY;
string dbgEventCount;
string dbgSpriteCount;
#endif
// --------------------------------------------------------------------------


// Level's folder path
// --------------------------------------------------------------------------
extern char path_levels[MAX_PATH];
// --------------------------------------------------------------------------

extern BOOL GLOBAL_QUAKE_ENABLED;
extern string OSL_CURRENT_FILE;
extern BOOL TERMINATE_IICHAN;

int WORLD_X = -30;
int WORLD_Y = 300;
int WORLD_MAX_X = 2000;
int WORLD_MIN_X = 0;


ii::ii()
{
	WriteToLog("----------------");
	WriteToLog("Creating game...");
	SetSpriteAllocator(NULL);
	SetEventAllocator(NULL);
	SetItemAllocator(NULL);
	SetScreenTextAllocator(NULL);
	SetInstanceAllocator(NULL);
	SetHUD(NULL);

	_previous_time = 0;
	_prev_event_time = 0;
	_seconds = 0;
}

ii::~ii()
{
	WriteToLog("Destroying game...");
	if(GetInstanceAllocator())
	{
		delete GetInstanceAllocator();
		SetInstanceAllocator(NULL);
	}
	if(GetSpriteAllocator())
	{
		delete GetSpriteAllocator();
		SetSpriteAllocator(NULL);
	}
	if(GetEventAllocator())
	{
		delete GetEventAllocator();
		SetEventAllocator(NULL);
	}
	if(GetItemAllocator())
	{
		delete GetItemAllocator();
		SetItemAllocator(NULL);
	}
	if(GetScreenTextAllocator())
	{
		delete GetScreenTextAllocator();
		SetScreenTextAllocator(NULL);
	}
	if(GetHUD())
	{
		delete GetHUD();
		SetHUD(NULL);
	}
	if(snd)
	{
		snd->StopAll();
	}
}

void ii::Draw()
{
	assert(GetSpriteAllocator());
	assert(GetInstanceAllocator());
	assert(GetScreenTextAllocator());
	assert(GetHUD());
		
	

	GetSpriteAllocator()->Draw();
	GetItemAllocator()->Draw();
	GetInstanceAllocator()->Draw();
	GetScreenTextAllocator()->Draw();
	if(GetInstanceAllocator()->GetHUDEnabled())
	{
		if(GLOBAL_QUAKE_ENABLED)
		{
			glPopMatrix();
		}
		GetHUD()->Draw();
	}
}

void ii::Move()
{
	assert(GetInstanceAllocator());
	assert(GetEventAllocator());
	assert(GetScreenTextAllocator());
	assert(GetSpriteAllocator());
	assert(GetItemAllocator());

	GetInstanceAllocator()->ProcessHotkeys();

	if(!GetInstanceAllocator()->GetPause())
	{
		if(current_time - _seconds >= 1000)
		{
			game_time += 1000;
			dbgTime = "Game time: " + IntToString(game_time / 1000);
			_seconds = current_time;
		}
	}

	if(current_time - _previous_time >= TICK)
	{
		GetInstanceAllocator()->Process();
		if(!GetInstanceAllocator()->GetPause())
		{
			GetScreenTextAllocator()->Process();
			GetItemAllocator()->Move();
			GetSpriteAllocator()->Move();
		}

#ifdef _DEBUG
		dbgWorldX = "WORLD X: " + IntToString(WORLD_X);
		dbgWorldY = "WORLD Y: " + IntToString(WORLD_Y);
		dbgEventCount = "Events: " + IntToString(GetEventAllocator()->GetCount());
		dbgSpriteCount = "Sprites: " + IntToString(GetSpriteAllocator()->GetCount());
#endif

		_previous_time = current_time;
	}

	// Отдельно обновляем события

	if(current_time - _prev_event_time >= EVENT_UPDATE_TICK)
	{
		if(!GetInstanceAllocator()->GetPause())
			GetEventAllocator()->Process();
		_prev_event_time = current_time;
	}

	// Если герой погиб, заканичаваем игру
	if(!GetItemAllocator()->GetFirstPlayer())
	{
		TERMINATE_IICHAN = TRUE;
		assert(gui);
		gui->wrapperShowGroup(MENU_FAILURE);
	}



}


void ii::Init()
{
	WriteToLog("Initting game...");
	iiSpriteAllocator* spr_alloc = new iiSpriteAllocator();
	iiEventAllocator* event_alloc = new iiEventAllocator();
	iiItemAllocator* item_alloc = new iiItemAllocator();
	iiScreenTextAllocator* scr_text_alloc = new iiScreenTextAllocator();
	iiInstanceAllocator* inst_alloc = new iiInstanceAllocator();
	HUD* hud = new HUD();
	SetItemAllocator(item_alloc);
	SetSpriteAllocator(spr_alloc);
	SetEventAllocator(event_alloc);
	SetScreenTextAllocator(scr_text_alloc);
	SetInstanceAllocator(inst_alloc);
	SetHUD(hud);
	GetItemAllocator()->SetHUD(hud);
	GetItemAllocator()->SetSpriteAllocator(GetSpriteAllocator());
	GetItemAllocator()->SetInstanceAllocator(GetInstanceAllocator());
	GetItemAllocator()->SetScreenTextAllocator(GetScreenTextAllocator());
	GetEventAllocator()->SetItemAllocator(GetItemAllocator());
	GetEventAllocator()->SetScreenTextAllocator(GetScreenTextAllocator());
	GetEventAllocator()->SetInstanceAllocator(GetInstanceAllocator());
	GetInstanceAllocator()->SetSpriteAllocator(GetSpriteAllocator());
	GetInstanceAllocator()->SetScreenTextAllocator(GetScreenTextAllocator());

	// Иницилизируем систему событий
	GetEventAllocator()->InitMappings();

	// Инициализируем хоткеи

	// F3
	GetInstanceAllocator()->MapHotkey(VK_F3, &iiInstanceAllocator::SwitchPause);
	
	// Q
	GetInstanceAllocator()->MapHotkey(0x51, &iiInstanceAllocator::SwitchQuake);

	// ESCAPE
	GetInstanceAllocator()->MapHotkey(VK_ESCAPE, &iiInstanceAllocator::SwitchMenu);

	WORLD_X = 0;
	WORLD_Y = 0;
	WORLD_MAX_X = 2000;
	WORLD_MIN_X = 0;

	OSL_CURRENT_FILE = "No file";

	TERMINATE_IICHAN = FALSE;

	if(osl_Load("scripts/start.txt", false))
	{
		PLAYER* player = (PLAYER*)GetItemAllocator()->GetFirstPlayer();

		if(player)
		{
#ifdef SIMPLE_HUD
			GetHUD()->SetHealthCount(player->GetHealth());
			GetHUD()->SetAmmoCount(player->GetAmmo());
			GetHUD()->SetScoreCount(player->GetScore());
#else
			// Это, блин, писец ))
			for(int i = 0; i < player->GetHealth(); i++)
			{
				GetHUD()->AddHealth();
			}

			for(int i = 0; i < player->GetAmmo(); i++)
			{
				GetHUD()->AddAmmo();
			}

			for(int i = 0; i < player->GetScore(); i++)
			{
				GetHUD()->AddScore();
			}
#endif
		}
	}
	else
	{
		current_state = GAME_EXITING;
	}

}


void ii::SetWorldMaxX(int value)
{
	WORLD_MAX_X = value;
}

void ii::SetWorldMinX(int value)
{
	WORLD_MIN_X = value;
}