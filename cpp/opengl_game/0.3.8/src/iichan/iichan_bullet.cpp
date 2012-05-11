#include "stdafx.h"
#include "iichan_bullet.h"
#include "phys/phys_dynamicbody.h"

BULLET::BULLET()
{
	SetType(itemBullet);
}



void BULLET::Move()
{
	__super::Move();
	/*int dx;
	if(GetSprite())
	{
	GetSprite()->GetMirror() ? dx = -GetSpeed() : dx = GetSpeed();
	GetSprite()->SetX(GetSprite()->GetX() + dx);
	}*/
}


// Обработка попадания в другое тело item
bool BULLET::BulletHit(const PhysBaseBody* pb)
{
	assert(pb);

	if (pb->GetType() == bodyDynamic)
	{
		ITEM* item = ((PhysDynamicBody*)pb)->item;

		assert(item);

		if (!item->IsDead())		// Тело еще не мертво
		{
			// Не стреляем по своим и только по игрокам или врагам
			if (item->GetType() != shooter->GetType() &&
				(item->GetType() == itemPlayer || item->GetType() == itemEnemy))
			{
				((CHARACTER*)item)->Ouch(damage);
			}
			else
				return false;
		}
		else
			return false;
	}

	SetDead(true);

	return true;
}
