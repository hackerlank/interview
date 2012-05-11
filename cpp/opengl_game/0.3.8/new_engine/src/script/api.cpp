#include "StdAfx.h"

#include "../config.h"
#include "../scene.h"
#include "../misc.h"

#include "../render/texture.h"
#include "../render/font.h"
#include "../render/draw.h"

#include "../game/proto.h"

#include "../game/camera.h"
#include "../game/ribbon.h"

#include "api.h"
#include "timerevent.h"

//////////////////////////////////////////////////////////////////////////

// scene.cpp
extern config cfg;	

// misc.cpp
extern CHAR path_config[MAX_PATH];
extern CHAR path_textures[MAX_PATH];
extern CHAR path_protos[MAX_PATH];
extern CHAR path_levels[MAX_PATH];
extern CHAR path_scripts[MAX_PATH];



// camera.cpp
extern CameraFocusObjectPoint CAMERA_FOCUS_ON_OBJ_POS;

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void scriptApi::RegisterAPI(lua_State* L)
{
	// ������ �������� package.path, ����� ������� require ������ ���� � ����� ��������
	lua_getglobal(L, "package");		// ����: package
	char* new_path = new char[strlen(path_scripts) + 1+6];
	memset(new_path,'\0', strlen(path_scripts) + 1+6);
	sprintf(new_path, "%s\?.lua\0", path_scripts);
	lua_pushstring(L, new_path);		// ����: package new_path
	lua_setfield(L, -2, "path");		// ����: package
	lua_pop(L, 1);						// ����: 
	DELETEARRAY(new_path);

	// ����������� �-�������
	lua_register(L, "LoadConfig", &scriptApi::LoadConfig);
	lua_register(L, "LoadTexture", &scriptApi::LoadTexture);
	lua_register(L, "LoadFont", &scriptApi::LoadFont);
	lua_register(L, "LoadPrototype", &scriptApi::LoadPrototype);
	lua_register(L, "InitNewGame", &scriptApi::InitNewGame);
	lua_register(L, "AddTimerEvent", &scriptApi::AddTimerEvent);

	lua_register(L, "Log", &scriptApi::Log);

	lua_register(L, "SetCamAttachedObj", &scriptApi::SetCamAttachedObj);
	lua_register(L, "CamMoveToPos", &scriptApi::CamMoveToPos);
	lua_register(L, "SetCamFocusOnObjPos", &scriptApi::SetCamFocusOnObjPos);	
	lua_register(L, "SetCamObjOffset", &scriptApi::SetCamObjOffset);
	lua_register(L, "SetCamAttachedAxis", &scriptApi::SetCamAttachedAxis);

	lua_register(L, "CreateRibbon", &scriptApi::CreateRibbon);
	lua_register(L, "CreateRibbonZ", &scriptApi::CreateRibbon);
	lua_register(L, "CreateRibbonB", &scriptApi::CreateRibbon);
	lua_register(L, "CreateRibbonZB", &scriptApi::CreateRibbon);

	lua_register(L, "CreatePlayer", &scriptApi::CreatePlayer);
	lua_register(L, "CreatePlayerZ", &scriptApi::CreatePlayer);

	lua_register(L, "CreateSpatialTrigger", &scriptApi::CreateSpatialTrigger);

	lua_register(L, "CreateSprite", &scriptApi::CreateSprite);
	lua_register(L, "CreateSpriteZ", &scriptApi::CreateSprite);
	lua_register(L, "CreateSpriteF", &scriptApi::CreateSprite);
	lua_register(L, "CreateSpriteZF", &scriptApi::CreateSprite);

	lua_register(L, "SetObjAnim", &scriptApi::SetObjAnim);

	lua_register(L, "GetPlayer", &scriptApi::GetPlayer);
	lua_register(L, "GetObject", &scriptApi::GetObject);

	lua_register(L, "SetObjPos", &scriptApi::SetObjPos);
	lua_register(L, "SetDynObjAcc", &scriptApi::SetDynObjAcc);
	lua_register(L, "SetDynObjVel", &scriptApi::SetDynObjVel);
	lua_register(L, "SetDynObjJumpVel", &scriptApi::SetDynObjJumpVel);	
	lua_register(L, "EnablePlayerControl", &scriptApi::EnablePlayerControl);
	lua_register(L, "SetDynObjMovement", &scriptApi::SetDynObjMovement);
	lua_register(L, "SetDynObjMoveDirX", &scriptApi::SetDynObjMoveDirX);
	lua_register(L, "SetObjSpriteMirrored", &scriptApi::SetObjSpriteMirrored);
	lua_register(L, "SetObjAnim", &scriptApi::SetObjAnim);


	lua_register(L, "SetObjProcessor", &scriptApi::SetObjProcessor);

	lua_register(L, "SetOnChangePlayerProcessor", &scriptApi::SetOnChangePlayerProcessor);
	lua_register(L, "BlockPlayerChange", &scriptApi::BlockPlayerChange);
	lua_register(L, "UnblockPlayerChange", &scriptApi::UnblockPlayerChange);

	lua_register(L, "NewThread", &scriptApi::NewThread);
	lua_register(L, "Wait", &scriptApi::Wait);
	lua_register(L, "Resume", &scriptApi::Resume);


	// TODO: ���������������� ������ ��������� ������, ����� ���� �������� � ��������.
	// ����������� �����. �������-���� � �. �. ���� � ����� �� ������ �� ilovelua

	// ������������ ����������� �������� ��� ������.
	lua_newtable(L);			// ������� ����� �������. ����: �������
	//temp1 = lua_gettop(L);	
	char* ch = new char[20];	// ����� ��� ��������
	memset(ch, '\0', 20);

	for (int i = 0; i < 255; i++)		// ���� �� ���� ��������� �����
	{
		if (GetVKeyByNum(i, ch))
		{
			lua_pushinteger(L,i);		// �������� � ���� ��� �������. ����: �������, i
			lua_setfield(L, -2, ch);	// ������� � ������� ��������� � ������ � ���������. ����: �������
		}
	}
	DELETEARRAY(ch);
	lua_setglobal(L, "keys");		// ������� ������� � ���������� ����������. ����:

	// ��������� ���������� ���������
	lua_newtable(L);		// ����: �������
	{
		lua_pushinteger(L, protoNullBeh);		lua_setfield(L, -2, "NullBehaviour");
		lua_pushinteger(L, protoPlayer);		lua_setfield(L, -2, "PlayerBehaviour");
		lua_pushinteger(L, protoEnemy);			lua_setfield(L, -2, "EnemyBehaviour");
		lua_pushinteger(L, protoPowerup);		lua_setfield(L, -2, "PowerUpBehaviour");
		lua_pushinteger(L, protoSprite);		lua_setfield(L, -2, "SpriteBehaviour");

		lua_pushinteger(L, CamFocusLeftBottomCorner);	lua_setfield(L, -2, "CamFocusLeftBottomCorner");
		lua_pushinteger(L, CamFocusLeftCenter);			lua_setfield(L, -2, "CamFocusLeftCenter");
		lua_pushinteger(L, CamFocusLeftTopCorner);		lua_setfield(L, -2, "CamFocusLeftTopCorner");
		lua_pushinteger(L, CamFocusRightBottomCorner);	lua_setfield(L, -2, "CamFocusRightBottomCorner");
		lua_pushinteger(L, CamFocusRightCenter);		lua_setfield(L, -2, "CamFocusRightCenter");
		lua_pushinteger(L, CamFocusRightTopCorner);		lua_setfield(L, -2, "CamFocusRightTopCorner");
		lua_pushinteger(L, CamFocusBottomCenter);		lua_setfield(L, -2, "CamFocusBottomCenter");
		lua_pushinteger(L, CamFocusCenter);				lua_setfield(L, -2, "CamFocusCenter");
		lua_pushinteger(L, CamFocusTopCenter);			lua_setfield(L, -2, "CamFocusTopCenter");
	}
	lua_setglobal(L, "constants");		// ������� ������� � ���������� ����������. ����:

}

