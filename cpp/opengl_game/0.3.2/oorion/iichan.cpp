#include "stdafx.h"
//#include "texture.h"

#include "config.h"
#include "font.h"
#include "draw.h"

#include "sprite.h"
#include "animatedsprite.h"
#include "character.h"
#include "iichan_small_types.h"
#include "player.h"


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

int WORLD_X = 0;
int WORLD_Y = 0;
int WORLD_MAX_X = 2000;
int WORLD_MIN_X = 0;



void HUD::Destroy()
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

	sprintf(buffer, "%d", healthCount);

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

	sprintf(buffer, "%d", ammoCount);

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

	sprintf(buffer, "%d", scoreCount);

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









IICHAN::IICHAN()
{
	SetPlayer(NULL);
}

void IICHAN::ProcessPlayer()
{
	if(!player->GetShooting() && !player->GetJumping() && !player->GetMoving() && player->GetDoneAnimation())
		if(player->GetCurrentAnimation() != animStand)
			player->SetCurrentAnimation(animStand, true);

	if(keys[cfg.fire])
	{
		if(!player->GetShooting())
		{
			if(player->GetJumping() || player->GetMoving())
			{
				if(player->GetAmmo() > 0)
				{
					player->SetShooting(true);
				}
			}
			else		
			{
				if(player->GetAmmo() > 0)
				{
					player->SetShooting(true);
					player->SetCurrentAnimation(animShoot1, false);
				}
			}
		}
	}
	if(keys[cfg.right])
	{
		player->SetMirror(false);
		if(!player->GetJumping())
		{
			player->SetMoving(true);
			player->SetMovingDirection(false);
		}

		if(player->GetX() - WORLD_X + (int)cfg.hero_speed <= cfg.scr_width - player->GetPatternWidth())
			player->SetX(player->GetX() + (int)cfg.hero_speed);

		if((player->GetX() - WORLD_X + (int)cfg.hero_speed >= cfg.scr_width - BORDER_MARGIN) &&
			(WORLD_X + (int)cfg.hero_speed <= WORLD_MAX_X))
			WORLD_X += (int)cfg.hero_speed;


	}
	if(keys[cfg.left])
	{
		player->SetMirror(true);
		if(!player->GetJumping())
		{
			player->SetMoving(true);
			player->SetMovingDirection(true);
		}
		if(player->GetX() - (int)cfg.hero_speed >= 0)
			player->SetX(player->GetX() - (int)cfg.hero_speed);

		if((player->GetX() - WORLD_X - (int)cfg.hero_speed <= BORDER_MARGIN) &&
			(WORLD_X - (int)cfg.hero_speed >= WORLD_MIN_X))
			WORLD_X -= (int)cfg.hero_speed;
	}

	if(!keys[cfg.left] &&
		player->GetMoving() &&
		player->GetMovingDirection() == true)
	{
		player->SetMoving(false);
		if(player->GetCurrentAnimation() == animWalk)
			player->SetCurrentAnimation(animStand, true);
	}

	if(!keys[cfg.right] &&
		player->GetMoving() &&
		player->GetMovingDirection() == false)
	{
		player->SetMoving(false);
		if(player->GetCurrentAnimation() == animWalk)
			player->SetCurrentAnimation(animStand, true);
	}

	//if(keys[cfg.down])
	//{
	//	if(!player->GetShooting())
	//		player->SetCurrentAnimation(animSit, true);
	//}
	if(keys[cfg.jump])
	{
		if(!player->GetJumping())
		{
			player->SetJumping(true);
			player->SetYSpeed(cfg.hero_speed);
		}
	}
		
	if(player->GetJumping())
	{
		if(player->GetMoving())
			player->SetMoving(false);
		//if(!player->GetShooting())
		//{
			player->SetCurrentAnimation(animJump, false);
		//}
			static float ay = - (cfg.gravity * PROCESS_DELAY / 1000);
			player->SetYSpeed(player->GetYSpeed() + ay);
			player->SetJumpDirection(player->GetYSpeed() < 0);
			if(!player->GetJumpDirection())
				player->SetY(int(player->GetY() - player->GetYSpeed()));
			else
			{
				if(player->GetY() + player->GetYSpeed() < player->GetYStart())
					player->SetY(int(player->GetY() - player->GetYSpeed()));
				else
				{
					player->SetY(player->GetYStart());
					player->SetJumping(false);
					player->SetLanding(true);
				}
			}
		//}

	}

	if(player->GetLanding())
	{
		player->SetLandPattern(26);
		player->SetLandCount(2);
		player->SetCurrentAnimation(animLand, false);

		// here create puffs
		//
		//
		player->SetLanding(false);
	}

	if(player->GetShooting())
	{
		if(player->GetShoots() < cfg.hero_reload)
		{
			if(player->GetShoots() == 1)
			{
				player->Shoot();
				hud->RemoveAmmo();
				if(!player->GetMirror())
				{
					CreateExplosion(player->GetX() + FLASH_DX_RIGHT, player->GetY() + FLASH_DY, false, false);
					CreateBullet(player->GetX() + FLASH_DX_RIGHT, player->GetY() + FLASH_DY, false);
				}
				else
				{
					CreateExplosion(player->GetX() - FLASH_DX_LEFT, player->GetY() + FLASH_DY, true, true);
					CreateBullet(player->GetX() - FLASH_DX_LEFT, player->GetY() + FLASH_DY, true);
				}
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
		//for(vector<SPRITE*>::iterator it = objects.begin();
		//	it != objects.end();
		//	it++)
		//{
		//	SPRITE* obj = *it;

		//	if(obj)
		//	{
		//		obj->SetX(obj->GetX() - GetWorldX());
		//	}
		//}
		player->SetCurrentAnimation(animWalk, true);
	}

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
					enemy->GetPatternWidth() / 2 ,
					enemy->GetPatternHeight() / 2))
				{
					bullet->SetLife(0);
					enemy->SetHealth(enemy->GetHealth() - bullet->GetDamage());
					break;
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

			//CreateExplosion(bullets[i]->GetX() + dx,
			//	bullets[i]->GetY(),
			//	true);
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

					switch(pwr->GetType())
					{
					case pwrHealth:
						for(int i = 0; i < pwr->GetCount(); i++)
						{
							hud->AddHealth();
						}
						break;
					case pwrAmmo:
						for(int i = 0; i < pwr->GetCount(); i++)
						{
							hud->AddAmmo();
						}
						break;
					case pwrScore:
						for(int i = 0; i < pwr->GetCount(); i++)
						{
							hud->AddScore();
						}
						break;
					}

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
		explosions[i]->SetMirror(player->GetMirror());


		if(!explosions[i]->GetMirror())
		{
			explosions[i]->SetX(player->GetX() + FLASH_DX_RIGHT);
		}
		else
		{
			explosions[i]->SetX(player->GetX() - FLASH_DX_LEFT);
		}

		int dy = FLASH_DY;
		if(player->GetJumping())
			//if(player->GetJumpDirection())
			//	dy += JUMP_DY;
			//else
			//	dy -= JUMP_DY;
			//
		explosions[i]->SetY(player->GetY() + FLASH_DY - JUMP_DY);
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


	if(current_tick - tick10 >= PROCESS_DELAY)
	{
			MoveBullets();
			MovePhrases();

		if(player)
			ProcessPlayer();

			ProcessEnemies();
			ProcessBullets();
		tick10 = current_tick;
	}

	if(current_tick - tick100 >= MOVE_DELAY)
	{
			MovePowerups();

		if(player)
		{
			player->Move();

			//if(player->GetMoving())
			//{
			//	if(!player->GetMovingDirection())
			//		SetWorldX(GetWorldX() + cfg.hero_speed);
			//	else
			//		SetWorldX(GetWorldX() - cfg.hero_speed);

			//	for(vector<SPRITE*>::iterator it = objects.begin();
			//		it != objects.end();
			//		it++)
			//	{
			//		SPRITE* spr = *it;

			//		if(spr)
			//		{
			//			spr->SetX(spr->GetX() - GetWorldX());
			//		}
			//	}
			//}
		}


			MoveEnemies();
			MoveExplosions();

		tick100 = current_tick;
	}


		ProcessPowerups();
		ProcessExplosions();
		ProcessPhrases();
}

void IICHAN::DrawBackground()
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
}

