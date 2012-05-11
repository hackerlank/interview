#include "stdafx.h"
#include "../osl/osl_types.h"
#include "../osl/osl_lexems.h"
#include "../osl/osl_parse.h"
#include "../osl/osl.h"
#include "../osl/osl_misc.h"
#include "../misc.h"
#include "iichan_resource.h"

vector<OBJECT_RES> resources;

extern unsigned int OSL_CURRENT_LINE;

void SetResourceField(OBJECT_RES* res, string field, string value)
{
	if(field == "name")
	{
		string name = osl_RemoveQuotesStr(value);
		res->name = new char[name.length()];
		sprintf(res->name, "%s", name.c_str());
	}
	else if(field == "type") res->type = RES_TYPE(atoi(value.c_str()));
	else if(field == "texture")
	{
		string name = osl_RemoveQuotesStr(value);
		res->texture = new char[name.length()];
		sprintf(res->texture, "%s", name.c_str());
	}
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
	else if(field == "jump_down_pattern") res->jump_down_pattern = atoi(value.c_str());
	else if(field == "pattern_count") res->pattern_count = atoi(value.c_str());
	else if(field == "animation_speed") res->animation_speed = atoi(value.c_str());
	else if(field == "start_pattern") res->start_pattern = atoi(value.c_str());
	else if(field == "pattern_x") res->pattern_x = atoi(value.c_str());
	else if(field == "pattern_y") res->pattern_y = atoi(value.c_str());
	else if(field == "z") res->z = atoi(value.c_str());
	else if(field == "current_pattern") res->current_pattern = atoi(value.c_str());
}


bool LoadResource(const char* file_name)
{
	char* str = NULL;
	vector<OSL_LEXEM> lxm;

	bool ok = false;
	int lxm_count = 0;

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Loading resource file: " + string(file_name));

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Input...");
	ok = osl_Input(file_name, &str);

	if(ok)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input OK");
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input FAILED");
		return false;
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize...");
	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

	if(ok)
	{
		char buffer[50];

		sprintf(buffer, "Lexemize OK (%u lexems token)", lxm_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
		delete[] str;
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize FAILED");
		return false;
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Making resource...");

	OBJECT_RES res;
	memset(&res, '\0', sizeof(res));

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

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Resource successfully loaded");
	
	return true;
}


OBJECT_RES* ResourceByName(char* name)
{
	for(vector<OBJECT_RES>::iterator it = resources.begin();
		it != resources.end();
		it++)
	{
		OBJECT_RES* t = &(*it);

		if(strcmp(t->name, name) == 0)
			return t;
	}
	return NULL;
}