//////////////////////////////////////////////////////////////////////////

int scriptApi::LoadConfig(lua_State* L)
{
	// ��������� ������ ��� ����� (����+��� �����)
	char* config_file = new char[strlen(path_config) + strlen(DEFAULT_CONFIG_NAME) + 1];
	memset(config_file, '\0', strlen(path_config) + strlen(DEFAULT_CONFIG_NAME) + 1);
	sprintf(config_file, "%s%s\0", path_config, DEFAULT_CONFIG_NAME);
	SCRIPT::ExecFile(config_file);
	DELETEARRAY(config_file);

	lua_getglobal(L, "config");		// ����: config

	if (lua_istable(L, -1))
	{
		SCRIPT::GetUIntFieldByName(L, "scr_width", &cfg.scr_width );
		SCRIPT::GetUIntFieldByName(L, "scr_height", &cfg.scr_height );
		SCRIPT::GetUIntFieldByName(L, "bpp", &cfg.scr_bpp );

		SCRIPT::GetFloatFieldByName(L, "near_z", &cfg.near_z);
		SCRIPT::GetFloatFieldByName(L, "far_z", &cfg.far_z);

		SCRIPT::GetUIntFieldByName(L, "window_width", &cfg.window_width );
		SCRIPT::GetUIntFieldByName(L, "window_height", &cfg.window_height );

		SCRIPT::GetUIntFieldByName(L, "fullscreen", (UINT*)&cfg.fullscreen );
		SCRIPT::GetUIntFieldByName(L, "vert_sync", (UINT*)&cfg.vert_sync );
		SCRIPT::GetUIntFieldByName(L, "debug", (UINT*)&cfg.debug );
		SCRIPT::GetUIntFieldByName(L, "show_fps", (UINT*)&cfg.show_fps );

		SCRIPT::GetUIntFieldByName(L, "left", &cfg.left );
		SCRIPT::GetUIntFieldByName(L, "right", &cfg.right );
		SCRIPT::GetUIntFieldByName(L, "down", &cfg.down );
		SCRIPT::GetUIntFieldByName(L, "up", &cfg.up );
		SCRIPT::GetUIntFieldByName(L, "jump", &cfg.jump );
		SCRIPT::GetUIntFieldByName(L, "sit", &cfg.sit );
		SCRIPT::GetUIntFieldByName(L, "fire", &cfg.fire );
		SCRIPT::GetUIntFieldByName(L, "change_weapon", &cfg.change_weapon );
		SCRIPT::GetUIntFieldByName(L, "change_player", &cfg.change_player );
		SCRIPT::GetUIntFieldByName(L, "screenshot", &cfg.screenshot );
		SCRIPT::GetUIntFieldByName(L, "show_menu", &cfg.show_menu );
	}
	lua_pop(L, 1); // ����:


	scene::ApplyConfig(&cfg);
	return 0;
}


