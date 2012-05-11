#include "StdAfx.h"

#include "../misc.h"

#include "../game/objects/object_player.h"
#include "../game/objects/object_sprite.h"
#include "../game/objects/object_spatial_trigger.h"

#include "api.h"
#include "luathread.h"

//////////////////////////////////////////////////////////////////////////
extern ObjPlayer* _PLAYER;

extern lua_State* lua;
//////////////////////////////////////////////////////////////////////////
void PushVector(lua_State* L, Vector2& v);
void PushObject(lua_State* L, GameObject* o);
void PushPhysic(lua_State* L, ObjPhysic* o);
void PushDynamic(lua_State* L, ObjDynamic* o);
void PushCharacter(lua_State* L, ObjCharacter* o);
void PushPlayer(lua_State* L, ObjPlayer* o);
//////////////////////////////////////////////////////////////////////////

// Помещает в стек таблицу, представляющую вектор
void PushVector(lua_State* L, Vector2& v)
{
	// Стек: obj
	lua_createtable(L, 0, 2);	// Стек: obj vector

	lua_pushnumber(L, v.x); lua_setfield(L, -2, "x");
	lua_pushnumber(L, v.y); lua_setfield(L, -2, "y");
}

// Помещает в стек таблицу, представляющую AABB
void PushAABB(lua_State* L, CAABB& a)
{
	// Стек: obj
	lua_createtable(L, 0, 3);	// Стек: obj aabb

	PushVector(L, a.p);		lua_setfield(L, -2, "p");
	lua_pushnumber(L, a.H); lua_setfield(L, -2, "H");
	lua_pushnumber(L, a.W); lua_setfield(L, -2, "W");	
}

// Помещает в стек таблицу, представлющую игровой объект
void PushObject(lua_State* L, GameObject* o)
{
	lua_newtable(L);	// Стек: obj

	lua_pushinteger(L, o->id);		lua_setfield(L, -2, "id");
	lua_pushinteger(L, o->type);	lua_setfield(L, -2, "type");
	PushAABB(L, o->aabb);			lua_setfield(L, -2, "aabb");

	lua_newtable(L);	// Стек: obj flags
	lua_pushboolean(L, o->IsPhysic());	lua_setfield(L, -2, "physic");
	lua_pushboolean(L, o->IsDead());	lua_setfield(L, -2, "dead");
	lua_setfield(L, -2, "flags");	// Стек: obj

	if(o->sprite)
	{
		lua_newtable(L);	// Стек: obj sprite
		lua_pushboolean(L, o->sprite->IsMirrored());	lua_setfield(L, -2, "mirrored");
		lua_pushboolean(L, o->sprite->IsFixed());		lua_setfield(L, -2, "fixed");
		lua_pushboolean(L, o->sprite->IsVisible());		lua_setfield(L, -2, "visible");
		lua_pushboolean(L, o->sprite->IsLooped());		lua_setfield(L, -2, "looped");
		lua_pushboolean(L, o->sprite->IsAnimDone());	lua_setfield(L, -2, "animDone");
		lua_pushstring(L, o->sprite->cur_anim.c_str()); lua_setfield(L, -2, "cur_anim");
		lua_setfield(L, -2, "sprite");	// Стек: obj
	}

	if(o->IsPhysic())	
		PushPhysic(L, (ObjPhysic*)o);

	if (o->type == objPlayer)
		PushPlayer(L, (ObjPlayer*)o);

	// Стек: obj
}

// Добавляет в таблице на вершине стека поля физического игрового объекта
void PushPhysic(lua_State* L, ObjPhysic* o)
{
	// Стек: obj
	lua_newtable(L);	// Стек: obj phFlags
	lua_pushboolean(L, o->IsSolid());	lua_setfield(L, -2, "solid");
	lua_pushboolean(L, o->IsBulletCollidable());	lua_setfield(L, -2, "bulletCollideable");
	lua_pushboolean(L, o->IsDynamic());	lua_setfield(L, -2, "dynamic");
	lua_setfield(L, -2, "phFlags");	// Стек: obj

	if (o->IsDynamic())
		PushDynamic(L, (ObjDynamic*)o);
}

