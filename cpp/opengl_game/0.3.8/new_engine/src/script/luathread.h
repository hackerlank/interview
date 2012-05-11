#ifndef __LUATHREAD_H_
#define __LUATHREAD_H_


class LuaThread
{
public:
	LuaThread(void)
	{
		lThread = NULL;
		masterState = NULL;
		refKey = LUA_NOREF;
	}

	lua_State* masterState;			// 
	lua_State* lThread;				// Coroutine

	int refKey;
};

void ProcessThread(int r);
UINT NewThread(lua_State* L);

#endif // __LUATHREAD_H_