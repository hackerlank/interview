#include "stdafx.h"
#include "texture.h"

#include "config.h"
#include "font.h"
#include "draw.h"
#include "iichan.h"
#include "iichan_defines.h"

#include "misc.h"

#include "gui.h"

#include "osl.h"

unsigned int current_state;

extern char path_levels[MAX_PATH];
extern bool keys[256];
extern config cfg;
extern GUI* gui;


void CHARACTER::Shoot()
{
	BULLET* bullet = new BULLET();
	bullet->SetTexture(TextureByName("bullet"));
	bullet->SetPatternWidth(64);
	bullet->SetPatternHeight(32);
	bullet->SetY(GetY() + 36);
	if(GetMirror())
		bullet->SetX(GetX() - 60);
	else
		bullet->SetX(GetX() + 60);
		
	bullet->SetIICHAN(GetIICHAN());
	bullet->SetLife(cfg.bullet_life);
	bullet->SetSpeed(cfg.bullet_speed);
	bullet->SetDamage(cfg.bullet_damage);
	bullet->SetMirror(GetMirror());

	GetIICHAN()->AddBullet(bullet);

	SetAmmo(GetAmmo()-1);
	
}

void PLAYER::CollectPowerUp(enum POWERUP_TYPE type, int count)
{
	char* msg = new char[64];
	switch(type)
	{
	case pwrHealth:
		SetHealth(GetHealth() + count);
		sprintf(msg, "Health collected (+%u)\0", count);
		break;
	case pwrAmmo:
		SetAmmo(GetAmmo() + count);
		sprintf(msg, "Ammo collected (+%u)\0", count);
		break;
	case pwrScore:
		SetScore(GetScore() + count);
		sprintf(msg, "Score collected (+%u)\0", count);
		break;
	}

			
	gui->LogOnScreen(msg);
}

void PHRASE::Draw()
{
		__super::Draw();

		PrintText(GetText().c_str(), FontByName("oorion_font"), GetX()+10, GetY()+10);
}

IICHAN::IICHAN()
{
	SetPlayer(NULL);
}

void IICHAN::ProcessPlayer()
{
	if(!player->GetShooting() && !player->GetJumping() && !player->GetMoving())
		if(player->GetCurrentAnimation() != animStand)
			player->SetCurrentAnimation(animStand, true);

	if(keys[cfg.fire])
	{
		if(!player->GetMoving())
		{
			player->SetShooting(true);
			player->SetCurrentAnimation(animShoot1, false);
		}
	}
	if(keys[cfg.right])
	{
		player->x = player->x + cfg.hero_speed;
		player->SetMirror(false);
		player->SetMoving(true);
		player->SetMovingDirection(false);
	}
	if(keys[cfg.left])
	{
		player->x = player->x - cfg.hero_speed;
		player->SetMirror(true);
		player->SetMoving(true);
		player->SetMovingDirection(true);
	}

	if(!keys[cfg.left] &&
		player->GetMoving() &&
		player->GetMovingDirection() == true)
	{
		player->SetMoving(false);
	}

	if(!keys[cfg.right] &&
		player->GetMoving() &&
		player->GetMovingDirection() == false)
	{
		player->SetMoving(false);
	}

	if(keys[cfg.down])
	{
		if(!player->GetShooting())
			player->SetCurrentAnimation(animSit, true);
	}
	if(keys[cfg.jump])
	{
		player->SetJumping(true);
	}
		
	if(player->GetJumping())
	{

		if(player->GetJumps() < cfg.hero_jump_height && !player->GetJumpDirection())
		{
			player->SetY(player->GetY() - cfg.hero_speed);
			player->SetJumps(player->GetJumps() + 1);
		}
		else
		{
			player->SetJumpDirection(true);
			player->SetY(player->GetY() + cfg.hero_speed);
			player->SetJumps(player->GetJumps() - 1);
			if(player->GetJumps() <= 0)
			{
				player->SetJumping(false);
				player->SetJumpDirection(false);
			}
		}
	}

	if(player->GetShooting())
	{
		if(player->GetShoots() < cfg.hero_reload)
		{
			if(player->GetShoots() == 1)
			{
				player->Shoot();
				if(!player->GetMirror())
					CreateExplosion(player->GetX()+80, player->GetY()+40, false);
				else
					CreateExplosion(player->GetX()-20, player->GetY()+40, false);
			}
			player->SetShoots(player->GetShoots() + 1);
		}
		else
		{
			player->SetShooting(false);
			player->SetShoots(0);
		}
	}

	if(player->GetMoving())
	{
		player->SetCurrentAnimation(animWalk, true);
	}

}