// Добавляет в таблице на вершине стека поля динамического физического игрового объекта
void PushDynamic(lua_State* L, ObjDynamic* o)
{
	// Стек: obj
	PushVector(L, o->acc);						lua_setfield(L, -2, "acc");
	PushVector(L, o->vel);						lua_setfield(L, -2, "vel");
	lua_pushinteger(L, o->walk_acc);			lua_setfield(L, -2, "walk_acc");
	PushVector(L, o->gravity);					lua_setfield(L, -2, "gravity");
	lua_pushinteger(L, o->jump_vel);			lua_setfield(L, -2, "jump_vel");
	lua_pushinteger(L, o->max_x_vel);			lua_setfield(L, -2, "max_x_vel");
	lua_pushinteger(L, o->max_y_vel);			lua_setfield(L, -2, "max_y_vel");
	lua_pushboolean(L, o->movementDirectionX);	lua_setfield(L, -2, "movementDirectionX");

	lua_pushinteger(L, o->movement);			lua_setfield(L, -2, "movement");
	lua_pushinteger(L, o->activity);			lua_setfield(L, -2, "activity");
}

// Добавляет в таблице на вершине стека поля игрового объекта персонажа
void PushCharacter(lua_State* L, ObjCharacter* o)
{
	// Стек: obj
	lua_pushinteger(L, o->health);			lua_setfield(L, -2, "health");
	lua_pushinteger(L, o->gunDirection);	lua_setfield(L, -2, "gunDirection");
}

// Добавляет в таблице на вершине стека поля игрового объекта игрока
void PushPlayer(lua_State* L, ObjPlayer* o)
{
	// Стек: obj
	PushCharacter(L, o);
	lua_pushboolean(L, o->controlEnabled);	lua_setfield(L, -2, "controlEnabled");
}

//////////////////////////////////////////////////////////////////////////

// Возвращает таблицу, предствавлющую собой объект игрока
int scriptApi::GetPlayer(lua_State* L)
{
	if (_PLAYER)
	{
		PushObject(L, _PLAYER);
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка. Игрока нет.");
		return 0;
	}	

	return 1;
}

// Возвращает таблицу, предствавлющую собой игровой объект
int scriptApi::GetObject(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = GetGameObject(id);
	if (obj)
	{
		PushObject(L, obj);
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка. Объекта с таким id нет.");
		return 0;
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////

// Изменяет координаты игрового объекта
int scriptApi::SetObjPos(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "X coord expected");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Y coord expected");

	UINT id = lua_tointeger(L, 1);
	scalar x = lua_tonumber(L, 2);
	scalar y = lua_tonumber(L, 3);

	GameObject* obj = GetGameObject(id);
	if (obj)
	{
		obj->aabb.p.x = x;
		obj->aabb.p.y = y;

		/*if(obj->IsPhysic())
		{
		if (((ObjPhysic*)obj)->IsDynamic())
		{
		((ObjDynamic*)obj)->old_aabb = obj->aabb;
		}
		}*/
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка изменения координат объекта. Объекта с таким id нет.");
	}

	return 0;
}

// Изменяет ускорение динамического игрового объекта
int scriptApi::SetDynObjAcc(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "X acceleration expected");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Y acceleration expected");

	UINT id = lua_tointeger(L, 1);
	GameObject* obj = GetGameObject(id);
	if (obj && obj->IsPhysic() && ((ObjPhysic*)obj)->IsDynamic())
	{
		((ObjDynamic*)obj)->acc.x = lua_tonumber(L, 2);
		((ObjDynamic*)obj)->acc.y = lua_tonumber(L, 3);
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка изменения ускорения объекта. Объекта с таким id нет или он не динамический.");
	}

	return 0;
}

// Изменяет скорость динамическго игрового объекта
int scriptApi::SetDynObjVel(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "X acceleration expected");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Y acceleration expected");

	UINT id = lua_tointeger(L, 1);
	GameObject* obj = GetGameObject(id);
	if (obj && obj->IsPhysic() && ((ObjPhysic*)obj)->IsDynamic())
	{
		((ObjDynamic*)obj)->vel.x = lua_tonumber(L, 2);
		((ObjDynamic*)obj)->vel.y = lua_tonumber(L, 3);
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка изменения скорости объекта. Объекта с таким id нет или он не динамический.");
	}

	return 0;
}

// Включает/отключает контроль игрока
int scriptApi::EnablePlayerControl(lua_State* L)
{
	if (_PLAYER)
	{
		luaL_argcheck(L, lua_isboolean(L, 1), 1, "Boolean expected");

		_PLAYER->controlEnabled = lua_toboolean(L, 1);		
	}

	return 0;
}

