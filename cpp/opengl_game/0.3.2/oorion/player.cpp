#include "stdafx.h"
#include "player.h"
#include "iichan_small_types.h"

#include "gui.h"

extern GUI* gui;

void PLAYER::CollectPowerUp(enum POWERUP_TYPE type, int count)
{
	char* msg = new char[64];
	switch(type)
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