POWERUP::tagPOWERUP(enum POWERUP_TYPE type, int count)
{
	SetType(type);
	SetCount(count);
	SetDead(false);
}


void IICHAN::ProcessBullets()
{
	for(vector<BULLET*>::iterator it = bullets.begin();
		it != bullets.end();
		it++)
	{
		BULLET* bullet = *it;

		if(bullet)
		{
			for(vector<CHARACTER*>::iterator it = enemies.begin();
				it != enemies.end();
				it++)
			{
				CHARACTER* enemy = *it;

				if(PointInBox(bullet->GetX(),
					bullet->GetY(),
					enemy->GetX(),
					enemy->GetY(),
					enemy->GetPatternWidth(),
					enemy->GetPatternHeight()))
				{
					bullet->SetLife(0);
					enemy->SetHealth(enemy->GetHealth() - bullet->GetDamage());
				}
				bullet->SetLife(bullet->GetLife()-1);
			}		
		}
	}

	for(UINT i = 0; i < bullets.size(); i++)
	{
		if(bullets[i]->GetLife() <= 0)
		{
			int dx;
			bullets[i]->GetMirror() ? dx = - 20 : dx = 20;

			CreateExplosion(bullets[i]->GetX() + dx,
				bullets[i]->GetY(),
				true);
			delete bullets[i];
			bullets.erase(bullets.begin()+i);
		}
	}
}

void IICHAN::MoveBullets()
{
	for(vector<BULLET*>::iterator it = bullets.begin();
		it != bullets.end();
		it++)
	{
		BULLET* bullet = *it;
		int dx;
		bullet->GetMirror() ? dx = -bullet->GetSpeed() : dx = bullet->GetSpeed();
		bullet->SetX(bullet->GetX() + dx);
	}
}

void IICHAN::ProcessPowerups()
{
	for(UINT i = 0; i < powerups.size(); i++)
	{
		if(powerups[i]->GetDead())
		{
			delete powerups[i];
			powerups.erase(powerups.begin()+i);
		}
	}
}

void IICHAN::MovePowerups()
{
	for(vector<POWERUP*>::iterator it = powerups.begin();
		it != powerups.end();
		it++)
	{
		POWERUP* pwr = *it;

		if(pwr)
		{
			pwr->Move();
			
			if(GetPlayer())
			{

				if(PointInBox(pwr->GetX() + pwr->GetPatternWidth() / 2,
					pwr->GetY() + pwr->GetPatternHeight() / 2,
					player->GetX(), player->GetY(),
					player->GetPatternWidth() / 2, player->GetPatternHeight()))
				{
					player->CollectPowerUp(pwr->GetType(), pwr->GetCount());
					pwr->SetDead(true);
				}
			}
		}
	}
}

void IICHAN::ProcessExplosions()
{
	for(UINT i = 0; i < explosions.size(); i++)
	{
		if(explosions[i]->GetDead())
		{
			delete explosions[i];
			explosions.erase(explosions.begin()+i);
		}
	}
}

void IICHAN::ProcessEnemies()
{
	for(UINT i = 0; i < enemies.size(); i++)
	{
		CHARACTER* enemy = enemies[i];
		if(enemy->GetX() > player->GetX())
		{
			enemy->SetX(enemy->GetX() - cfg.enemy_speed);
			enemy->SetMirror(true);
		}
		else
		{
			enemy->SetX(enemy->GetX() + cfg.enemy_speed);
			enemy->SetMirror(false);
		}

		if(enemy->GetHealth() <= 0)
		{
			enemy->SetDead(true);
		}

		if(enemy->GetDead())
		{
			delete enemies[i];
			enemies.erase(enemies.begin()+i);
		}
	}
}

