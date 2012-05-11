#include "StdAfx.h"

#include "object_spatial_trigger.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ObjSpatialTrigger* CreateSpatialTrigger(CAABB aabb, ObjectType actors_type, UINT actor_id, int script, bool on_first_collision_only)
{
	ObjSpatialTrigger* trigger = new ObjSpatialTrigger;

	trigger->aabb = aabb;
	trigger->actor_id = actor_id;
	trigger->actors_type = actors_type;
	trigger->scriptOnTrigger = script;
	trigger->on_first_collision_only = on_first_collision_only;

	trigger->sprite = new Sprite();
	trigger->sprite->SetVisible();
	trigger->sprite->frameWidth = trigger->aabb.W*2;
	trigger->sprite->frameHeight = trigger->aabb.H*2;
	trigger->sprite->z = -1;

	if (!actor_id && actors_type == objBullet)
		trigger->SetBulletCollidable();

	AddObject(trigger);

	return trigger;
}

//////////////////////////////////////////////////////////////////////////

ObjSpatialTrigger::ObjSpatialTrigger()
{
	type = objSpatialTrigger;
	actor_id = 0;
	actors_type = objNone;
	on_first_collision_only = false;
	scriptOnTrigger = LUA_NOREF;
}

ObjSpatialTrigger::~ObjSpatialTrigger()
{
	SCRIPT::RemoveFromRegistry(scriptOnTrigger);
	//scriptOnTrigger = LUA_NOREF;
}

//////////////////////////////////////////////////////////////////////////

void ObjSpatialTrigger::Trigger(GameObject* obj)
{
	if (this->IsDead())
		return;

	if ( (this->actor_id && this->actor_id == obj->id) ||
		(!this->actor_id && this->actors_type == obj->type) )
	{
		// push
			if (SCRIPT::ExecChunkFromReg(this->scriptOnTrigger))
			{
				// В скрипте произошла какая-то ошибка. Будем испльзовать стандартный в следующий раз.
				SCRIPT::RemoveFromRegistry(this->scriptOnTrigger);
				this->scriptOnTrigger = LUA_NOREF;

				this->SetDead();
			}

	}
}
