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
	// Код из функции luaB_cocreate файла lbaselib.c
	// Стек: func
	lua_State *NL = lua_newthread(L); // Стек: func thread

	luaL_argcheck(L, lua_isfunction(L, 1) && !lua_iscfunction(L, 1), 1,
		"Lua function expected");

	lua_pushvalue(L, 1);  // Стек: func thread func
	lua_xmove(L, NL, 1);  /* move function from L to NL */
	// Конец кода из функции luaB_cocreate файла lbaselib.c

	// Стек: func thread

	lua_pushvalue(L, -1);  // Стек: func thread thread

	LuaThread* lt = new LuaThread();
	//lt->refKey = luaL_ref(lua, LUA_REGISTRYINDEX);	
	lt->refKey = SCRIPT::AddToRegistry();	// Стек: func thread
	lt->masterState = L;
	lt->lThread = NL;
	threads.push_back(lt);

	return lt->refKey;
}

// Создает новую coroutine. Аналогична команде базовой библиотеки
// lua coroutines.create, за исключением того, что 
// запоминает созданную coroutine.
int scriptApi::NewThread(lua_State* L)
{
	::NewThread(L);

	return 1;
}



// Функция приостанавливает coroutine, из которой вызвана. Аналогична 
// команде базовой библиотеки lua coroutines.yield, но требует первый 
// аргумент - число милисекунд, на которое coroutine будет приостановлена.
int scriptApi::Wait(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1,
		"Number expected");

	if (lua_pushthread(L))
	{
		// lua_pushthread вернет true, если мы пытаемся остановить основной поток
		luaL_where(L, 0);
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_SCRIPT_EV, "%sПопытка приостновить основной поток.", lua_tostring(L, -1));
		lua_pop(L, lua_gettop(L));
		return 0;
	}
	else
	{
		// lua_pushthread вернула false, мы не в основном потоке, убираем из стека не нужное значение
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

// Продолжает выполнение приостановленной coroutine, являющейся
// тредом, за котором следит игра.
int Resume(ThreadIter it)
{
	LuaThread* lt = *it;
	SCRIPT::GetFromRegistry(lt->refKey);
	UINT res = luaB_coresume(lt->masterState);

	if (lua_status(lt->lThread) != LUA_YIELD) 
	{ 
		if (lua_status(lt->lThread) > 1)		// Всякие ошибки
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

// Продолжает выполнение приостановленной coroutine.
// Аналогична команде coroutines.resume, однако работает 
// несколько дольше, так как сначала ищет среди созданных игрой тредов.
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

// Вызывается при настплении события таймера, вызывает продолжение работы треда
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