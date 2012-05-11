#include "StdAfx.h"

#include "../misc.h"

#include "api.h"
#include "timerevent.h"

#include "luathread.h"

//////////////////////////////////////////////////////////////////////////

extern lua_State* lua;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

list<LuaThread*> threads;
typedef list<LuaThread*>::iterator ThreadIter;

ThreadIter GetThread(lua_State* c)
{
	list<LuaThread*>::iterator it;
	for(list<LuaThread*>::iterator it = threads.begin();
		it != threads.end();
		it++)
	{
		LuaThread* lt = *it;
		if (lt->lThread == c)
			return it;
	}
	return threads.end();
}

UINT NewThread(lua_State* L)
{
	// ��� �� ������� luaB_cocreate ����� lbaselib.c
	// ����: func
	lua_State *NL = lua_newthread(L); // ����: func thread

	luaL_argcheck(L, lua_isfunction(L, 1) && !lua_iscfunction(L, 1), 1,
		"Lua function expected");

	lua_pushvalue(L, 1);  // ����: func thread func
	lua_xmove(L, NL, 1);  /* move function from L to NL */
	// ����� ���� �� ������� luaB_cocreate ����� lbaselib.c

	// ����: func thread

	lua_pushvalue(L, -1);  // ����: func thread thread

	LuaThread* lt = new LuaThread();
	//lt->refKey = luaL_ref(lua, LUA_REGISTRYINDEX);	
	lt->refKey = SCRIPT::AddToRegistry();	// ����: func thread
	lt->masterState = L;
	lt->lThread = NL;
	threads.push_back(lt);

	return lt->refKey;
}

// ������� ����� coroutine. ���������� ������� ������� ����������
// lua coroutines.create, �� ����������� ����, ��� 
// ���������� ��������� coroutine.
int scriptApi::NewThread(lua_State* L)
{
	::NewThread(L);

	return 1;
}



// ������� ���������������� coroutine, �� ������� �������. ���������� 
// ������� ������� ���������� lua coroutines.yield, �� ������� ������ 
// �������� - ����� ����������, �� ������� coroutine ����� ��������������.
int scriptApi::Wait(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1,
		"Number expected");

	if (lua_pushthread(L))
	{
		// lua_pushthread ������ true, ���� �� �������� ���������� �������� �����
		luaL_where(L, 0);
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_SCRIPT_EV, "%s������� ������������ �������� �����.", lua_tostring(L, -1));
		lua_pop(L, lua_gettop(L));
		return 0;
	}
	else
	{
		// lua_pushthread ������� false, �� �� � �������� ������, ������� �� ����� �� ������ ��������
		lua_pop(L, 1);
	}

	ThreadIter it = GetThread(L);
	if (it != threads.end())
	{
		AddThreadTimerEvent(lua_tointeger(L, 1), (*it)->refKey);
	}
	lua_remove(L, 1);


	return lua_yield(L, lua_gettop(L));
}

// ���������� ���������� ���������������� coroutine, ����������
// ������, �� ������� ������ ����.
int Resume(ThreadIter it)
{
	LuaThread* lt = *it;
	SCRIPT::GetFromRegistry(lt->refKey);
	UINT res = luaB_coresume(lt->masterState);

	if (lua_status(lt->lThread) != LUA_YIELD) 
	{ 
		if (lua_status(lt->lThread) > 1)		// ������ ������
		{
			const char* err = lua_tostring(lua, -1);
			sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "%s", err);
		}

		SCRIPT::RemoveFromRegistry(lt->refKey);
		DELETESINGLE(lt);
		threads.erase(it);
	}

	return res;
}

// ���������� ���������� ���������������� coroutine.
// ���������� ������� coroutines.resume, ������ �������� 
// ��������� ������, ��� ��� ������� ���� ����� ��������� ����� ������.
int scriptApi::Resume(lua_State* L)
{
	lua_State *co = lua_tothread(L, 1);
	luaL_argcheck(L, co, 1, "coroutine expected");
	ThreadIter it = GetThread(co);
	if (it != threads.end())
	{
		return ::Resume(it);
	}


	return luaB_coresume(L);
}

// ���������� ��� ���������� ������� �������, �������� ����������� ������ �����
void ProcessThread(int r)
{
	LuaThread* lt = NULL;
	ThreadIter it;
	for(it = threads.begin();
		it != threads.end();
		it++)
	{
		lt = *it;
		if (lt->refKey == r)
			break;
	}

	if (lt)
	{		
		Resume(it);
	}
}