void IICHAN::DrawBullets()
{
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
}

void IICHAN::DrawPowerups()
{
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
}

void IICHAN::DrawPlayer()
{
	if(player)
	{
		//char hud[64];

		//sprintf(hud, "Health: %d   Ammo: %d   Score: %d",
		//	player->GetHealth(), player->GetAmmo(), player->GetScore());

		glDisable(GL_BLEND);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		player->Draw();
		glEnable(GL_BLEND);

		//static font_t* font = FontByName("default");
		//static int hud_y = cfg.scr_height - HUD_DY;
		//static int hud_x = 10;
		//PrintText(hud, font, hud_x, hud_y);
	}
}

void IICHAN::DrawEnemies()
{
	if(enemies.size() > 0)
		for(vector<CHARACTER*>::iterator it = enemies.begin();
			it != enemies.end();
			it++)
		{
			CHARACTER* enemy = *it;

			if(enemy)
			{
				glDisable(GL_BLEND);

				switch(enemy->GetColor())
				{
				case clr1: glColor4f(0.5f, 0.4f, 0.0f, 1.0f); break;
				case clr2: glColor4f(0.4f, 6.0f, 0.0f, 1.0f); break;
				case clr3: glColor4f(0.8f, 0.9f, 0.0f, 1.0f); break;
				case clr4: glColor4f(1.0f, 1.0f, 0.0f, 1.0f); break;
				default:
				case clrNone: glColor4f(1.0f, 1.0f, 1.0f, 1.0f); break;
				}
				enemy->Draw();
				glEnable(GL_BLEND);
			}
		}
}