void IICHAN::ProcessPhrases()
{
	for(UINT i = 0; i < phrases.size(); i++)
	{
		PHRASE* p = phrases[i];
		p->SetTime(p->GetTime() - 1);

		if(p->GetTime() <= 0)
			p->SetDead(true);

		if(p->GetDead())
		{
			delete phrases[i];
			phrases.erase(phrases.begin() + i);
		}
	}
}

void IICHAN::MoveExplosions()
{
	for(vector<EXPLOSION*>::iterator it = explosions.begin();
		it != explosions.end();
		it++)
	{
		EXPLOSION* exp = *it;

		if(exp)
		{
			exp->Move();
		}
	}
}

void IICHAN::MoveEnemies()
{
	for(vector<CHARACTER*>::iterator it = enemies.begin();
		it != enemies.end();
		it++)
	{
		CHARACTER* enemy = *it;

		if(enemy)
		{
			enemy->Move();
		}
	}
}

void IICHAN::MovePhrases()
{
	//for(vector<PHRASE*>::iterator it = phrases.begin();
	//	it != phrases.end();
	//	it++)
	//{
	//	PHRASE* p = *it;

	//	if(p)
	//	{
	//		p->
}


void IICHAN::Process()
{
	int current_tick = GetTickCount();
	static int tick10 = 0;
	static int tick100 = 0;


	if(current_tick - tick10 >= 10)
	{
		if(bullets.size() > 0)
			MoveBullets();

		if(explosions.size() > 0)
			MoveExplosions();
		
		if(phrases.size() > 0)
			MovePhrases();

		if(player)
			ProcessPlayer();

		if(enemies.size() > 0)
			ProcessEnemies();

		tick10 = current_tick;
	}

	if(current_tick - tick100 >= 100)
	{
		if(powerups.size() > 0)
			MovePowerups();

		if(player)
			player->Move();

		if(enemies.size() > 0)
			MoveEnemies();

		tick100 = current_tick;
	}


	if(powerups.size() > 0)
		ProcessPowerups();
	if(explosions.size() > 0)
		ProcessExplosions();
	if(bullets.size() > 0)
		ProcessBullets();
	if(phrases.size() > 0)
		ProcessPhrases();
}

void IICHAN::Draw()
{

	if(background.size() > 0)
		for(vector<SPRITE*>::iterator it = background.begin();
			it != background.end();
			it++)
		{
			SPRITE* spr = *it;

			if(spr)
			{
				glColor3f(1.0, 1.0, 1.0);
				spr->Draw();
			}
		}
	if(bullets.size() > 0)
		for(vector<BULLET*>::iterator it = bullets.begin();
			it != bullets.end();
			it++)
		{
			BULLET* bullet = *it;

			if(bullet)
			{
				glColor3f(1.0, 1.0, 1.0);
				bullet->Draw();
			}
		}

	if(powerups.size() > 0)
		for(vector<POWERUP*>::iterator it = powerups.begin();
			it != powerups.end();
			it++)
		{
			POWERUP* pwr = *it;

			if(pwr)
			{
				glColor3f(1.0, 1.0, 1.0);
				pwr->Draw();
			}
		}

	if(player)
	{
		char hud[64];

		sprintf(hud, "Health: %d   Ammo: %d   Score: %d",
			player->GetHealth(), player->GetAmmo(), player->GetScore());

		glDisable(GL_BLEND);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		player->Draw();
		glEnable(GL_BLEND);

		static font_t* font = FontByName("oorion_font");
		static int hud_y = cfg.scr_height - 20;
		static int hud_x = 10;
		PrintText(hud, font, hud_x, hud_y);
	}

	if(enemies.size() > 0)
		for(vector<CHARACTER*>::iterator it = enemies.begin();
			it != enemies.end();
			it++)
		{
			CHARACTER* enemy = *it;

			if(enemy)
			{
				glDisable(GL_BLEND);
				glColor4f(1.0, 1.0, 1.0, 1.0);
				enemy->Draw();
				glEnable(GL_BLEND);
			}
		}

	if(explosions.size() > 0)
		for(vector<EXPLOSION*>::iterator it = explosions.begin();
			it != explosions.end();
			it++)
		{
			EXPLOSION* exp = *it;

			if(exp)
			{
				glColor3f(1.0, 1.0, 1.0);
				exp->Draw();
			}
		}
	if(phrases.size() > 0)
		for(vector<PHRASE*>::iterator it = phrases.begin();
			it != phrases.end();
			it++)
		{
			PHRASE* p = *it;

			if(p)
			{
				glColor3f(1.0, 1.0, 1.0);
				p->Draw();
			}
		}


}