int scriptApi::LoadTexture (lua_State* L)
{
	luaL_argcheck(L, lua_isstring(L, 1), 1, "��������� ��� ��������");

	const char* file_name = lua_tostring(L, 1);			
	char* tex_file = new char[strlen(path_textures) + strlen(file_name) + 1];
	memset(tex_file, '\0', strlen(path_textures) + strlen(file_name) + 1);
	sprintf(tex_file, "%s%s\0", path_textures, file_name);

	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "��������� ��������: %s", file_name);

	if (!::LoadTexture(tex_file, NULL))
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "������ �������� ��������.");

	DELETEARRAY(tex_file);

	return 0;
}

int scriptApi::LoadFont (lua_State* L)
{
	luaL_argcheck(L, lua_isstring(L, 1), 1, "��������� ��� ������");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "��������� ������ ������");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "��������� ������ ������");
	luaL_argcheck(L, lua_isstring(L, 4), 4, "��������� ��� ������ ��� ����");

	// ����: font_name height weight out_name
	const char* font_name = lua_tostring(L, 1);			
	UINT height = (UINT)lua_tointeger(L, 2);
	UINT weight = (UINT)lua_tointeger(L, 3);
	const char* out_name = lua_tostring(L, 4); 

	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "��������� �����: %s", font_name);

	if (!LoadWindowsFont(font_name, height, weight, out_name))
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "������ �������� ������.");
	}


	return 0;
}


