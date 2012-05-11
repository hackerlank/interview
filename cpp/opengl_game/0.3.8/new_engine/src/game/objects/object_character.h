#ifndef __OBJECT_CHARACTER_H_
#define __OBJECT_CHARACTER_H_

#include "object_dynamic.h"
#include "weapon.h"

enum CharacterGunDirection { cgdNone, cgdUp, cgdDown };

class ObjCharacter abstract : public ObjDynamic
{
public:
	SHORT health;
	
	CharacterGunDirection gunDirection;	

	UINT gunReloadTick;
	UINT gunReloadTime;

	Weapon* weapon;

	ObjCharacter()
	{
		health = 0;
		gunDirection = cgdNone;

		weapon = NULL;
	}

	virtual ~ObjCharacter()
	{
		DELETESINGLE(weapon);
	}

	void ReceiveDamage(UINT damage)
	{
		health -= damage;

		if (health <= 0)
			activity = oatDying;
	}
};

#endif // __OBJECT_CHARACTER_H_