void IICHAN::AddBullet(BULLET *spr)
{
	bullets.push_back(spr);
}

void IICHAN::AddPowerup(POWERUP* pwr)
{
	powerups.push_back(pwr);
}

void IICHAN::AddBackground(SPRITE* spr)
{
	background.push_back(spr);
}

void IICHAN::AddExplosion(EXPLOSION* exp)
{
	explosions.push_back(exp);
}

void IICHAN::AddEnemy(CHARACTER* enemy)
{
	enemies.push_back(enemy);
}

void IICHAN::AddPhrase(PHRASE* p)
{
	phrases.push_back(p);
}

void IICHAN::CreatePowerup(enum POWERUP_TYPE type, int count, int x, int y)
{
	POWERUP* pwr = new POWERUP(type, count);

	pwr->SetTexture(TextureByName("powerups"));
	pwr->SetPatternWidth(16);
	pwr->SetPatternHeight(16);
	pwr->SetImageWidth(464);
	pwr->SetImageHeight(16);

	switch(type)
	{
	case pwrScore:
		pwr->SetStartPattern(0);
		pwr->SetPatternCount(20);
		break;
	case pwrHealth:
		pwr->SetStartPattern(20);
		pwr->SetPatternCount(3);
		break;
	case pwrAmmo:
		pwr->SetStartPattern(23);
		pwr->SetPatternCount(6);			
		break;
	}

	pwr->SetX(x);
	pwr->SetY(y);
	pwr->SetCurrentPattern(pwr->GetStartPattern());
	pwr->SetMirror(false);
	pwr->SetReverse(false);

	AddPowerup(pwr);
}

void IICHAN::CreateExplosion(int x, int y, bool reverse)
{
	EXPLOSION* spr = new EXPLOSION();

	spr->SetTexture(TextureByName("explosions"));
	spr->SetX(x);
	spr->SetY(y);
	spr->SetReverse(reverse);
	spr->SetImageWidth(150);
	spr->SetImageHeight(32);
	spr->SetPatternWidth(50);
	spr->SetPatternHeight(32);
	spr->SetPatternCount(3);
	if(reverse)
		spr->SetStartPattern(2);
	else
		spr->SetStartPattern(0);
	spr->SetLooped(false);
	spr->SetIICHAN(this);
	spr->SetMirror(false);
	spr->SetDead(false);

	AddExplosion(spr);

}

void IICHAN::CreateEnemy(int x, int y, int health)
{
	CHARACTER* enemy = new CHARACTER();

	enemy->SetTexture(TextureByName("enemy"));
	enemy->SetX(x);
	enemy->SetY(y);
	enemy->SetHealth(health);
	enemy->SetImageWidth(770);
	enemy->SetImageHeight(97);
	enemy->SetPatternWidth(70);
	enemy->SetPatternHeight(97);
	enemy->SetIICHAN(this);
	enemy->SetWalkPattern(0);
	enemy->SetWalkCount(10);
	enemy->SetCurrentAnimation(animWalk, true);
	enemy->SetMirror(x > player->GetX());
	
	AddEnemy(enemy);
}

void IICHAN::CreatePhrase(int x, int y, string text, int time)
{
	PHRASE* p = new PHRASE(text, time);
	p->SetX(x);
	p->SetY(y);
	p->SetTexture(TextureByName("baloon"));
	p->SetImageWidth(90);
	p->SetImageHeight(32);
	p->SetPatternWidth(90);
	p->SetPatternHeight(32);
	
	AddPhrase(p);
}

void IICHAN::Init()
{
	SPRITE* spr = new SPRITE();

	spr->SetX(0);
	spr->SetY(0);
	spr->SetTexture(TextureByName("background"));
	spr->SetPatternWidth(800);
	spr->SetPatternHeight(480);
	spr->SetMirror(false);

	AddBackground(spr);

	osl_Load_v_04("levels\\level1.txt");

	if(player)
	{
		CreatePhrase(player->GetX(), player->GetY()-50, "O, hi", 2000);
	}



}