// ������� ��������� �������� ������� �� �����
int scriptApi::LoadPrototype(lua_State* L)
{
	luaL_argcheck(L, lua_isstring(L, 1), 1, "��������� ��� ���������");

	// ����: file_name
	const char* file_name = lua_tostring(L, 1);			
	char* proto_file = new char[strlen(path_protos) + strlen(file_name) + 1];
	memset(proto_file, '\0', strlen(path_protos) + strlen(file_name) + 1);
	sprintf(proto_file, "%s%s\0", path_protos, file_name);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "��������� ��������: %s", file_name);

	// ���� �������� ����� �������� � ���������� ���������, ����� �� �������� ���-���� ����������.
	// ��������� ��������� ��� �����: http://community.livejournal.com/ru_lua/402.html
	lua_newtable(L);				// ����: file_name env
	lua_newtable(L);				// ����: file_name env meta
	lua_getglobal(L, "_G");			// ����: file_name env meta _G
	lua_setfield(L, -2, "__index");	// ����: file_name env meta
	lua_setmetatable(L, -2);		// ����: file_name env
	if(luaL_loadfile(L, proto_file))
	{
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, "%s", lua_tostring(L, -1));
		return 0;
	}
	// ����: file_name env loadfile
	lua_pushvalue(L, -2);			// ����: file_name env loadfile env
	lua_setfenv(L, -2);				// ����: file_name env loadfile

	if(lua_pcall(L, 0, 0, 0))
	{
		// �����-�� ������ ���������� �����
		const char* err = lua_tostring(L, -1);	// ����: file_name env err
		sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_WARNING_EV, err );
		lua_pop(L, 1);	// ����: file_name env
	}
	else
	{
		// ����: file_name env
		Proto* proto = new Proto;

		char* proto_name = NULL;
		SCRIPT::GetStringFieldByName(L, "name", &proto_name);
		// TODO: �������� proto_name

		SCRIPT::GetUIntFieldByName(L, "behaviour", (UINT*)&proto->behaviour);
		SCRIPT::GetStringFieldByName(L, "texture", &proto->texture);
		SCRIPT::GetFloatFieldByName(L, "z", &proto->z);
		SCRIPT::GetUIntFieldByName(L, "image_width", &proto->image_width);
		SCRIPT::GetUIntFieldByName(L, "image_height", &proto->image_height);
		SCRIPT::GetUIntFieldByName(L, "frame_width", &proto->frame_widht);
		SCRIPT::GetUIntFieldByName(L, "frame_height", &proto->frame_height);
		SCRIPT::GetUIntFieldByName(L, "frames_count", &proto->frames_count);
		SCRIPT::GetUIntFieldByName(L, "frame_x", &proto->frame_x);
		SCRIPT::GetUIntFieldByName(L, "frame_y", &proto->frame_y);
		SCRIPT::GetFloatFieldByName(L, "phys_max_x_vel", &proto->phys_max_x_vel);
		SCRIPT::GetFloatFieldByName(L, "phys_max_y_vel", &proto->phys_max_y_vel);
		SCRIPT::GetFloatFieldByName(L, "phys_jump_vel", &proto->phys_jump_vel);
		SCRIPT::GetFloatFieldByName(L, "phys_walk_acc", &proto->phys_walk_acc);
		SCRIPT::GetUIntFieldByName(L, "physic", &proto->physic);
		SCRIPT::GetUIntFieldByName(L, "phys_solid", &proto->phys_solid);
		SCRIPT::GetUIntFieldByName(L, "phys_bullet_collidable", &proto->phys_bullet_collidable);
		SCRIPT::GetIntFieldByName(L, "bullets_count", &proto->bullets_count);
		SCRIPT::GetUIntFieldByName(L, "reload_time", &proto->reload_time);
		SCRIPT::GetUIntFieldByName(L, "bullet_damage", &proto->bullet_damage);
		SCRIPT::GetFloatFieldByName(L, "bullet_vel", &proto->bullet_vel);
		SCRIPT::GetStringFieldByName(L, "main_weapon", &proto->main_weapon);
		SCRIPT::GetStringFieldByName(L, "alt_weapon", &proto->alt_weapon);
		SCRIPT::GetUIntFieldByName(L, "health", &proto->health);

		// �������� ������� ���������� ��������
		lua_getfield(L, -1, "animations");	// ����: file_name env animations
		if (lua_istable(L, -1))
		{
			const UINT count = lua_objlen(L, -1);
			// ���� �� ������� ��������
			// ���� � ������� ���� ���-��, �������� �� ������, ����� ���������� ������ ����� �� �����������
			for (UINT i = 1; i <= count; i++)
			{
				// �������� � ����� i-��� ������� �������
				lua_pushnumber(L, i);	// ����: file_name env animations i
				lua_gettable(L, -2);	// ����: file_name env animations animations[i]
				if (lua_istable(L, -1))
				{
					// �������� ������� ��������� ��������, ������ ��
					AnimProto* ap = new AnimProto;
					memset(ap, '\0', sizeof(AnimProto));

					SCRIPT::GetStringFieldByName(L, "name", &ap->anim_name);
					SCRIPT::GetUIntFieldByName(L, "frames_count", &ap->frame_count);
					SCRIPT::GetUIntFieldByName(L, "start_frame", &ap->start_frame);
					SCRIPT::GetUIntFieldByName(L, "anim_speed", &ap->anim_speed);
					SCRIPT::GetUIntFieldByName(L, "real_x", &ap->real_x);
					SCRIPT::GetUIntFieldByName(L, "real_y", &ap->real_y);
					SCRIPT::GetUIntFieldByName(L, "real_mirror_x", &ap->real_mirror_x);
					SCRIPT::GetUIntFieldByName(L, "real_mirror_y", &ap->real_mirror_y);
					SCRIPT::GetUIntFieldByName(L, "real_width", &ap->real_width);
					SCRIPT::GetUIntFieldByName(L, "real_height", &ap->real_height);

					proto->animations.push_back(ap);
				}
				lua_pop(L, 1);	// ����: file_name env animations
			}

		} // if (lua_istable(L, -1))

		// �������� ������� ��������, ��������� ��� � map ����������
		AddProto(proto_name, proto);
		DELETEARRAY(proto_name);
	}

	DELETEARRAY(proto_file);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int scriptApi::InitNewGame(lua_State* L)
{
	game::InitGame();
	lua_pop(L, lua_gettop(L));
	return 0;
}

