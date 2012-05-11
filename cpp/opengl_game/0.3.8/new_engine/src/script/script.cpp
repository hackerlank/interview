#include "StdAfx.h"


#include "../misc.h"

#include "script.h"
#include "api.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

lua_State* lua = NULL;

extern CHAR path_app[MAX_PATH];
extern CHAR path_scripts[MAX_PATH];

//////////////////////////////////////////////////////////////////////////
// Инициализирует скриптовую систему
BOOL SCRIPT::InitScript()
{
	if (lua)
	{
		FreeScript();
	}

	lua = luaL_newstate();			// Создаем виртуальную машину Lua
	if ( lua == NULL )
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Ошибка создания Lua" );
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_ERROR_EV, "Ошибка создания Lua" );
		return 1;
	}
	sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_INFO_EV, "Lua создан.");

	luaL_openlibs ( lua );                              // open standart libraries
	
	scriptApi::RegisterAPI(lua);			// Инициализируем константы, привязываем C-функции.

	return 0;
}

// Освобождает скриптовую систему
void SCRIPT::FreeScript()
{
	if (lua)
	{
		lua_close(lua);
		lua = NULL;
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_INFO_EV, "Lua уничтожен." );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Загружает файл и оставляет чанк на вершине стека
BOOL SCRIPT::LoadFile(const char* filename)
{
	if (!filename)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка исполнения файла. Передана пустая строка-имя файла." );
		return 1;
	}

	if (!lua)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка загрузки файла \"%s\". Lua не создана.", filename);
		return 1;
	}

	SetCurrentDirectory(path_scripts);

	if(luaL_loadfile(lua, filename))
	{
		const char* err = lua_tostring(lua, -1);
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "%s", err);
		return 1;
	}

	SetCurrentDirectory(path_app);
	// Стек: func
	return 0;
}

// Загружает строку скрипта и оставляет чанк на вершине стека
BOOL SCRIPT::LoadString(const char* str)
{
	if (!str)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "Ошибка загрузки команды. Передана пустая строка." );
		return 1;
	}

	if (!lua)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"Ошибка загрузки команды: %s. Lua не создана.", str);
		return 1;
	}	

	if(luaL_loadstring(lua, str))
	{
		const char* err = lua_tostring(lua, -1);
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "%s", err);
		return 1;
	}

	return 0;
}

