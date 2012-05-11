#include "StdAfx.h"

#include "object_bullet.h"

#include "weapon.h"

//////////////////////////////////////////////////////////////////////////

extern DWORD internal_time;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Загрузка оружия из прототипа
BOOL LoadWeaponFromProto(Proto* proto, Weapon* weapon)
{
	if (!proto || !weapon)
		return TRUE;

	weapon->bullets_count = (USHORT)proto->bullets_count;
	if (proto->bullets_count < 0)
	{
		// Бесконечный боезапас
		weapon->is_infinite = true;
	}

	weapon->reload_time = proto->reload_time;
	weapon->bullet_proto = proto;

	return FALSE;
}

// Создание оружия
Weapon* CreateWeapon(Proto* proto)
{
	if (!proto)
		return NULL;

	Weapon* weapon = new Weapon();

	if (LoadWeaponFromProto(proto, weapon))
	{
		DELETESINGLE(weapon);
		return NULL;
	}

	return weapon;
}

Weapon* CreateWeapon(const char* proto_name)
{
	if (!proto_name)
		return NULL;

	return CreateWeapon(GetProto(proto_name));
}

//////////////////////////////////////////////////////////////////////////

// Выстрел из оружия
void Weapon::Fire(ObjCharacter* shooter, Vector2 coord, WeaponDirection wd)
{
	CreateBullet(this->bullet_proto, coord, shooter, wd); 
	last_reload_tick = internal_time;
	bullets_count--;
}

// Проверка на перезаряженность.
bool Weapon::IsReloaded()
{
	return  internal_time - last_reload_tick >= reload_time;
}

// Проверка на готовность оружия к стрельбе
bool Weapon::IsReady()
{
	return (is_infinite || bullets_count > 0) && IsReloaded();
}