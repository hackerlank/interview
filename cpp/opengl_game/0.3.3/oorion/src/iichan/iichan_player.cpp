#include "stdafx.h"
#include "iichan_player.h"
#include "iichan_small_types.h"

#include "../gui/gui.h"

extern GUI* gui;

PLAYER::PLAYER(int health, int ammo, int score)
{
	__super::CHARACTER();
	SetHealth(health);
	SetAmmo(ammo);
	SetScore(score);
	SetType(itemPlayer);
}

void PLAYER::CollectPowerup(enum POWERUP_TYPE power_type, int count)
{
	char* msg = new char[64];
	switch(power_type)
	{
	case pwrHealth:
		SetHealth(GetHealth() + count);
		sprintf(msg, "Health collected (+%u)\0", count);
		break;
	case pwrAmmo:
		SetAmmo(GetAmmo() + count);
		sprintf(msg, "Ammo collected (+%u)\0", count);
		break;
	case pwrScore:
		SetScore(GetScore() + count);
		sprintf(msg, "Score collected (+%u)\0", count);
		break;
	}

			
	gui->LogOnScreen(msg);
}