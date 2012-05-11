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
		GetHUD()->Draw();
}

void IICHAN::Move()
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

void IICHAN::Init()
{
	IICHANSpriteAllocator* spr_alloc = new IICHANSpriteAllocator();
	IICHANEventAllocator* event_alloc = new IICHANEventAllocator();
	IICHANItemAllocator* item_alloc = new IICHANItemAllocator();
	IICHANScreenTextAllocator* scr_text_alloc = new IICHANScreenTextAllocator();
	HUD* hud = new HUD();
	SetItemAllocator(item_alloc);
	SetSpriteAllocator(spr_alloc);
	SetEventAllocator(event_alloc);
	SetScreenTextAllocator(scr_text_alloc);
	SetHUD(hud);
	GetItemAllocator()->SetHUD(hud);
	GetItemAllocator()->SetSpriteAllocator(spr_alloc);
	GetEventAllocator()->SetItemAllocator(GetItemAllocator());
	GetEventAllocator()->SetScreenTextAllocator(GetScreenTextAllocator());

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


void IICHAN::PrintDebug()
{
//	UINT i = 0;
//	static const font_t* font = FontByName("default");
//	for(vector<CHARACTER*>::iterator it = enemies.begin();
//		it != enemies.end();
//		it++)
//	{
//		CHARACTER* c = *it;
//
//		if(c)
//		{
//#ifdef OPTIMIZE_DRAW
//			i++;
//			char buffer[32];
//			sprintf(buffer, "character #%u: %s\0", i, c->GetVisible() ? "visible" : "not visible");
//			PrintText(buffer, font, 5, 50+i*15);
//#endif
//		}
//	}
//	UINT j = i;
//	for(vector<SPRITE*>::iterator it = background.begin();
//		it != background.end();
//		it++)
//	{
//		SPRITE* c = *it;
//
//		if(c)
//		{
//#ifdef OPTIMIZE_DRAW
//			j++;
//			char buffer[32];
//
//			sprintf(buffer, "sprite #%u: %s\0", i, c->GetVisible() ? "visible" : "not visible");
//			PrintText(buffer, font, 5, 50+j*15);
//#endif
//		}
//	}

}

