#ifndef __OBJECT_WEAPON_H_
#define __OBJECT_WEAPON_H_

// ��������������� ����������� ������ �������.
// ��� ��� �� ����� ��������, �� � ������ ������� ��� �����
// �� ���� ������� ���������� �� ����.
class ObjCharacter;


// ����������� ��������. ������������, ����� ���������� ����.
enum WeaponDirection { wdRight, wdLeft, wdUpLeft, wdUpRight, wdDownLeft, wdDownRight/*, wdUp, wdDown*/ };



//////////////////////////////////////////////////////////////////////////

class Weapon
{
public:
	USHORT reload_time;			// �����, ������� ������� �� ����������� ����� ��������
	DWORD last_reload_tick;		// ����� ������ �����������

	Proto* bullet_proto;		// ��������, �� �������� ��������� ����
	USHORT bullets_count;		// ������� ���������� ����
	bool is_infinite;			// ���������� �� ����

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