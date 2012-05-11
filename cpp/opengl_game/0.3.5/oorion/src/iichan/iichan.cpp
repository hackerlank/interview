#include "stdafx.h"
#include "../osl/osl.h"
#include "iichan.h"


// --------------------------------------------------------------------------
unsigned int current_state;
// --------------------------------------------------------------------------


// Level's folder path
// --------------------------------------------------------------------------
extern char path_levels[MAX_PATH];
// --------------------------------------------------------------------------

int WORLD_X = 0;
int WORLD_Y = 0;
int WORLD_MAX_X = 2000;
int WORLD_MIN_X = 0;


IICHAN::IICHAN()
{
	SetSpriteAllocator(NULL);
	SetEventAllocator(NULL);
	SetItemAllocator(NULL);
	SetScreenTextAllocator(NULL);
	SetInstanceAllocator(NULL);
	SetHUD(NULL);
}

IICHAN::~IICHAN()
{
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
	if(GetInstanceAllocator())
	{
		delete GetInstanceAllocator();
		SetInstanceAllocator(NULL);
	}
	if(GetHUD())
	{
		delete GetHUD();
		SetHUD(NULL);
	}
}

void IICHAN::Draw()
{
	if(GetSpriteAllocator())
		GetSpriteAllocator()->Draw();
	if(GetScreenTextAllocator())
		GetScreenTextAllocator()->Draw();
	if(GetHUD())
	{
		if(GetInstanceAllocator())
		{
			if(GetInstanceAllocator()->GetHUDEnabled())
				GetHUD()->Draw();
		}
		else
			GetHUD()->Draw();
	}
}

void IICHAN::Move()
{
	if(GetInstanceAllocator())
	{
		GetInstanceAllocator()->Process();
		if(!GetInstanceAllocator()->GetPause())
		{
			if(GetEventAllocator())
				GetEventAllocator()->Process();
		}
			if(GetItemAllocator())
				GetItemAllocator()->Move();
		if(GetSpriteAllocator())
			GetSpriteAllocator()->Move();
		if(GetScreenTextAllocator())
			GetScreenTextAllocator()->Process();
	}
	else
	{
		if(GetSpriteAllocator())
			GetSpriteAllocator()->Move();
		if(GetItemAllocator())
			GetItemAllocator()->Move();
		if(GetScreenTextAllocator())
			GetScreenTextAllocator()->Process();
		if(GetEventAllocator())
			GetEventAllocator()->Process();
	}
}

void IICHAN::Init()
{
	IICHANSpriteAllocator* spr_alloc = new IICHANSpriteAllocator();
	IICHANEventAllocator* event_alloc = new IICHANEventAllocator();
	IICHANItemAllocator* item_alloc = new IICHANItemAllocator();
	IICHANScreenTextAllocator* scr_text_alloc = new IICHANScreenTextAllocator();
	IICHANInstanceAllocator* inst_alloc = new IICHANInstanceAllocator();
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

	osl_Load_v_04("levels\\level1.txt");

	PLAYER* player = (PLAYER*)GetItemAllocator()->GetFirstPlayer();

	if(player)
	{
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
	}


}

void IICHAN::SetWorldMaxX(int value)
{
	WORLD_MAX_X = value;
}

void IICHAN::SetWorldMinX(int value)
{
	WORLD_MIN_X = value;
}