// Выполняет функцию (чанк) в конце стека
BOOL SCRIPT::ExecChunk()
{
	if (lua_pcall(lua, 0, LUA_MULTRET, 0))
	{
		const char* err = lua_tostring(lua, -1);
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "%s", err);
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Заносит в реестр то, что лежит на вершине стека, и возвращает ссылку на него
// При этом с вершины стека оно удаляется
int SCRIPT::AddToRegistry()
{
	// Стек: ... data
	return luaL_ref(lua, LUA_REGISTRYINDEX);	// Стек:
}

// Удаляет из реестра содержимое по ссылке
void SCRIPT::RemoveFromRegistry(int r)
{
	lua_unref(lua, r);
}

// Помещает на верхушку стека значенее из реестра
void SCRIPT::GetFromRegistry(int r)
{
	lua_rawgeti(lua, LUA_REGISTRYINDEX, r);
}

//////////////////////////////////////////////////////////////////////////

// Очищает стек и исполняет файл скрипта
BOOL SCRIPT::ExecFile(const char* filename)
{	
	lua_pop(lua, lua_gettop(lua));
	return !LoadFile(filename) ? ExecChunk() : 1;
}

// Очищает стек и исполняет строку скрипта
BOOL SCRIPT::ExecString(const char* str)
{
	return !LoadString(str) ? ExecChunk() : 1;
}

// Загружает файл скрипта и заносит его в реестр
int SCRIPT::LoadFileToReg(const char* filename)
{
	return !LoadFile(filename) ? AddToRegistry() : 1;
}

// Загружает строку скрипта и заносит её в реестр
int SCRIPT::LoadStringToReg(const char* str)
{
	return !LoadString(str) ? AddToRegistry() : 1;
}

// Берет функцию из реестра и выполяняет ее
BOOL SCRIPT::ExecChunkFromReg(int r)
{
	GetFromRegistry(r);
	return ExecChunk();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Загружает в стек таблицу и берет из нее значение переменной с заданным именем.
// Возвращает стек в исххожное состояние.
void SCRIPT::GetUIntFieldByName(lua_State* L, const char* table, const char* name, UINT* val)
{
	// Стек: ...
	lua_getglobal(L, table);		// Стек: ..., table
	if (lua_istable(L, -1))
	{
		lua_getfield(L, -1, name);	// Стек: ..., table, name
		if (lua_isnumber(L, -1))
		{
			*val = (UINT)lua_tonumber(L, -1);
		}
		lua_pop(L, 1);		// Стек: ..., table
	}
	lua_pop(L, 1); // Стек: ...
}

// Берет из таблицы на вершине стека значение переменной с заданным именем.
// Возвращает стек в исходное состояние.
void SCRIPT::GetIntFieldByName(lua_State* L, const char* name, int* val)
{
	// Стек: ..., table
	lua_getfield(L, -1, name);	// Стек: ..., table, name
	if (lua_isnumber(L, -1))
	{
		*val = lua_tointeger(L, -1);
	}
	lua_pop(L, 1);		// Стек: ..., table
}

// Берет из таблицы на вершине стека значение переменной с заданным именем.
// Возвращает стек в исходное состояние.
void SCRIPT::GetUIntFieldByName(lua_State* L, const char* name, UINT* val)
{
	// Стек: ..., table
	lua_getfield(L, -1, name);	// Стек: ..., table, name
	if (lua_isnumber(L, -1))
	{
		*val = (UINT)lua_tonumber(L, -1);
	}
	lua_pop(L, 1);		// Стек: ..., table
}

// Берет из таблицы на вершине стека значение переменной с заданным именем.
// Возвращает стек в исходное состояние.
void SCRIPT::GetFloatFieldByName(lua_State* L, const char* name, float* val)
{
	// Стек: ..., table
	lua_getfield(L, -1, name);	// Стек: ..., table, name
	if (lua_isnumber(L, -1))
	{
		*val = (float)lua_tonumber(L, -1);
	}
	lua_pop(L, 1);		// Стек: ..., table
}

// Берет из таблицы на вершине стека значение переменной с заданным именем.
// Возвращает стек в исходное состояние.
void SCRIPT::GetStringFieldByName(lua_State* L, const char* name, char** val)
{
	// Стек: ..., table
	lua_getfield(L, -1, name);	// Стек: ..., table, name
	if (lua_isstring(L, -1))
	{
		const char* str = lua_tostring(L, -1);
		*val = new char[strlen(str) + 1];
		strcpy(*val, str);
	}
	lua_pop(L, 1);		// Стек: ..., table

}

//////////////////////////////////////////////////////////////////////////

// Выводит в лог дамп содержимого стека. Использовать для отладки функций.
void SCRIPT::StackDumpToLog(lua_State *L) 
{
	int i;
	int top = lua_gettop(L);
	string s("");
	for (i = 1; i <= top; i++) 
	{  /* repeat for each level */
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING: /* strings */
			s += string("'") + lua_tostring(L, i) + "'";
			break;

		case LUA_TBOOLEAN: /* booleans */
			s += lua_toboolean(L, i) ? "true" : "false";
			break;

		case LUA_TNUMBER: /* numbers */
			s += lua_tostring(L, i);
			break;

		default: /* other values */
			s += lua_typename(L, t);
			break;

		}
		s += " "; /* put a separator */
	}
	sLog(DEFAULT_SCRIPT_LOG_NAME, "DUMP", s.c_str());
}
