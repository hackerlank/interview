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

	if(GetLife() <= 0)
	{
		if(GetSprite())
			GetSprite()->SetDead(true);
	}
}

void POWERUP::Move()
{
	__super::Move();
}

void EXPLOSION::Move()
{
	__super::Move();
	//if(GetSprite())
	//{
	//	if(!GetSprite()->GetReverse())
	//	{		
	//		if(GetSprite()->GetCurrentPattern() -
	//			GetSprite()->GetStartPattern() == GetSprite()->GetPatternCount()-1)
	//		{
	//			GetSprite()->SetDead(true);
	//		}
	//		else
	//			GetSprite()->SetCurrentPattern(GetSprite()->GetCurrentPattern() + 1);
	//	}
	//	else
	//	{
	//		if(GetSprite()->GetStartPattern() -
	//			GetSprite()->GetCurrentPattern() == GetSprite()->GetPatternCount()-1)
	//		{
	//			GetSprite()->SetDead(true);
	//		}
	//		else
	//		{
	//			if(GetSprite()->GetCurrentPattern() == 0)
	//				GetSprite()->SetCurrentPattern(GetSprite()->GetStartPattern());
	//			else
	//				GetSprite()->SetCurrentPattern(GetSprite()->GetCurrentPattern() - 1);
	//		}
	//	}
	//}
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