void IICHAN::Destroy()
{
	UINT l = powerups.size();
	for(UINT i = 0; i < l; i++)
	{
		POWERUP* pwr = powerups[i];
		if(pwr)
		{
			delete pwr;
			pwr = NULL;
		}
	}
	powerups.clear();
	l = background.size();
	for(UINT i = 0; i < l; i++)
	{
		SPRITE* spr = background[i];
		if(spr)
		{
			delete spr;
			spr = NULL;
		}
	}
	background.clear();
	l = bullets.size();
	for(UINT i = 0; i < l; i++)
	{
		SPRITE* spr = bullets[i];
		if(spr)
		{
			delete spr;
			spr = NULL;
		}
	}
	bullets.clear();
	if(player)
	{
		delete player;
		player = NULL;
	}
	l = explosions.size();
	for(UINT i = 0; i < l; i++)
	{
		EXPLOSION* spr = explosions[i];
		if(spr)
		{
			delete spr;
			spr = NULL;
		}
	}
	explosions.clear();
	l = enemies.size();
	for(UINT i = 0; i < l; i++)
	{
		CHARACTER* enemy = enemies[i];
		if(enemy)
		{
			delete enemy;
			enemy = NULL;
		}
	}
	enemies.clear();
	l = phrases.size();
	for(UINT i = 0; i < l; i++)
	{
		PHRASE* p = phrases[i];
		if(p)
		{
			delete p;
			p = NULL;
		}
	}
	phrases.clear();

}



bool IICHAN::LoadLevel(const char* file_name)
{
	//char buffer[64];
	//sprintf(buffer, "Loading level: %s", file_name);
	//WriteToLog(buffer);

	//FILE* f = fopen(file_name, "rb");

	//LEVEL* level = new LEVEL();

	//if(!f)
	//{
	//	WriteToLog("Error opening level");
	//	delete level;
	//	return false;
	//}
	//else
	//{
	//	WORD buf2;
	//	size_t recs;

	//	/* width */
	//	recs = fread(&buf2, sizeof(WORD), 1, f);
	//	if(recs != 1)
	//	{
	//		WriteToLog("Failed to read width");
	//		delete level;
	//		return false;
	//	}
	//	level->sizeX = buf2;

	//	/* height */
	//	recs = fread(&buf2, sizeof(WORD), 1, f);
	//	if(recs != 1)
	//	{
	//		WriteToLog("Failed to read height");
	//		delete level;
	//		return false;
	//	}
	//	level->sizeY = buf2;

	//	/* hero x */
	//	recs = fread(&buf2, sizeof(WORD), 1, f);
	//	if(recs != 1)
	//	{
	//		WriteToLog("Failed to read width");
	//		delete level;
	//		return false;
	//	}
	//	level->heroX = buf2;

	//	/* hero y */
	//	recs = fread(&buf2, sizeof(WORD), 1, f);
	//	if(recs != 1)
	//	{
	//		WriteToLog("Failed to read width");
	//		delete level;
	//		return false;
	//	}
	//	level->heroY = buf2;
	//	
	//	UINT header_size = ftell(f) * sizeof(WORD);
	//	ShowNumber(header_size);
	//	UINT file_size = fseek(f, 0, SEEK_END);
	//	ShowNumber(file_size);
	//	UINT data_size = file_size - header_size;
	//	ShowNumber(data_size);

	//	level->cells = new LEVELCELL[data_size/sizeof(LEVELCELL)];
	//	fseek(f, header_size, SEEK_SET);
	//
	//	/* reading cells */
	//	UINT cells_read = 0;
	//	LEVELCELL cell;
	//	while(!feof(f))
	//	{		
	//		recs = fread(&cell, sizeof(LEVELCELL), 1, f);
	//		if(recs != 1)
	//		{
	//			WriteToLog("Failed to read cell");
	//			delete level;
	//			return false;
	//		}
	//		cells_read++;
	//	

	//		level->cells[cells_read-1] = cell;
	//	}
	//	WriteToLog("Level loaded");
	//	return true;
	//}
	return false;
}
			
