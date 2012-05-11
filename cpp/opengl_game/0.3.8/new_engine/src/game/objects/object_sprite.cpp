#include "StdAfx.h"


//#include "../../render/texture.h"

//#include "../phys/phys_misc.h"
//#include "../animation.h"
//#include "../sprite.h"
//#include "../proto.h"

#include "object_sprite.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void _Init(GameObject* obj)
{
	
}

//////////////////////////////////////////////////////////////////////////

GameObject* CreateSprite(Proto* proto, Vector2 coord,bool overr_z, float z, bool fixed)
{
	if (!proto)
		return NULL;
	
	GameObject* object = NULL;

	if (proto->physic)
	{
		ObjPhysic* o = new ObjPhysic;
		o->sap_handle = 0;
		o->objFlags = 0;
		o->physFlags = 0;
		o->SetPhysic();
		object = (GameObject*)o;
	}
	else
	{
		object = new GameObject;
		object->objFlags = 0;
	}

	object->type = objSprite;

	if (LoadObjectFromProto(proto, object))
	{
		DELETESINGLE(object);
		return NULL;
	}

	if (fixed) object->sprite->SetFixed();

	if (overr_z)
		object->sprite->z = z;

	object->aabb.p = coord;

	// TODO: Кривой хак. Хоя вероятно и перерастет в постояный. Но лучше ничего не придумал.
	// В этот момент объектв в SAP еще не добавлен, поэтому в SetAnimation нельзя еще обновлять
	// его состояние в SAP.
	object->ClearPhysic();
	SetAnimation(object, "idle", true, true);
	if (proto->physic) object->SetPhysic();

	AddObject(object);

	return object;
}


GameObject* CreateSprite(const char* proto_name, Vector2 coord, bool fixed)
{
	if (!proto_name)
		return NULL;

	return CreateSprite(GetProto(proto_name), coord, false, 0, fixed);
}

GameObject* CreateSprite(const char* proto_name, Vector2 coord, float z, bool fixed)
{
	if (!proto_name)
		return NULL;

	return CreateSprite(GetProto(proto_name), coord, true, z, fixed);
}


