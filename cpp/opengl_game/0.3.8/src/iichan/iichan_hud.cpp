#include "stdafx.h"
#include "iichan_hud.h"
#include "../draw/font.h"
#include "../draw/draw.h"
#include "../config.h"
#include "iichan_defines.h"

extern config cfg;

HUD::HUD()
{
#ifdef SIMPLE_HUD
		health = new SPRITE();
		health->SetTexture(TextureByName("heart"));
		health->SetPatternWidth(11);
		health->SetPatternHeight(11);
		health->SetImageWidth(11);
		health->SetImageHeight(11);
		health->SetMirror(false);
		health->SetX(HUD_X + HEALTH_X);
		health->SetY(cfg.scr_height - HUD_DY);
		health->SetFixed(true);
		health->SetCollidable(false);

		ammo = new SPRITE();
		ammo->SetTexture(TextureByName("ammo"));
		ammo->SetPatternWidth(14);
		ammo->SetPatternHeight(16);
		ammo->SetImageWidth(14);
		ammo->SetImageHeight(16);
		ammo->SetMirror(false);
		ammo->SetX(HUD_X + AMMO_X);
		ammo->SetY(cfg.scr_height - HUD_DY);
		ammo->SetFixed(true);
		ammo->SetCollidable(false);

		score = new SPRITE();
		score->SetTexture(TextureByName("score"));
		score->SetPatternWidth(11);
		score->SetPatternHeight(14);
		score->SetImageWidth(11);
		score->SetImageHeight(14);
		score->SetMirror(false);
		score->SetX(HUD_X + SCORE_X);
		score->SetY(cfg.scr_height - HUD_DY);
		score->SetFixed(true);
		score->SetCollidable(false);
#else
	health.resize(0);
	ammo.resize(0);
	score.resize(0);
#endif

	healthCount = 0;
	ammoCount = 0;
	scoreCount = 0;
}

HUD::~HUD()
{
#ifdef SIMPLE_HUD
	if(health)
	{
		delete health;
		health = NULL;
	}
	if(ammo)
	{
		delete ammo;
		ammo = NULL;
	}
	if(score)
	{
		delete score;
		score = NULL;
	}
#else
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
#endif
}

void HUD::Draw()
{
	static const Font* font = FontByName("default");
	static const int y = cfg.scr_height - HUD_DY;
	char buffer[8];
	int x = 0;

	DarkBox(0, y, cfg.scr_width, cfg.scr_height);

#ifdef SIMPLE_HUD
	if(health)
	{
		health->Draw();
		sprintf(buffer, "%d\0", healthCount);
		PrintText(buffer, font, health->GetX() + 25, y);
	}
	if(ammo)
	{
		ammo->Draw();
		sprintf(buffer, "%d\0", ammoCount);
		PrintText(buffer, font, ammo->GetX() + 25, y);
	}
	if(score)
	{
		score->Draw();
		sprintf(buffer, "%d\0", scoreCount);
		PrintText(buffer, font, score->GetX() + 25, y);
	}
#else
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

	x = cfg.scr_width - 20;

	sprintf(buffer, "%d\0", scoreCount);

	PrintText(buffer, font, x, cfg.scr_height - HUD_DY);
#endif
}

void HUD::AddHealth()
{
	healthCount++;
#ifdef SIMPLE_HUD
#else
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
		spr->SetCollidable(false);

		health.push_back(spr);
	}
#endif
}

void HUD::AddAmmo()
{
	ammoCount++;
#ifdef SIMPLE_HUD
#else
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
		spr->SetCollidable(false);

		ammo.push_back(spr);
	}
#endif
}

void HUD::AddScore()
{
	scoreCount++;
#ifdef SIMPLE_HUD
#else
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
		spr->SetCollidable(false);

		score.push_back(spr);
	}
#endif
}

void HUD::RemoveHealth()
{
#ifdef SIMPLE_HUD
#else
	if(healthCount % HUD_Q == 0)
	{
		if(health.size() > 0)
		{
			delete health[health.size()-1];
			health.erase(health.begin() + health.size() - 1);
		}
	}
#endif
	healthCount--;
}

void HUD::RemoveAmmo()
{
#ifdef SIMPLE_HUD
#else
	if(ammoCount % HUD_Q == 0)
	{
		if(ammo.size() > 0)
		{
			delete ammo[ammo.size()-1];
			ammo.erase(ammo.begin() + ammo.size() - 1);
		}
	}
#endif
	ammoCount--;
}

void HUD::RemoveScore()
{
#ifdef SIMPLE_HUD
#else
	if(scoreCount % HUD_Q == 0)
	{
		if(score.size() > 0)
		{
			delete score[score.size()-1];
			score.erase(score.begin() + score.size() - 1);
		}
	}
#endif
	scoreCount--;
}
