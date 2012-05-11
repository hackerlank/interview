#include "stdafx.h"
#include "config.h"
#include "osl_types.h"

#include "osl_lexems.h"
#include "osl_parse.h"

#include "osl_debug.h"
#include "osl.h"
#include "misc.h"

extern unsigned int OSL_CURRENT_LINE;

int StringToKey(string s)
{
	if(s == "vk_left")
		return VK_LEFT;
	else if(s == "vk_right")
		return VK_RIGHT;
	else if(s == "vk_down")
		return VK_DOWN;
	else if(s == "vk_up")
		return VK_UP;
	else if(s == "vk_control")
		return VK_CONTROL;
	else if(s == "vk_space")
		return VK_SPACE;
	else if(s == "vk_a")
		return 0x41;
	else if(s == "vk_b")
		return 0x42;
	else if(s == "vk_c")
		return 0x43;
	else if(s == "vk_d")
		return 0x44;
	else if(s == "vk_e")
		return 0x45;
	else if(s == "vk_f")
		return 0x46;
	else if(s == "vk_g")
		return 0x47;
	else if(s == "vk_h")
		return 0x48;
	else if(s == "vk_i")
		return 0x49;
	else if(s == "vk_j")
		return 0x4A;
	else if(s == "vk_k")
		return 0x4B;
	else if(s == "vk_l")
		return 0x4C;
	else if(s == "vk_m")
		return 0x4D;
	else if(s == "vk_n")
		return 0x4E;
	else if(s == "vk_o")
		return 0x4F;
	else if(s == "vk_p")
		return 0x50;
	else if(s == "vk_q")
		return 0x51;
	else if(s == "vk_r")
		return 0x52;
	else if(s == "vk_s")
		return 0x53;
	else if(s == "vk_t")
		return 0x54;
	else if(s == "vk_u")
		return 0x55;
	else if(s == "vk_v")
		return 0x56;
	else if(s == "vk_w")
		return 0x57;
	else if(s == "vk_x")
		return 0x58;
	else if(s == "vk_y")
		return 0x59;
	else if(s == "vk_z")
		return 0x5A;
	else
		return 0x00;
}

void SetConfigField(config* cfg, string field, string value)
{
	if(field == "scr_width")	cfg->scr_width	= atoi(value.c_str());
	if(field == "scr_height")	cfg->scr_height	= atoi(value.c_str());
	if(field == "bpp")			cfg->scr_bpp	= atoi(value.c_str());
	if(field == "fullscreen")	cfg->fullscreen	= atoi(value.c_str()) != 0;
	if(field == "vert_sync")	cfg->vert_sync	= atoi(value.c_str()) != 0;
	if(field == "debug")		cfg->debug		= atoi(value.c_str()) != 0;
	if(field == "force_start")  cfg->force_start = atoi(value.c_str()) != 0;

	if(field == "left")			cfg->left		= StringToKey(value);
	if(field == "right")		cfg->right		= StringToKey(value);
	if(field == "down")			cfg->down		= StringToKey(value);
	if(field == "jump")			cfg->jump		= StringToKey(value);
	if(field == "fire")			cfg->fire		= StringToKey(value);

	if(field == "bullet_speed")		cfg->bullet_speed = atoi(value.c_str());
	if(field == "bullet_life")		cfg->bullet_life = atoi(value.c_str());
	if(field == "hero_speed")		cfg->hero_speed = atoi(value.c_str());
	if(field == "hero_jump_height") cfg->hero_jump_height = atoi(value.c_str());
	if(field == "hero_reload")		cfg->hero_reload = atoi(value.c_str());

	if(field == "enemy_speed")		cfg->enemy_speed = atoi(value.c_str());

	if(field == "bullet_damage")	cfg->bullet_damage = atoi(value.c_str());



}

bool LoadConfig(char* file_name, config* cfg)
{
	char* str = NULL;
	vector<OSL_LEXEM> lxm;

	bool ok = false;
	int lxm_count = 0;

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Loading config file: " + string(file_name));

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


#ifdef _DEBUG_OSL
	osl_dDumpLexemStream(&lxm);
#endif

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Applying config...");

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
				WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadConfig() error: invalid line");
				//return false;
			}
			else
			{
				//if(!osl_IsEqualcolon(sqm[1].token) || !osl_IsIntConstant(sqm[2].token))
				//{
				//	WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadConfig() error: invalid line");
				//}
				//else
				//{
					#ifdef _DEBUG_OSL
					osl_dDumpLexemStream(&sqm);
					#endif
					SetConfigField(cfg, sqm[0].token, sqm[2].token);
					sqm.clear();
				//}
			}
		}
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Applying config OK");
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Config successfully loaded");
	
	return true;
}