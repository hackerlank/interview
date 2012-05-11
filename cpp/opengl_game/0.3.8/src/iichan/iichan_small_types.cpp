#include "stdafx.h"
#include "iichan_small_types.h"

void POWERUP::Move()
{
	__super::Move();
}

void EXPLOSION::Move()
{
	__super::Move();
}

void SHADOW::Move()
{
	__super::Move();
}

SHADOW::SHADOW()
{
	SetType(itemShadow);
}

POWERUP::POWERUP(enum POWERUP_TYPE power_type, int count)
{
	SetPowerType(power_type);
	SetCount(count);
	SetDead(false);
	SetType(itemPowerup);
}

EXPLOSION::EXPLOSION()
{
	SetType(itemExplosion);
}