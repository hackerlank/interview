#include "StdAfx.h"

#include "object_character.h"


#include "object_bullet.h"

//////////////////////////////////////////////////////////////////////////

//#define SHOOTER_SPEED_ADDS_TO_BULLETS

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Загрузка пули из прототипа. Рассчет скорости пули.
BOOL LoadBulletFromProto(Proto* proto, ObjBullet* bullet, WeaponDirection wd)
{
	if (!proto || !bullet)
		return TRUE;

	switch (wd)
	{
	case wdLeft:
	case wdRight:
		bullet->acc.x = proto->bullet_vel;
		bullet->acc.y = 0;
		break;
	case wdUpLeft:
	case wdUpRight:
		bullet->acc.x = proto->bullet_vel * (M_SQRT2 * 0.5f);	// cos 45
		bullet->acc.y = -proto->bullet_vel * (M_SQRT2 * 0.5f);	// sin 45
		break;
	case wdDownLeft:
	case wdDownRight:
		bullet->acc.x = proto->bullet_vel * (M_SQRT2 * 0.5f);	// cos 45
		bullet->acc.y = proto->bullet_vel * (M_SQRT2 * 0.5f);	// sin 45
		break;
	}

	switch (wd)
	{
	case wdLeft:
	case wdUpLeft:
	case wdDownLeft:
		bullet->sprite->SetMirrored();
		break;
	}

	if (bullet->sprite->IsMirrored())
		bullet->acc.x = -bullet->acc.x;


	bullet->max_x_vel = fabs(bullet->acc.x);
	bullet->max_y_vel = fabs(bullet->acc.y);

	bullet->damage = proto->bullet_damage;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

ObjBullet* CreateBullet(Proto* proto, Vector2 coord, ObjCharacter* shooter, WeaponDirection wd)
{
	if (!proto)
		return NULL;

	ObjBullet* bullet = new ObjBullet();

	

	if (LoadObjectFromProto(proto, (GameObject*)bullet) || 
		LoadBulletFromProto(proto, bullet, wd) ||
		CheckAnimations((GameObject*)bullet, objBullet))
	{
		DELETESINGLE(bullet);
		return NULL;
	}

	// TODO: Кривой хак. Хоя вероятно и перерастет в постояный. Но лучше ничего не придумал.
	// В этот момент объектв в SAP еще не добавлен, поэтому в SetAnimation нельзя еще обновлять
	// его состояние в SAP.
	bullet->ClearPhysic();
	
	switch (wd)
	{
	case wdLeft:
	case wdRight:
		SetAnimation((GameObject*)bullet, "straight" ,true, true);
		break;
	case wdUpLeft:
	case wdUpRight:
		SetAnimation((GameObject*)bullet, "diagup" ,true, true);
		break;
	case wdDownLeft:
	case wdDownRight:
		SetAnimation((GameObject*)bullet, "diagdown" ,true, true);
		break;
	}

	bullet->SetPhysic();

	bullet->shooter = shooter;
	bullet->aabb.p = coord;
	
	AddObject(bullet);

	return bullet;
}

ObjBullet* CreateBullet(const char* proto_name, Vector2 coord, ObjCharacter* shooter, WeaponDirection wd)
{
	if (!proto_name)
		return NULL;

	return CreateBullet(GetProto(proto_name), coord, shooter, wd);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ObjBullet::Process()
{
	if (activity == oatDying)
	{
		// Тут делаем взрыв

		this->SetDead();
	}
}

// Попадание пули во что-то (пересечение ее с физическим объектом)
void ObjBullet::Hit(ObjPhysic* obj)
{
	if (this->IsDead() || activity == oatDying)
		return;

	if (!obj->IsBulletCollidable())
		return;

	if (this->shooter == obj)
		return;

	/*if (this->shooter->type == obj->type)
		return;*/

	if (obj->IsDynamic())
	{
		
		switch (obj->type)
		{
		case objPlayer:
			((ObjCharacter*)obj)->ReceiveDamage(this->damage);
			break;
		case objBullet:
			((ObjDynamic*)obj)->activity = oatDying;
			break;
		}
	}

	this->activity = oatDying;


}