////////////////////////////////////

int scriptApi::AddTimerEvent(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "��������� �������� �������");
	luaL_argcheck(L, lua_isfunction(L, 2), 2, "��������� �������");

	//SCRIPT::StackDumpToLog(L);

	UINT dt = (UINT)lua_tointeger(L, 1);
	UINT period = 0;
	UINT maxCalls = 0;

	int num = lua_gettop ( L );	// ����� ���������� ��������� � ����� (���������� ���������� ����������)
	if (num == 4)
	{
		luaL_argcheck(L, lua_isnumber(L, 3), 3, "��������� ������");
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "��������� ���������� ����������");

		period = (UINT)lua_tointeger(L, 3);
		maxCalls = (UINT)lua_tointeger(L, 4);
	}

	
	lua_settop(L, 2);	// ����� ������� ������� �������� �� ������� �����
	//SCRIPT::StackDumpToLog(L);
	int action = SCRIPT::AddToRegistry();
	
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "������� ������� �������.");

	int result = 0;
	if (num == 2)
		result = ::AddTimerEvent(dt, action);
	else 
		result = ::AddTimerEvent(dt, action, period, maxCalls);

	if (!result)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "������ �������� ������� �������.");
	}

	return 0;
}


///////////////////////////////////////

// ������ � ���. �������� ��� print
int scriptApi::Log(lua_State* L)
{
	int num = lua_gettop ( L );	// ����� ���������� ��������� � ����� (���������� ���������� ����������)

	string buf(""); 
	for (int i = -num; i < 0; i++)
	{
		if(lua_isstring(L, i))
			buf += lua_tostring(L, i);
		else if (lua_isboolean(L, i))
			buf += lua_toboolean(L, i) ? "true" : "false";
		else if (lua_isnil(L, i))		
			buf += "nil";

	}

	sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_SCRIPT_EV, buf.c_str());

	lua_pop(L, num);	// ����:
	return 0;	
}

//////////////////////////////////////////////////////////////////////////

// ��������� ������ �� �������. ������ �������� �� ��� id
int scriptApi::SetCamAttachedObj(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Object id expected");

	UINT id = lua_tointeger(L, 1);

	GameObject* obj = NULL;
	if (id != 0)
	{
		obj = GetGameObject(id);
	}

	CameraAttachToObject(obj);

	return 0;
}

// ������ ����� ��������� ������ �� �������� ����, ����� �� ��������
int scriptApi::SetCamAttachedAxis(lua_State* L)
{
	luaL_argcheck(L, lua_isboolean(L, 1), 1, "Boolean expected");
	luaL_argcheck(L, lua_isboolean(L, 2), 1, "Boolean expected");

	CameraAttachToAxis(lua_toboolean(L, 1), lua_toboolean(L, 2));

	return 0;
}

