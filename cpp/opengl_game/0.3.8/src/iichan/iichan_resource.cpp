#include "stdafx.h"
#include "../osl/osl_types.h"
#include "../osl/osl_lexems.h"
#include "../osl/osl_parse.h"
#include "../osl/osl.h"
#include "../osl/osl_misc.h"
#include "../osl/osl_debug.h"
#include "../misc.h"
#include "iichan_resource.h"

vector<RESOURCE> resources;

extern unsigned int OSL_CURRENT_LINE;

void SetResourceField(RESOURCE* res, string field, string value)
{
	if(field == "name") res->name = osl_RemoveQuotesStr(value);
	else if(field == "behaviour") res->behaviour = RES_BEH(atoi(value.c_str()));
	else if(field == "enemy_type") res->enemy_type = atoi(value.c_str());
	else if(field == "enemy_squashable") res->enemy_squashable = (atoi(value.c_str()) != 0);
	else if(field == "enemy_attack_collide") res->enemy_attack_collide = (atoi(value.c_str()) != 0);
	
	else if(field == "attack_range") res->attack_range = atoi(value.c_str()); 
	else if(field == "reload_speed") res->reload_speed = atoi(value.c_str());
	else if(field == "damage") res->damage = atoi(value.c_str());

	else if(field == "texture") res->texture = osl_RemoveQuotesStr(value);
	else if(field == "shadow_texture") res->shadow_texture = osl_RemoveQuotesStr(value);

	else if(field == "real_x") res->real_x = atoi(value.c_str());
	else if(field == "real_y") res->real_y = atoi(value.c_str());
	else if(field == "real_mirror_x") res->real_mirror_x = atoi(value.c_str());
	else if(field == "real_mirror_y") res->real_mirror_y = atoi(value.c_str());
	else if(field == "real_width") res->real_width = atoi(value.c_str());
	else if(field == "real_height") res->real_height = atoi(value.c_str());

	else if(field == "pattern_width") res->pattern_width = atoi(value.c_str());
	else if(field == "pattern_height") res->pattern_height = atoi(value.c_str());
	else if(field == "real_pattern_count") res->real_pattern_count = atoi(value.c_str());
	else if(field == "image_width") res->image_width = atoi(value.c_str());
	else if(field == "image_height") res->image_height = atoi(value.c_str());

	else if(field == "shoot_pattern") res->shoot_pattern = atoi(value.c_str());
	else if(field == "shoot_count") res->shoot_count = atoi(value.c_str());
	else if(field == "shoot_speed") res->shoot_speed = atoi(value.c_str());
	else if(field == "shoot_attack_pattern") res->shoot_attack_pattern = atoi(value.c_str());
	else if(field == "bullet_type") res->bullet_type = osl_RemoveQuotesStr(value);
	else if(field == "shoot_sound") res->shoot_sound = osl_RemoveQuotesStr(value);

	else if(field == "walk_pattern") res->walk_pattern = atoi(value.c_str());
	else if(field == "walk_count") res->walk_count = atoi(value.c_str());
	else if(field == "walk_speed") res->walk_speed = atoi(value.c_str());

	else if(field == "jump_pattern") res->jump_pattern = atoi(value.c_str());
	else if(field == "jump_count") res->jump_count = atoi(value.c_str());
	else if(field == "jump_speed") res->jump_speed = atoi(value.c_str());

	else if(field == "idle_pattern") res->idle_pattern = atoi(value.c_str());
	else if(field == "idle_count") res->idle_count = atoi(value.c_str());
	else if(field == "idle_speed") res->idle_speed = atoi(value.c_str());

	else if(field == "land_pattern") res->land_pattern = atoi(value.c_str());
	else if(field == "land_count") res->land_count = atoi(value.c_str());
	else if(field == "land_speed") res->land_speed = atoi(value.c_str());

	else if(field == "stand_pattern") res->stand_pattern = atoi(value.c_str());
	else if(field == "stand_count") res->stand_count = atoi(value.c_str());
	else if(field == "stand_speed") res->stand_speed = atoi(value.c_str());

	else if(field == "damage_pattern") res->damage_pattern = atoi(value.c_str());
	else if(field == "damage_count") res->damage_count = atoi(value.c_str());
	else if(field == "damage_speed") res->damage_speed = atoi(value.c_str());

	else if(field == "die_pattern") res->die_pattern = atoi(value.c_str());
	else if(field == "die_count") res->die_count = atoi(value.c_str());
	else if(field == "die_speed") res->die_speed = atoi(value.c_str());

	else if(field == "jump_down_pattern") res->jump_down_pattern = atoi(value.c_str());

	else if(field == "pattern_count") res->pattern_count = atoi(value.c_str());
	else if(field == "animation_speed") res->animation_speed = atoi(value.c_str());
	else if(field == "start_pattern") res->start_pattern = atoi(value.c_str());
	else if(field == "pattern_x") res->pattern_x = atoi(value.c_str());
	else if(field == "pattern_y") res->pattern_y = atoi(value.c_str());

	else if(field == "shadow_x") res->shadow_x = atoi(value.c_str());
	else if(field == "shadow_y") res->shadow_y = atoi(value.c_str());
	else if(field == "shadow_mirror_x") res->shadow_mirror_x = atoi(value.c_str());
	else if(field == "shadow_mirror_y") res->shadow_mirror_y = atoi(value.c_str());

	else if(field == "blend") res->blend = (float)atof(value.c_str());
	else if(field == "z") res->z = atoi(value.c_str());
	//else if(field == "z") res->z = Z_ORDER(atoi(value.c_str()));
	else if(field == "current_pattern") res->current_pattern = atoi(value.c_str());

	else if(field == "sit_pattern") res->sit_pattern = atoi(value.c_str());
	else if(field == "sit_count") res->sit_count = atoi(value.c_str());
	else if(field == "sit_speed") res->sit_speed = atoi(value.c_str());

	else if(field == "sit_shoot_pattern") res->sit_shoot_pattern = atoi(value.c_str());
	else if(field == "sit_shoot_count") res->sit_shoot_count = atoi(value.c_str());
	else if(field == "sit_shoot_speed") res->sit_shoot_speed = atoi(value.c_str());

	else if(field == "flash_jump_x") res->flash_jump_x = atoi(value.c_str());
	else if(field == "flash_jump_y") res->flash_jump_y = atoi(value.c_str());
	else if(field == "flash_sit_x") res->flash_sit_x = atoi(value.c_str());
	else if(field == "flash_sit_y") res->flash_sit_y = atoi(value.c_str());
	else if(field == "flash_stand_x") res->flash_stand_x = atoi(value.c_str());
	else if(field == "flash_stand_y") res->flash_stand_y = atoi(value.c_str());
	else if(field == "flash_walk_x") res->flash_walk_x = atoi(value.c_str());
	else if(field == "flash_walk_y") res->flash_walk_y = atoi(value.c_str());
	else if(field == "flash_jump_mirror_x") res->flash_jump_mirror_x = atoi(value.c_str());
	else if(field == "flash_jump_mirror_y") res->flash_jump_mirror_y = atoi(value.c_str());
	else if(field == "flash_sit_mirror_x") res->flash_sit_mirror_x = atoi(value.c_str());
	else if(field == "flash_sit_mirror_y") res->flash_sit_mirror_y = atoi(value.c_str());
	else if(field == "flash_stand_mirror_x") res->flash_stand_mirror_x = atoi(value.c_str());
	else if(field == "flash_stand_mirror_y") res->flash_stand_mirror_y = atoi(value.c_str());
	else if(field == "flash_walk_mirror_x") res->flash_walk_mirror_x = atoi(value.c_str());
	else if(field == "flash_walk_mirror_y") res->flash_walk_mirror_y = atoi(value.c_str());

	else if(field == "speed_x") res->speed_x = (float)atof(value.c_str());						// устаревшее
	else if(field == "speed_y") res->speed_y = (float)atof(value.c_str());						// устаревшее

	else if(field == "phys_walk_vel") res->phys_walk_vel = (float)atof(value.c_str());
	else if(field == "phys_jump_speed") res->phys_jump_speed = (float)atof(value.c_str());
	else if(field == "phys_max_x_speed") res->phys_max_x_speed = (float)atof(value.c_str());
	else if(field == "phys_max_y_speed") res->phys_max_y_speed = (float)atof(value.c_str());
	else if(field == "phys_collidable") res->phys_collidable = atoi(value.c_str());
	else if(field == "bullet_collidable") res->bullet_collidable = atoi(value.c_str());

	else if(field == "real_shoot_x") res->real_shoot_x = atoi(value.c_str());
	else if(field == "real_shoot_y") res->real_shoot_y = atoi(value.c_str());
	else if(field == "real_walk_x") res->real_walk_x = atoi(value.c_str());
	else if(field == "real_walk_y") res->real_walk_y = atoi(value.c_str());
	else if(field == "real_sit_x") res->real_sit_x = atoi(value.c_str());
	else if(field == "real_sit_y") res->real_sit_y = atoi(value.c_str());
	else if(field == "real_walk_x") res->real_walk_x = atoi(value.c_str());
	else if(field == "real_walk_y") res->real_walk_y = atoi(value.c_str());
	else if(field == "real_stand_x") res->real_stand_x = atoi(value.c_str());
	else if(field == "real_stand_y") res->real_stand_y = atoi(value.c_str());
	else if(field == "real_jump_x") res->real_jump_x = atoi(value.c_str());
	else if(field == "real_jump_y") res->real_jump_y = atoi(value.c_str());

	else if(field == "real_shoot_mirror_x") res->real_shoot_mirror_x = atoi(value.c_str());
	else if(field == "real_shoot_mirror_y") res->real_shoot_mirror_y = atoi(value.c_str());
	else if(field == "real_sit_mirror_x") res->real_sit_mirror_x = atoi(value.c_str());
	else if(field == "real_sit_mirror_y") res->real_sit_mirror_y = atoi(value.c_str());
	else if(field == "real_walk_mirror_x") res->real_walk_mirror_x = atoi(value.c_str());
	else if(field == "real_walk_mirror_y") res->real_walk_mirror_y = atoi(value.c_str());
	else if(field == "real_stand_mirror_x") res->real_stand_mirror_x = atoi(value.c_str());
	else if(field == "real_stand_mirror_y") res->real_stand_mirror_y = atoi(value.c_str());
	else if(field == "real_jump_mirror_x") res->real_jump_mirror_x = atoi(value.c_str());
	else if(field == "real_jump_mirror_y") res->real_jump_mirror_y = atoi(value.c_str());

	else if(field == "real_shoot_width") res->real_shoot_width = atoi(value.c_str());
	else if(field == "real_shoot_height") res->real_shoot_height = atoi(value.c_str());
	else if(field == "real_stand_width") res->real_stand_width = atoi(value.c_str());
	else if(field == "real_stand_height") res->real_stand_height = atoi(value.c_str());
	else if(field == "real_sit_width") res->real_sit_width = atoi(value.c_str());
	else if(field == "real_sit_height") res->real_sit_height = atoi(value.c_str());
	else if(field == "real_walk_width") res->real_walk_width = atoi(value.c_str());
	else if(field == "real_walk_height") res->real_walk_height = atoi(value.c_str());
	else if(field == "real_jump_width") res->real_jump_width = atoi(value.c_str());
	else if(field == "real_jump_height") res->real_jump_height = atoi(value.c_str());

	//else if(field == "collidable") res->collidable = atoi(value.c_str());
	else
		WriteToLog("SetResourceField(): unknown field");
}


