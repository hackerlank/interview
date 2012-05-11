#include "stdafx.h"
#include "iichan_small_types.h"

void BULLET::Move()
{
	__super::Move();
	int dx;
	if(GetSprite())
	{
		GetSprite()->GetMirror() ? dx = -GetSpeed() : dx = GetSpeed();
		GetSprite()->SetX(GetSprite()->GetX() + dx);
	}
}

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

BULLET::BULLET()
{
	SetType(itemBullet);
}

SHADOW::SHADOW()
{
	SetType(itemShadow);
}