// ���������� ������ �� �����, �������� ������������
int scriptApi::CamMoveToPos(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Number expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 1, "Number expected");

	CameraMoveToPos(lua_tonumber(L, 1), lua_tonumber(L, 2));

	return 0;
}


// ������ �������� ������ ������������ �������
int scriptApi::SetCamObjOffset(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Number expected");
	luaL_argcheck(L, lua_isnumber(L, 2), 1, "Number expected");

	SetCameraAttachedObjectOffset(lua_tonumber(L, 1), lua_tonumber(L, 2));

	return 0;
}


// ������ ���� �������, �� ������ ������������ ������
int scriptApi::SetCamFocusOnObjPos(lua_State* L)
{
	luaL_argcheck(L, lua_isnumber(L, 1), 1, "Number expected");

	CAMERA_FOCUS_ON_OBJ_POS = (CameraFocusObjectPoint)lua_tointeger(L, 1);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int scriptApi::CreateRibbon(lua_State* L)
{
	CHECKGAME;

	luaL_argcheck(L, lua_isstring(L, 1), 1, "��������� ��� ���������");
	luaL_argcheck(L, lua_isnumber(L, 2), 2, "��������� ���������� X");
	luaL_argcheck(L, lua_isnumber(L, 3), 3, "��������� ���������� Y");
	

	int num = lua_gettop ( L );
	const char* proto_name = lua_tostring(L, 1);
	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);
	float fact = 0;

	bool overrride_z = false;
	float z = 0;

	float bx = 0;
	float by = 0;
	float bw = 0;
	float bh = 0;

	// ������� ����� ������� 8 ���������� - ������� + ��������� ������
	if (num == 5)
	{
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "��������� ���������� Z");
		luaL_argcheck(L, lua_isnumber(L, 5), 5, "��������� ����������� ��������");
		z = lua_tonumber(L, 4);	
		fact = lua_tonumber(L, 5);
		overrride_z = true;
	}
	else if (num == 8)
	{
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "��������� ����������� ��������");
		luaL_argcheck(L, lua_isnumber(L, 5), 5, "��������� ���������� X1 ������");
		luaL_argcheck(L, lua_isnumber(L, 6), 6, "��������� ���������� Y1 ������");
		luaL_argcheck(L, lua_isnumber(L, 7), 7, "��������� ���������� X2 ������");
		luaL_argcheck(L, lua_isnumber(L, 8), 8, "��������� ���������� Y2 ������");
		fact = lua_tonumber(L, 4);
		bx = lua_tonumber(L, 5);
		by = lua_tonumber(L, 6);
		bw = lua_tonumber(L, 7);
		bh = lua_tonumber(L, 8);
	}
	else if (num == 9)
	{
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "��������� ���������� Z");
		luaL_argcheck(L, lua_isnumber(L, 5), 5, "��������� ����������� ��������");
		luaL_argcheck(L, lua_isnumber(L, 6), 6, "��������� ���������� X1 ������");
		luaL_argcheck(L, lua_isnumber(L, 7), 7, "��������� ���������� Y1 ������");
		luaL_argcheck(L, lua_isnumber(L, 8), 8, "��������� ���������� X2 ������");
		luaL_argcheck(L, lua_isnumber(L, 9), 9, "��������� ���������� Y2 ������");		
		z = lua_tonumber(L, 4);	
		fact = lua_tonumber(L, 5);
		bx = lua_tonumber(L, 6);
		by = lua_tonumber(L, 7);
		bw = lua_tonumber(L, 8);
		bh = lua_tonumber(L, 9);
		overrride_z = true;
	}
	else 
	{
		luaL_argcheck(L, lua_isnumber(L, 4), 4, "��������� ����������� ��������");
		fact = lua_tonumber(L, 4);	
	}

	lua_pop(L, num);	// ����: 

	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "C������ Ribbon: %s", proto_name);
	Ribbon* r = NULL;

	if (overrride_z)
		r = ::CreateRibbon(proto_name, Vector2(x, y), z, fact, bx, by, bw, bh);
	else
		r = ::CreateRibbon(proto_name, Vector2(x, y), fact, bx, by, bw, bh);

	if (r)
	{
		/*if (obj->id != 0)
		lua_pushnumber(L, obj->id);	// ����: obj->id*/

	}
	else
	{
		lua_pushnil(L);				// ����: nil
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "������ c������� �������");
	}

	return 1;
}