// Устанавливает тип движения динамического игрового объекта
int scriptApi::SetDynObjMovement(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Movement type expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = GetGameObject(id);
	if (obj && obj->IsPhysic() && ((ObjPhysic*)obj)->IsDynamic())
	{
		((ObjDynamic*)obj)->movement = (ObjectMovementType)lua_tointeger(L, 2);
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка изменения типа движения объекта. Объекта с таким id нет или он не динамический.");
	}

	return 0;
}

// Устанавливает направление движения динамического игрового объекта
int scriptApi::SetDynObjMoveDirX(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isboolean(L, 2), 2, "Movement type expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = GetGameObject(id);
	if (obj && obj->IsPhysic() && ((ObjPhysic*)obj)->IsDynamic())
	{
		((ObjDynamic*)obj)->movementDirectionX = lua_toboolean(L, 2);
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка изменения направления движения объекта. Объекта с таким id нет или он не динамический.");
	}

	return 0;
}

int scriptApi::SetObjSpriteMirrored(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isboolean(L, 2), 2, "Boolean expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = GetGameObject(id);
	if (obj)
	{
		if (lua_toboolean(L, 2))
			obj->sprite->SetMirrored();
		else
			obj->sprite->ClearMirrored();
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка SetObjSpriteMirrored. Объекта с таким id нет или он не динамический.");
	}

	return 0;
}

int scriptApi::SetObjAnim(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isstring(L, 2), 2, "String expected");
	luaL_argcheck(L, lua_isboolean(L, 3), 3, "Boolean expected");
	luaL_argcheck(L, lua_isboolean(L, 4), 4, "Boolean expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = GetGameObject(id);
	if (obj)
	{
		SetAnimation(obj, string(lua_tostring(L, 2)), lua_toboolean(L, 3), lua_toboolean(L, 4));
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка SetObjAnim. Объекта с таким id нет или он не динамический.");
	}

	return 0;
}

int scriptApi::SetObjProcessor(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isfunction(L, 2) || lua_isnil(L, 2), 2, "Function or nil expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = GetGameObject(id);
	if (obj)
	{	
		if(lua_isnil(L, 2))
		{
			SCRIPT::RemoveFromRegistry(obj->scriptProcess);
			obj->scriptProcess = LUA_NOREF;
		}
		else
		{
			//obj->scriptProcess = ::NewThread(L);
			lua_xmove(L, lua, 1);
			obj->scriptProcess = SCRIPT::AddToRegistry();
		}
	}
	else
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка изменения обработчика объекта. Объекта с таким id нет или он не динамический.");
	}


	return 0;
}

int scriptApi::SetDynObjJumpVel(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "jump vel expected");

	GameObject* obj = GetGameObject(lua_tointeger(L, 1));
	if (obj)
	{
		if (obj->IsPhysic() && ((ObjPhysic*)obj)->IsDynamic())
		{
			((ObjDynamic*)obj)->jump_vel = lua_tonumber(L, 2);
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int scriptApi::CreateSprite(lua_State* L)
{
	CHECKGAME;

	luaL_argcheck(L, lua_isstring(L, 1), 1, "Ожидается имя прототипа");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Ожидается координата X");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Ожидается координата Y");

	int num = lua_gettop ( L );
	bool isFixed = false;
	float z = 0;
	bool overrride_z = false;

	if (num == 4)
	{
		if (lua_isboolean(L, 4))
		{
			isFixed = lua_toboolean(L, 4);
		}
		else if (lua_isnumber(L, 4))
		{
			z = lua_tonumber(L, 4);
			overrride_z = true;
		}
		else
			luaL_argcheck(L, false, 4, "Boolean or number expected");		
	}
	else if (num == 5)
	{
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "Number expected");	
		luaL_argcheck(L, lua_isboolean(L, 5), 5, "Boolean expected");	
		
		z = lua_tonumber(L, 4);
		isFixed = lua_toboolean(L, 5);
		overrride_z = true;
	}

	const char* proto_name = lua_tostring(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);

	lua_pop(L, num);	// Стек: 

	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Cоздаем спрайт: %s", proto_name);
	
	GameObject* obj = NULL;
	if (overrride_z)
		obj = ::CreateSprite(proto_name, Vector2(x, y), z, isFixed);
	else
		obj = ::CreateSprite(proto_name, Vector2(x, y), isFixed);

	if (obj)
	{
		if (obj->id != 0)
			lua_pushnumber(L, obj->id);	// Стек: obj->id

	}
	else
	{
		lua_pushnil(L);				// Стек: nil
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Ошибка cоздания спрайта");
	}

	return 1;
}

