#ifndef __OBJECT_WEAPON_H_
#define __OBJECT_WEAPON_H_

// Предварительное объявляение класса стрелка.
// Мне жто не очень нравится, но с другой стороны все равно
// от него никакой информации не надо.
class ObjCharacter;


// Направление стрельбы. Используется, чтобы развернуть пулю.
enum WeaponDirection { wdRight, wdLeft, wdUpLeft, wdUpRight, wdDownLeft, wdDownRight/*, wdUp, wdDown*/ };



//////////////////////////////////////////////////////////////////////////

class Weapon
{
public:
	USHORT reload_time;			// Время, которое тратися на перезарядку после выстрела
	DWORD last_reload_tick;		// Время начала перезарядки

	Proto* bullet_proto;		// Прототип, по которому создается пуля
	USHORT bullets_count;		// Текущее количество пуль
	bool is_infinite;			// Бесконечны ли пули

	Weapon()
	{
		reload_time = 0;
		last_reload_tick = 0;

		bullet_proto = NULL;
		bullets_count = 0;
		is_infinite = false;
	}

	void Fire(ObjCharacter* shooter, Vector2 coord, WeaponDirection wd);
	bool IsReloaded();
	bool IsReady();
};

Weapon* CreateWeapon(const char* proto_name);

#endif // __OBJECT_BULLET_H_