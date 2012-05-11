#ifndef __OBJECT_BULLET_H_
#define __OBJECT_BULLET_H_

#include "object_dynamic.h"
#include "object_character.h"

class ObjBullet : public ObjDynamic
{
public:
	UINT damage;
	ObjCharacter* shooter;

	ObjBullet()
	{
		damage = 0;
		shooter = NULL;

		this->type = objBullet;

		this->SetBullet();
	}


	virtual void Process();

	void Hit(ObjPhysic* obj);
};


ObjBullet* CreateBullet(const char* proto_name, Vector2 coord, ObjCharacter* shooter, WeaponDirection wd);
ObjBullet* CreateBullet(Proto* proto, Vector2 coord, ObjCharacter* shooter, WeaponDirection wd);

#endif // __OBJECT_BULLET_H_