int scriptApi::CreatePlayer(lua_State* L)
{
	CHECKGAME;

	luaL_argcheck(L, lua_isstring(L, 1), 1, "Ожидается имя прототипа");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Ожидается координата X");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Ожидается координата Y");
	

	const char* proto_name = lua_tostring(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int health = 0;
	
	bool override_z = false;
	float z = 0;

	int num = lua_gettop(L);
	if (num == 4)
	{
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "Ожидается координата Z");

		z = lua_tonumber(L, 4);
		override_z = true;
	}


	lua_pop(L, num);

	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Cоздаем игрока: %s", proto_name);

	int result = -1;
	GameObject* obj = NULL;
	
	if (override_z)
		obj = ::CreatePlayer(proto_name, Vector2(x, y), z);
	else
		obj = ::CreatePlayer(proto_name, Vector2(x, y));

	if (obj)
	{
		result = obj->id;
	}
	else
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Ошибка cоздания игрока");
	}

	if (result == -1)
	{
		lua_pushnil(L);				// Стек: nil
	}
	else
	{
		lua_pushnumber(L, result);	// Стек: result
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////

extern int on_change_player_processor;

int scriptApi::SetOnChangePlayerProcessor(lua_State* L)
{
	CHECKGAME;

	luaL_argcheck(L, lua_isfunction(L, 1) || lua_isnil(L, 1), 1, "Function or nil expected");
	
	if(lua_isnil(L, 1))
	{
		SCRIPT::RemoveFromRegistry(on_change_player_processor);
		on_change_player_processor = LUA_NOREF;
	}
	else
	{
		//obj->scriptProcess = ::NewThread(L);
		lua_xmove(L, lua, 1);
		on_change_player_processor = SCRIPT::AddToRegistry();
	}

	return 0;
}

int scriptApi::BlockPlayerChange(lua_State* L)
{
	CHECKGAME;
	::BlockPlayerChange();
	return 0;
}

int scriptApi::UnblockPlayerChange(lua_State* L)
{
	CHECKGAME;
	::UnblockPlayerChange();
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int scriptApi::CreateSpatialTrigger(lua_State* L)
{
	CHECKGAME;

	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Ожидается координата X1");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "Ожидается координата Y1");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "Ожидается координата X2");
	luaL_argcheck(L, lua_isnumber(L, 4), 4, "Ожидается координата Y2");
	luaL_argcheck(L, lua_isnumber(L, 5) || lua_isnil(L, 5), 5, "Ожидается тип объекта");
	luaL_argcheck(L, lua_isnumber(L, 6) || lua_isnil(L, 6), 6, "Ожидается id объектов");
	luaL_argcheck(L, lua_isboolean(L, 7), 7, "bool");
	luaL_argcheck(L, lua_isfunction(L, 8) || lua_isnil(L, 8), 8, "Function or nil expected");
	
	if (lua_isnil(L, 5) && lua_isnil(L, 6))
	{
		luaL_error(L, "id и тип не должны быть одновременно nil");
	}
	
	float x1 = lua_tonumber(L, 1);
	float y1 = lua_tonumber(L, 2);
	float x2 = lua_tonumber(L, 3);
	float y2 = lua_tonumber(L, 4);
	ObjectType type = (ObjectType)lua_tointeger(L, 5);
	UINT id = lua_tointeger(L, 6);	
	bool on_first_collision_only = lua_toboolean(L, 7);
	

	
	// TODO: Если на стеке >8 значений, то функция не последняя.
	lua_xmove(L, lua, 1);
	lua_isfunction(L, -1);
	int script = SCRIPT::AddToRegistry();

	lua_pop(L, lua_gettop(L));

	ObjSpatialTrigger* tr = ::CreateSpatialTrigger(CAABB(x1,y1,x2,y2), type, id, script, on_first_collision_only);
	
	if (tr)
	{
		lua_pushnumber(L, tr->id);
	}
	else
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Ошибка cоздания игрока");
		lua_pushnil(L);
	}

	return 1;
}