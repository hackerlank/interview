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
	SetAI(aiHuman);
}

void PLAYER::CollectPowerup(enum POWERUP_TYPE power_type, int count)
{
	switch(power_type)
	{
	case pwrHealth:
		SetHealth(GetHealth() + count);
		break;
	case pwrAmmo:
		SetAmmo(GetAmmo() + count);
		break;
	case pwrScore:
		SetScore(GetScore() + count);
		break;
	}
}

void PLAYER::Move()
{
	__super::Move();
}