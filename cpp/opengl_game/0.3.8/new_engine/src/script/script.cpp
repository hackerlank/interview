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
// �������������� ���������� �������
BOOL SCRIPT::InitScript()
{
	if (lua)
	{
		FreeScript();
	}

	lua = luaL_newstate();			// ������� ����������� ������ Lua
	if ( lua == NULL )
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "������ �������� Lua" );
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_ERROR_EV, "������ �������� Lua" );
		return 1;
	}
	sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_INFO_EV, "Lua ������.");

	luaL_openlibs ( lua );                              // open standart libraries
	
	scriptApi::RegisterAPI(lua);			// �������������� ���������, ����������� C-�������.

	return 0;
}

// ����������� ���������� �������
void SCRIPT::FreeScript()
{
	if (lua)
	{
		lua_close(lua);
		lua = NULL;
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_INFO_EV, "Lua ���������." );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ��������� ���� � ��������� ���� �� ������� �����
BOOL SCRIPT::LoadFile(const char* filename)
{
	if (!filename)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"������ ���������� �����. �������� ������ ������-��� �����." );
		return 1;
	}

	if (!lua)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"������ �������� ����� \"%s\". Lua �� �������.", filename);
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
	// ����: func
	return 0;
}

// ��������� ������ ������� � ��������� ���� �� ������� �����
BOOL SCRIPT::LoadString(const char* str)
{
	if (!str)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "������ �������� �������. �������� ������ ������." );
		return 1;
	}

	if (!lua)
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, 
			"������ �������� �������: %s. Lua �� �������.", str);
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

// ��������� ������� (����) � ����� �����
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
// ������� � ������ ��, ��� ����� �� ������� �����, � ���������� ������ �� ����
// ��� ���� � ������� ����� ��� ���������
int SCRIPT::AddToRegistry()
{
	// ����: ... data
	return luaL_ref(lua, LUA_REGISTRYINDEX);	// ����:
}

// ������� �� ������� ���������� �� ������
void SCRIPT::RemoveFromRegistry(int r)
{
	lua_unref(lua, r);
}

// �������� �� �������� ����� �������� �� �������
void SCRIPT::GetFromRegistry(int r)
{
	lua_rawgeti(lua, LUA_REGISTRYINDEX, r);
}

//////////////////////////////////////////////////////////////////////////

// ������� ���� � ��������� ���� �������
BOOL SCRIPT::ExecFile(const char* filename)
{	
	lua_pop(lua, lua_gettop(lua));
	return !LoadFile(filename) ? ExecChunk() : 1;
}

// ������� ���� � ��������� ������ �������
BOOL SCRIPT::ExecString(const char* str)
{
	return !LoadString(str) ? ExecChunk() : 1;
}

// ��������� ���� ������� � ������� ��� � ������
int SCRIPT::LoadFileToReg(const char* filename)
{
	return !LoadFile(filename) ? AddToRegistry() : 1;
}

// ��������� ������ ������� � ������� � � ������
int SCRIPT::LoadStringToReg(const char* str)
{
	return !LoadString(str) ? AddToRegistry() : 1;
}

// ����� ������� �� ������� � ���������� ��
BOOL SCRIPT::ExecChunkFromReg(int r)
{
	GetFromRegistry(r);
	return ExecChunk();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ��������� � ���� ������� � ����� �� ��� �������� ���������� � �������� ������.
// ���������� ���� � ��������� ���������.
void SCRIPT::GetUIntFieldByName(lua_State* L, const char* table, const char* name, UINT* val)
{
	// ����: ...
	lua_getglobal(L, table);		// ����: ..., table
	if (lua_istable(L, -1))
	{
		lua_getfield(L, -1, name);	// ����: ..., table, name
		if (lua_isnumber(L, -1))
		{
			*val = (UINT)lua_tonumber(L, -1);
		}
		lua_pop(L, 1);		// ����: ..., table
	}
	lua_pop(L, 1); // ����: ...
}

// ����� �� ������� �� ������� ����� �������� ���������� � �������� ������.
// ���������� ���� � �������� ���������.
void SCRIPT::GetIntFieldByName(lua_State* L, const char* name, int* val)
{
	// ����: ..., table
	lua_getfield(L, -1, name);	// ����: ..., table, name
	if (lua_isnumber(L, -1))
	{
		*val = lua_tointeger(L, -1);
	}
	lua_pop(L, 1);		// ����: ..., table
}

// ����� �� ������� �� ������� ����� �������� ���������� � �������� ������.
// ���������� ���� � �������� ���������.
void SCRIPT::GetUIntFieldByName(lua_State* L, const char* name, UINT* val)
{
	// ����: ..., table
	lua_getfield(L, -1, name);	// ����: ..., table, name
	if (lua_isnumber(L, -1))
	{
		*val = (UINT)lua_tonumber(L, -1);
	}
	lua_pop(L, 1);		// ����: ..., table
}

// ����� �� ������� �� ������� ����� �������� ���������� � �������� ������.
// ���������� ���� � �������� ���������.
void SCRIPT::GetFloatFieldByName(lua_State* L, const char* name, float* val)
{
	// ����: ..., table
	lua_getfield(L, -1, name);	// ����: ..., table, name
	if (lua_isnumber(L, -1))
	{
		*val = (float)lua_tonumber(L, -1);
	}
	lua_pop(L, 1);		// ����: ..., table
}

// ����� �� ������� �� ������� ����� �������� ���������� � �������� ������.
// ���������� ���� � �������� ���������.
void SCRIPT::GetStringFieldByName(lua_State* L, const char* name, char** val)
{
	// ����: ..., table
	lua_getfield(L, -1, name);	// ����: ..., table, name
	if (lua_isstring(L, -1))
	{
		const char* str = lua_tostring(L, -1);
		*val = new char[strlen(str) + 1];
		strcpy(*val, str);
	}
	lua_pop(L, 1);		// ����: ..., table

}

//////////////////////////////////////////////////////////////////////////

// ������� � ��� ���� ����������� �����. ������������ ��� ������� �������.
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
