#include "stdafx.h"

#include "font.h"
#include "texture.h"
#include "misc.h"
#include "config.h"
#include "draw.h"
#include "gui.h"

#include "osl_types.h"
#include "osl_lexems.h"
#include "osl_misc.h"


#include "osl_makeiichan.h"

#include "iichan.h"


extern GUI* gui;
extern IICHAN* iichan;

void CreatePlayer(string p1, string p2, string p3, string p4,
				  string p5, string p6, string p7, string p8,
				  string p9, string p10, string p11, string p12,
				  string p13, string p14, string p15, string p16,
				  string p17, string p18, string p19, string p20,
				  string p21, string p22, string p23, string p24,
				  string p25, string p26, string p27, string p28)
{
	if(iichan)
	{
		if(iichan->GetPlayer())
		{
			osl_Error("Player already exist");
			return;
		}


		if(!(osl_IsIntConstant(p1) &&  osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) &&  osl_IsStrConstant(p4) &&  
			osl_IsIntConstant(p5) &&  osl_IsIntConstant(p6) &&  
			osl_IsIntConstant(p7) &&  osl_IsIntConstant(p8) &&  
			osl_IsIntConstant(p9) &&  osl_IsIntConstant(p10) &&  
			osl_IsIntConstant(p11) &&  osl_IsIntConstant(p12) &&  
			osl_IsIntConstant(p13) &&  osl_IsIntConstant(p14) &&  
			osl_IsIntConstant(p15) &&  osl_IsIntConstant(p16) &&
			osl_IsIntConstant(p17) &&  osl_IsIntConstant(p18) &&  
			osl_IsIntConstant(p19) &&  osl_IsIntConstant(p20) &&  
			osl_IsIntConstant(p21) &&  osl_IsIntConstant(p22) &&  
			osl_IsIntConstant(p23) &&  osl_IsIntConstant(p24) &&  
			osl_IsIntConstant(p25) && osl_IsIntConstant(p26) &&
			osl_IsIntConstant(p27) && osl_IsIntConstant(p28)))
		{
			osl_Error("CreatePlayer(): incompatible parametres");
			return;
		}
		else
		{
			int health = osl_StrToInt(p1);
			int ammo = osl_StrToInt(p2);
			int score = osl_StrToInt(p3);
			texture_t* texture = TextureByName((char*)osl_RemoveQuotesStr(p4).c_str());
			if(!texture)
				osl_Warning("Texture not found");
			int x = osl_StrToInt(p5);
			int y = osl_StrToInt(p6);
			int pattern_width = osl_StrToInt(p7);
			int pattern_height = osl_StrToInt(p8);
			int image_width = osl_StrToInt(p9);
			int image_height = osl_StrToInt(p10);
			int shoot1_p = osl_StrToInt(p11);
			int shoot1_c = osl_StrToInt(p12);
			int shoot2_p = osl_StrToInt(p13);
			int shoot2_c = osl_StrToInt(p14);
			int shoot3_p = osl_StrToInt(p15);
			int shoot3_c = osl_StrToInt(p16);
			int stand_p = osl_StrToInt(p17);
			int stand_c = osl_StrToInt(p18);
			int walk_p = osl_StrToInt(p19);
			int walk_c = osl_StrToInt(p20);
			int sit_p = osl_StrToInt(p21);
			int sit_c = osl_StrToInt(p22);
			int idle_p = osl_StrToInt(p23);
			int idle_c = osl_StrToInt(p24);
			int talk_p = osl_StrToInt(p25);
			int talk_c = osl_StrToInt(p26);
			ANIMATION_TYPE type = ANIMATION_TYPE(osl_StrToInt(p27));
			bool looped = osl_StrToInt(p28) != 0;

			PLAYER* player = new PLAYER(health, ammo, score);
			player->SetTexture(texture);
			player->SetX(x);
			player->SetY(y);
			player->SetPatternWidth(pattern_width);
			player->SetPatternHeight(pattern_height);
			player->SetImageWidth(image_width);
			player->SetImageHeight(image_height);
			player->SetShoot1Pattern(shoot1_p);
			player->SetShoot1Count(shoot1_c);
			player->SetShoot2Pattern(shoot2_p);
			player->SetShoot2Count(shoot2_c);
			player->SetShoot3Pattern(shoot3_p);
			player->SetShoot3Count(shoot3_c);
			player->SetWalkPattern(walk_p);
			player->SetWalkCount(walk_c);
			player->SetStandPattern(stand_p);
			player->SetStandCount(stand_c);
			player->SetSitPattern(sit_p);
			player->SetSitCount(sit_c);
			player->SetIdlePattern(idle_p);
			player->SetIdleCount(idle_c);
			player->SetTalkPattern(talk_p);
			player->SetTalkCount(talk_c);
			player->SetCurrentAnimation(type, looped);
			player->SetIICHAN(iichan);
			iichan->SetPlayer(player);

		}
	}
	else
	{
		osl_Error("Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Player created");
}

void CreatePowerup(string p1, string p2, string p3, string p4)
{
	if(iichan)
	{
		POWERUP_TYPE type = POWERUP_TYPE(osl_StrToInt(p1));
		int count = osl_StrToInt(p2);
		int x = osl_StrToInt(p3);
		int y = osl_StrToInt(p4);

		iichan->CreatePowerup(type, count, x, y);
	}
	else
	{
		osl_Error("Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Powerup created");
}

void CreateEnemy(string p1, string p2, string p3)
{
	if(iichan)
	{
		int x = osl_StrToInt(p1);
		int y = osl_StrToInt(p2);
		int health = osl_StrToInt(p3);
		iichan->CreateEnemy(x, y, health);

	}
	else
	{
		osl_Error("Game is not created");
		return;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Enemy created");
}