bool LoadResource(const char* file_name)
{
	char* str = NULL;
	vector<OSL_LEXEM> lxm;

	bool ok = false;
	int lxm_count = 0;

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Loading resource file: " + string(file_name));
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Input...");
#endif
	if(osl_InputFile(file_name, &str))
		ok = osl_InputString(str, &str);

	if(ok)
	{
#ifdef _DEBUG_OSL
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input OK");
#endif
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input FAILED");
		return false;
	}

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize...");
#endif
	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

	if(ok)
	{
#ifdef _DEBUG_OSL
		char buffer[64];
		sprintf(buffer, "Lexemize OK (%u lexems token)", lxm_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
#endif
		delete[] str;
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize FAILED");
		return false;
	}
	vector <OSL_VAR> pool;
	extern OSL_TEMPLATE_STORAGE storage;
	ok = osl_Compile(&lxm, &pool, &storage);
	if (!ok)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Compile FAILED");
		return false;
	}

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Making resource...");
#endif
	RESOURCE res;
	ZeroMemory(&res, sizeof(RESOURCE));

	vector<OSL_LEXEM> sqm;
	for(vector<OSL_LEXEM>::iterator it = lxm.begin();
		it != lxm.end();
		it++)
	{
		if(!osl_IsSys((*it).token))
			sqm.push_back((*it));
		else
			OSL_CURRENT_LINE++;

		if(osl_IsSemicolon((*it).token))
		{
			if(sqm.size() != 4)
			{
				WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadResource() error: invalid line");
			}
			else
			{
				SetResourceField(&res, sqm[0].token, sqm[2].token);
				sqm.clear();
			}
		}
	}

	resources.push_back(res);
#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Resource successfully loaded");
#endif
	return true;
}


RESOURCE* ResourceByName(const char* name)
{
	for(vector<RESOURCE>::iterator it = resources.begin();
		it != resources.end();
		it++)
	{
		RESOURCE* t = &(*it);

		if(strcmp(t->name.c_str(), name) == 0)
			return t;
	}
	return NULL;
}
