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


// ��������� ��������� � ������ ���� item
bool BULLET::BulletHit(const PhysBaseBody* pb)
{
	assert(pb);

	if (pb->GetType() == bodyDynamic)
	{
		ITEM* item = ((PhysDynamicBody*)pb)->item;

		assert(item);

		if (!item->IsDead())		// ���� ��� �� ������
		{
			// �� �������� �� ����� � ������ �� ������� ��� ������
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
