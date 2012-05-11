#include "stdafx.h"
#include "iichan_hud.h"
#include "../draw/font.h"
#include "../draw/draw.h"
#include "../config.h"
#include "iichan_defines.h"

extern config cfg;

HUD::HUD()
{
	health.resize(0);
	ammo.resize(0);
	score.resize(0);

	healthCount = 0;
	ammoCount = 0;
	scoreCount = 0;
}

HUD::~HUD()
{
	UINT l = health.size();
	for(UINT i = 0; i < l; i++)
	{
		SPRITE* spr = health[i];
		if(spr)
		{
			delete spr;
			spr = NULL;
		}
	}
	health.clear();

	l = ammo.size();
	for(UINT i = 0; i < l; i++)
	{
		SPRITE* spr = ammo[i];
		if(spr)
		{
			delete spr;
			spr = NULL;
		}
	}
	ammo.clear();

	l = score.size();
	for(UINT i = 0; i < l; i++)
	{
		SPRITE* spr = score[i];
		if(spr)
		{
			delete spr;
			spr = NULL;
		}
	}
	score.clear();
}

void HUD::Draw()
{
	static const font_t* font = FontByName("default");
	char buffer[8];
	int x = 0;

	for(vector<SPRITE*>::iterator it = health.begin();
		it != health.end();
		it++)
	{
		SPRITE* spr = *it;
		if(spr)
			spr->Draw();
	}

	sprintf(buffer, "%d\0", healthCount);

	if(health.size() > 0)
		x = health[health.size()-1]->GetX() +
		health[health.size()-1]->GetPatternWidth() + 5;
	else
		x = 50;

	PrintText(buffer, font,	x, cfg.scr_height - HUD_DY);

	for(vector<SPRITE*>::iterator it = ammo.begin();
		it != ammo.end();
		it++)
	{
		SPRITE* spr = *it;
		if(spr)
			spr->Draw();
	}

	if(ammo.size() > 0)
		x = ammo[ammo.size()-1]->GetX() +
		ammo[ammo.size()-1]->GetPatternWidth() + 5;
	else
		x = 150;

	sprintf(buffer, "%d\0", ammoCount);

	PrintText(buffer, font, x, cfg.scr_height - HUD_DY);

	for(vector<SPRITE*>::iterator it = score.begin();
		it != score.end();
		it++)
	{
		SPRITE* spr = *it;
		if(spr)
			spr->Draw();
	}

	//if(score.size() > 0)
	//	x = score[score.size()-1]->GetX() +
	//	score[score.size()-1]->GetPatternWidth() + 5;
	//else
	//	x = 200;
	x = cfg.scr_width - 20;

	sprintf(buffer, "%d\0", scoreCount);

	PrintText(buffer, font, x, cfg.scr_height - HUD_DY);

}

void HUD::AddHealth()
{
	healthCount++;

	if(healthCount % HUD_Q == 0 || healthCount <= 1)
	{
		SPRITE* spr = new SPRITE();

		spr->SetTexture(TextureByName("heart"));
		spr->SetPatternWidth(11);
		spr->SetPatternHeight(11);
		spr->SetImageWidth(11);
		spr->SetImageHeight(11);
		spr->SetMirror(false);
		spr->SetX(HUD_X + health.size() * spr->GetPatternWidth() + 15);
		spr->SetY(cfg.scr_height - HUD_DY);
		spr->SetFixed(true);

		health.push_back(spr);
	}
}

void HUD::AddAmmo()
{
	ammoCount++;
	
	if(ammoCount % HUD_Q == 0 || ammoCount <= 1)
	{
		SPRITE* spr = new SPRITE();

		spr->SetTexture(TextureByName("ammo"));
		spr->SetPatternWidth(14);
		spr->SetPatternHeight(16);
		spr->SetImageWidth(14);
		spr->SetImageHeight(16);
		spr->SetMirror(false);
		spr->SetX(cfg.scr_width / 2 + ammo.size() * spr->GetPatternWidth() - 15);
		spr->SetY(cfg.scr_height - HUD_DY);
		spr->SetFixed(true);

		ammo.push_back(spr);
	}
}

void HUD::AddScore()
{
	scoreCount++;
	if(scoreCount % HUD_Q == 0 || score.size() <= 0)
	{
		SPRITE* spr = new SPRITE();

		spr->SetTexture(TextureByName("score"));
		spr->SetPatternWidth(11);
		spr->SetPatternHeight(14);
		spr->SetImageWidth(11);
		spr->SetImageHeight(14);
		spr->SetMirror(false);
		spr->SetX(cfg.scr_width - score.size() * spr->GetPatternWidth() - 35);
		spr->SetY(cfg.scr_height - HUD_DY);
		spr->SetFixed(true);

		score.push_back(spr);
	}
}

void HUD::RemoveHealth()
{

	if(healthCount % HUD_Q == 0)
	{
		if(health.size() > 0)
		{
			delete health[health.size()-1];
			health.erase(health.begin() + health.size() - 1);
		}
	}

	healthCount--;
}

void HUD::RemoveAmmo()
{
	if(ammoCount % HUD_Q == 0)
	{
		if(ammo.size() > 0)
		{
			delete ammo[ammo.size()-1];
			ammo.erase(ammo.begin() + ammo.size() - 1);
		}
	}

	ammoCount--;
}

void HUD::RemoveScore()
{
	if(scoreCount % HUD_Q == 0)
	{
		if(score.size() > 0)
		{
			delete score[score.size()-1];
			score.erase(score.begin() + score.size() - 1);
		}
	}

	scoreCount--;
}