void IICHAN::DrawExplosions()
{
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
}

void IICHAN::DrawPhrases()
{
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



void IICHAN::Draw()
{
	DrawBackground();
	DrawPowerups();
	DrawBullets();
	DrawEnemies();
	DrawPlayer();
	DrawExplosions();
	DrawPhrases();
	if(hud)
		hud->Draw();
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

void IICHAN::CreateExplosion(int x, int y, bool reverse, bool mirror)
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
	spr->SetPatternCount(2);
	if(reverse)
		spr->SetStartPattern(2);
	else
		spr->SetStartPattern(0);
	spr->SetLooped(false);
	spr->SetMirror(mirror);
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
	enemy->SetRealPatternCount(11);
	int r = Random(10);
	enemy->SetWalkPattern(r);
	enemy->SetWalkCount(10);
	enemy->SetCurrentAnimation(animWalk, true);
	enemy->SetColor(clrNone);
	if(player)
		enemy->SetMirror(x > player->GetX());
	else
		enemy->SetMirror(true);
	
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

void IICHAN::CreateBullet(int x, int y, bool mirror)
{
	BULLET* bullet = new BULLET();
	bullet->SetTexture(TextureByName("bullet"));
	bullet->SetPatternWidth(64);
	bullet->SetPatternHeight(32);
	bullet->SetX(x);
	bullet->SetY(y);		
	bullet->SetLife(cfg.bullet_life);
	bullet->SetSpeed(cfg.bullet_speed);
	bullet->SetDamage(cfg.bullet_damage);
	bullet->SetMirror(mirror);

	AddBullet(bullet);
}

void IICHAN::Init()
{
	osl_Load_v_04("levels\\level1.txt");

	hud = new HUD();

	for(int i = 0; i < GetPlayer()->GetHealth(); i++)
	{
		hud->AddHealth();
	}

	for(int i = 0; i < GetPlayer()->GetAmmo(); i++)
	{
		hud->AddAmmo();
	}

	for(int i = 0; i < GetPlayer()->GetScore(); i++)
	{
		hud->AddScore();
	}

}

void IICHAN::DeletePowerups()
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
}

void IICHAN::DeleteBackground()
{
	UINT l = background.size();
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
}

void IICHAN::DeleteBullets()
{
	UINT l = bullets.size();
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
}

void IICHAN::DeletePlayer()
{
	if(player)
	{
		delete player;
		player = NULL;
	}
}

void IICHAN::DeleteExplosions()
{
	UINT l = explosions.size();
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
}

void IICHAN::DeleteEnemies()
{
	UINT l = enemies.size();
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
}

void IICHAN::DeletePhrases()
{
	UINT l = phrases.size();
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

void IICHAN::Destroy()
{
	DeleteBackground();
	DeletePowerups();
	DeleteBullets();
	DeleteExplosions();
	DeleteEnemies();
	DeletePhrases();
	DeletePlayer();
	hud->Destroy();
	//objects.clear();
}

void IICHAN::SetWorldMaxX(int value)
{
	WORLD_MAX_X = value;
}

void IICHAN::SetWorldMinX(int value)
{
	WORLD_MIN_X = value;
}