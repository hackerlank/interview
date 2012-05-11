#include "stdafx.h"
#include "iichan_itemalloc.h"
#include "../misc.h"
#include "../config.h"

#include "iichan_player.h"
#include "iichan_small_types.h"

extern config cfg;
extern bool keys[256];
extern int WORLD_X;
extern int WORLD_Y;
extern int WORLD_MAX_X;
extern int WORLD_MIN_X;
extern unsigned int current_time;

//bool keyboard_enabled = true;


void IICHANItemAllocator::AddItem(ITEM* item)
{
	item->SetId(_count);
	items.push_back(item);
	if(item->GetSprite())
		GetSpriteAllocator()->AddSprite(item->GetSprite());

	switch(item->GetType())
	{
	case itemPlayer: _players.push_back(item->GetId()); break;
	case itemEnemy: _enemies.push_back(item->GetId()); break;
	case itemPowerup: _powerups.push_back(item->GetId()); break;
	}
	_count++;
}

void IICHANItemAllocator::DeleteItem(ITEM* item)
{
	if(!item)
		return;

	// first of all, remove its id from cache
	switch(item->GetType())
	{
	case itemPlayer:
		_remove_cached_value(&_players, item->GetId());
		break;
	case itemEnemy:
		_remove_cached_value(&_enemies, item->GetId());
		break;
	case itemPowerup:
		_remove_cached_value(&_powerups, item->GetId());
		break;
	}

	// mark sprite as dead
	// (it will be free'd by SpriteAllocator later)
	SPRITE* sprite = item->GetSprite();
	if(sprite)
		sprite->SetDead(true);

	//delete item
	int id = item->GetId();
	id = GetIndexById(id);

	delete item;
	item = NULL;
	items.erase(items.begin() + id);
}

ITEM* IICHANItemAllocator::GetItem(int id)
{
	for(vector<ITEM*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		ITEM* item = *it;
		if(item)
			if(item->GetId() == id)
				return item;
	}
	return NULL;
}

int IICHANItemAllocator::GetIndexById(int id)
{
	UINT index = 0;
	for(vector<ITEM*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		ITEM* item = *it;
		if(item)
		{
			if(item->GetId() == id)
				return index;
		}
		index++;
	}
	return -1;
}

IICHANItemAllocator::IICHANItemAllocator()
{
	items.resize(0);
	_players.resize(0);
	_enemies.resize(0);
	_powerups.resize(0);
	_prev_tick = 0;
	_count = 0;
	SetHUD(NULL);
	SetSpriteAllocator(NULL);
	SetInstanceAllocator(NULL);
	SetScreenTextAllocator(NULL);
}

IICHANItemAllocator::~IICHANItemAllocator()
{
	for(vector<ITEM*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		ITEM* item = *it;
		if(item)
		{
			try
			{
				delete item;
			}
			catch(...)
			{
				WriteToLog("Exception in IICHANItemAllocator::~IICHANItemAllocator()");
			}
		}
	}
	items.clear();
}

// Some useful macro's to make more readable code
// --------------------------------------------------------------------------

#define PLAYER_READY !player_sprite->GetShooting() && \
	!player_sprite->GetJumping() && \
	!player_sprite->GetMoving() && \
	!player_sprite->GetSitting() && \
	!player_sprite->GetDamaging() && \
	!player_sprite->GetDying() && \
	player_sprite->GetDoneAnimation()

#define PLAYER_NOT_OUT_OF_SCREEN player_sprite->GetX() - WORLD_X + (int)cfg.hero_speed \
	<= cfg.scr_width - player_sprite->GetPatternWidth()

#define PLAYER_OUT_OF_SCREEN (player_sprite->GetX() - WORLD_X + (int)cfg.hero_speed >= \
	cfg.scr_width - BORDER_MARGIN) && (WORLD_X + cfg.scr_width + (int)cfg.hero_speed <= WORLD_MAX_X)

#define PLAYER_STILL_MOVING_LEFT (player_sprite->GetMoving() && player_sprite->GetMovingDirection() == true)
#define PLAYER_STILL_MOVING_RIGHT (player_sprite->GetMoving() && player_sprite->GetMovingDirection() == false)

#define PRESSED_FIRE keys[cfg.fire]
#define PRESSED_LEFT keys[cfg.left]
#define PRESSED_RIGHT keys[cfg.right]
#define PRESSED_JUMP keys[cfg.jump]
#define PRESSED_DOWN keys[cfg.down]


#define PLAYER_GET_POWERUP_LEFT PointInBox(powerup_sprite->GetX() + \
				powerup_sprite->GetPatternWidth(), \
				powerup_sprite->GetY() + powerup_sprite->GetPatternHeight() / 2, \
				player_sprite->GetX() + player_sprite->GetRealX(), \
				player_sprite->GetY() + player_sprite->GetRealY(), \
				player_sprite->GetRealWidth(), player_sprite->GetRealHeight())
#define PLAYER_GET_POWERUP_RIGHT PointInBox(powerup_sprite->GetX(), \
					powerup_sprite->GetY() + powerup_sprite->GetPatternHeight() / 2, \
					player_sprite->GetX() + player_sprite->GetRealX(), \
					player_sprite->GetY() + player_sprite->GetRealY(), \
					player_sprite->GetRealWidth(), player_sprite->GetRealHeight())

#define ENEMY_RIGHT (enemy_sprite->GetX() + enemy_sprite->GetRealX() > \
	player_sprite->GetX() + player_sprite->GetRealX() + player_sprite->GetRealWidth())
#define ENEMY_LEFT (enemy_sprite->GetX() + enemy_sprite->GetRealX() + enemy_sprite->GetRealWidth() < \
	player_sprite->GetX() + player_sprite->GetRealX())

#define ENEMY_ON_SCREEN (enemy_sprite->GetX() - WORLD_X >= 0 && \
	enemy_sprite->GetX() - WORLD_X <= cfg.scr_width)

#define BULLET_OUT_OF_SCREEN bullet_sprite->GetX() - WORLD_X + (int)cfg.bullet_speed >= cfg.scr_width \
	|| bullet_sprite->GetX() + WORLD_X <= 0

#define KEYBOARD_ENABLED GetInstanceAllocator()->GetKeyboardEnabled()

// --------------------------------------------------------------------------


void IICHANItemAllocator::ProcessPlayer(ITEM* item)
{
	assert(item != NULL);
	ANIMATEDSPRITE* player_sprite = item->GetSprite();
	assert(player_sprite != NULL);
	PLAYER* player = (PLAYER*)item;
	if(PLAYER_READY)
		if(player_sprite->GetCurrentAnimation() != animStand)
			player_sprite->SetCurrentAnimation(animStand, true);
	SHADOW* shadow = player->GetShadow();
	SPRITE* shadow_sprite = NULL;
	if(shadow)
		 shadow_sprite = shadow->GetSprite();

	assert(GetInstanceAllocator() != NULL);

	if(player_sprite->GetDying())
	{
		if(player_sprite->GetCurrentAnimation() != animDie)
			player_sprite->SetCurrentAnimation(animDie, false);

		if(player_sprite->GetDoneAnimation())
		{
			player->SetDead(true);
			if(shadow)
				shadow->SetDead(true);
		}
		return;
	}

	if(PRESSED_FIRE && KEYBOARD_ENABLED)
	{
		if(!player_sprite->GetShooting())
		{
			if(player_sprite->GetJumping() || player_sprite->GetMoving())
			{
				if(player->GetAmmo() > 0)
				{
					player_sprite->SetShooting(true);
				}
			}
			else		
			{
				if(player->GetAmmo() > 0)
				{
					player_sprite->SetShooting(true);
					if(player_sprite->GetSitting())
						player_sprite->SetCurrentAnimation(animSitShoot, false);
					else
						player_sprite->SetCurrentAnimation(animShoot, false);
				}
			}
		}
	}
	if(PRESSED_RIGHT && KEYBOARD_ENABLED)
	{
		player_sprite->SetMirror(false);
		if(!player_sprite->GetJumping())
		{
			player_sprite->SetMoving(true);
			player_sprite->SetMovingDirection(false);
		}

		if(PLAYER_NOT_OUT_OF_SCREEN)
		{
			player_sprite->SetX(player_sprite->GetX() + cfg.hero_speed);
			if(shadow_sprite)
				shadow_sprite->SetX(shadow_sprite->GetX() + cfg.hero_speed);
		}


		if(PLAYER_OUT_OF_SCREEN)
			WORLD_X += (int)cfg.hero_speed;
	}
	if(PRESSED_LEFT && KEYBOARD_ENABLED)
	{
		player_sprite->SetMirror(true);
		if(!player_sprite->GetJumping())
		{
			player_sprite->SetMoving(true);
			player_sprite->SetMovingDirection(true);
		}
		if(player_sprite->GetX() - (int)cfg.hero_speed >= 0)
		{
			player_sprite->SetX(player_sprite->GetX() - cfg.hero_speed);
			if(shadow_sprite)
				shadow_sprite->SetX(shadow_sprite->GetX() - cfg.hero_speed);
		}

		if((player_sprite->GetX() - WORLD_X - (int)cfg.hero_speed <= BORDER_MARGIN) &&
			(WORLD_X - (int)cfg.hero_speed >= WORLD_MIN_X))
			WORLD_X -= (int)cfg.hero_speed;
	}

	if((!PRESSED_LEFT && PLAYER_STILL_MOVING_LEFT) || !KEYBOARD_ENABLED)
	{
		player_sprite->SetMoving(false);
		if(player_sprite->GetCurrentAnimation() == animWalk)
			player_sprite->SetCurrentAnimation(animStand, true);
	}

	if((!PRESSED_RIGHT && PLAYER_STILL_MOVING_RIGHT) || !KEYBOARD_ENABLED)
	{
		player_sprite->SetMoving(false);
		if(player_sprite->GetCurrentAnimation() == animWalk)
			player_sprite->SetCurrentAnimation(animStand, true);
	}

	if(PRESSED_JUMP && KEYBOARD_ENABLED)
	{
		if(!player_sprite->GetJumping())
		{
			player_sprite->SetJumping(true);
			player_sprite->SetYSpeed((float)cfg.hero_speed);
		}
	}

	if((!PRESSED_DOWN && player_sprite->GetSitting()) || !KEYBOARD_ENABLED)
	{
		player_sprite->SetSitting(false);
		if(player_sprite->GetCurrentAnimation() == animSit)
			player_sprite->SetCurrentAnimation(animStand, true);
	}

	if(PRESSED_DOWN && KEYBOARD_ENABLED)
	{
		if(!player_sprite->GetJumping() && !player_sprite->GetMoving() &&
			!player_sprite->GetShooting())
		{
			player_sprite->SetSitting(true);
			if(player_sprite->GetCurrentAnimation() != animSit)
				player_sprite->SetCurrentAnimation(animSit, true);
		}
	}

		
	if(player_sprite->GetJumping())
	{
		if(player_sprite->GetMoving())
			player_sprite->SetMoving(false);
			player_sprite->SetCurrentAnimation(animJump, false);
			static float ay = - (cfg.gravity * PROCESS_DELAY / 1000);
			player_sprite->SetYSpeed(player_sprite->GetYSpeed() + ay);
			player_sprite->SetJumpDirection(player_sprite->GetYSpeed() < 0);
			if(!player_sprite->GetJumpDirection())
				player_sprite->SetY(int(player_sprite->GetY() - player_sprite->GetYSpeed()));
			else
			{
				if(player_sprite->GetY() + player_sprite->GetYSpeed() < player_sprite->GetYStart())
					player_sprite->SetY(int(player_sprite->GetY() - player_sprite->GetYSpeed()));
				else
				{
					player_sprite->SetY(player_sprite->GetYStart());
					player_sprite->SetJumping(false);
					player_sprite->SetLanding(true);
				}
			}
	}

	if(!player->GetVulnerable())
	{
		if(player_sprite->GetDoneAnimation())
			player_sprite->SetDamaging(false);

		if(player->GetVulnTicks() < cfg.hero_vuln)
		{
			player->SetVulnTicks(player->GetVulnTicks() + 1);
			player_sprite->SetVisible(!player_sprite->GetVisible());
		}
		else
		{
			player_sprite->SetVisible(true);
			player->SetVulnerable(true);
		}
	}

	if(player_sprite->GetDamaging() && player->GetVulnerable())
	{
		if(player_sprite->GetCurrentAnimation() != animDamage)
			player_sprite->SetCurrentAnimation(animDamage, false);
		player->SetVulnerable(false);
		player->SetVulnTicks(0);
	}

	if(player_sprite->GetLanding())
	{
		player_sprite->SetCurrentAnimation(animLand, false);
		player_sprite->SetLanding(false);
	}

	if(player_sprite->GetShooting())
	{
		if(player->GetShoots() < cfg.hero_reload)
		{
			if(player->GetShoots() == 1)
			{
				player->Shoot();
				if(GetHUD())
					GetHUD()->RemoveAmmo();

				int y = player_sprite->GetY() + FLASH_DY;

				if(player_sprite->GetSitting())
					y += FLASH_SIT_DY;
				if(player_sprite->GetMoving())
					y = player_sprite->GetY() + FLASH_DY + FLASH_MOVE_DY;

				if(!player_sprite->GetMirror())
				{
					CreateExplosion(player_sprite->GetX() + FLASH_DX_RIGHT,
						y,
						false,
						false);
					CreateBullet(player_sprite->GetX() + FLASH_DX_RIGHT,
						y,
						false);
				}
				else
				{
					CreateExplosion(player_sprite->GetX() - FLASH_DX_LEFT,
						y,
						false,
						true);
					CreateBullet(player_sprite->GetX() - FLASH_DX_LEFT,
						y,
						true);
				}
			}
			player->SetShoots(player->GetShoots() + 1);
		}
		else
		{
			player_sprite->SetShooting(false);
			player->SetShoots(0);
		}
	}

	if(player_sprite->GetMoving())
	{
		player_sprite->SetCurrentAnimation(animWalk, true);
	}
}

void IICHANItemAllocator::ProcessPowerup(ITEM* item)
{
	if(!item)
		return;
	POWERUP* powerup = (POWERUP*)item;
	ANIMATEDSPRITE* powerup_sprite = powerup->GetSprite();

	if(powerup_sprite)
	{
		for(vector<int>::iterator it = _players.begin();
			it != _players.end();
			it++)
		{
			int id = *it;
			PLAYER* player = (PLAYER*)GetItem(id);
			ANIMATEDSPRITE* player_sprite = NULL;
			if(player)
				player_sprite = player->GetSprite();

			if(player_sprite)
				if(PLAYER_GET_POWERUP_LEFT || PLAYER_GET_POWERUP_RIGHT)
				{
					player->CollectPowerup(powerup->GetPowerType(), powerup->GetCount());

					switch(powerup->GetPowerType())
					{
					case pwrHealth:
						for(int i = 0; i < powerup->GetCount(); i++)
						{
							GetHUD()->AddHealth();
						}
						break;
					case pwrAmmo:
						for(int i = 0; i < powerup->GetCount(); i++)
						{
							GetHUD()->AddAmmo();
						}
						break;
					case pwrScore:
						for(int i = 0; i < powerup->GetCount(); i++)
						{
							GetHUD()->AddScore();
						}
						break;
					}

					item->SetDead(true);
				}
		}
	}
}

void IICHANItemAllocator::ProcessEnemy(ITEM* item)
{
	if(!item)
		return;

	CHARACTER* enemy = (CHARACTER*)item;
	ANIMATEDSPRITE* enemy_sprite = enemy->GetSprite();
	SHADOW* shadow = enemy->GetShadow();
	SPRITE* shadow_sprite = NULL;
	if(shadow)
		shadow_sprite = shadow->GetSprite();

	if(enemy_sprite)
	{
		if(GetInstanceAllocator()->GetAI() && enemy->GetAI() == aiOn)
		{
			for(vector<int>::iterator it = _players.begin();
				it != _players.end();
				it++)
			{
				int id = *it;
				PLAYER* player = (PLAYER*)GetItem(id);
				ANIMATEDSPRITE* player_sprite = NULL;

				if(player)
					player_sprite = player->GetSprite();
				if(player_sprite)
				{
					if(ENEMY_RIGHT && ENEMY_ON_SCREEN)
					{
						enemy_sprite->SetCurrentAnimation(animWalk, true);
						enemy_sprite->SetX(enemy_sprite->GetX() - cfg.enemy_speed);
						enemy_sprite->SetMirror(true);
						if(shadow_sprite)
							shadow_sprite->SetX(shadow_sprite->GetX() - cfg.enemy_speed);
					}
					else if(ENEMY_LEFT && ENEMY_ON_SCREEN)
					{
						enemy_sprite->SetCurrentAnimation(animWalk, true);
						enemy_sprite->SetX(enemy_sprite->GetX() + cfg.enemy_speed);
						enemy_sprite->SetMirror(false);
						if(shadow_sprite)
							shadow_sprite->SetX(shadow_sprite->GetX() + cfg.enemy_speed);
					}
					else
					{
						if(enemy_sprite->GetCurrentAnimation() != animStand)
							enemy_sprite->SetCurrentAnimation(animStand, true);
					}

					RECT intersect;
					RECT player_rect;
					RECT enemy_rect;

					player_sprite->GetMirror() ? player_rect = player_sprite->GetMirrorCollideRect() :
						player_rect = player_sprite->GetCollideRect();

					enemy_sprite->GetMirror() ? enemy_rect = enemy_sprite->GetMirrorCollideRect() :
						enemy_rect = enemy_sprite->GetCollideRect();

					if(IntersectRect(&intersect, &player_rect, &enemy_rect) &&
						player->GetVulnerable() &&
						!player_sprite->GetDamaging())
					{
						if(player->GetHealth() > 0)
						{
							player_sprite->SetDamaging(true);

							//int new_x;

							//if(enemy_sprite->GetMirror())
							//	new_x = enemy_sprite->GetX() + enemy_sprite->GetRealX() -
							//	DAMAGE_DX - player_sprite->GetRealWidth();
							//else
							//	new_x = enemy_sprite->GetX() + enemy_sprite->GetRealX() +
							//	enemy_sprite->GetRealWidth() + DAMAGE_DX;

							//if(new_x >= WORLD_MIN_X && new_x <= WORLD_MAX_X)
							//	player_sprite->SetX(new_x);

							player->Ouch();
							GetHUD()->RemoveHealth();
						}
						else
						{
							player_sprite->SetDying(true);
						}
					}
				}
			}
		}
		else
			enemy_sprite->SetCurrentAnimation(animStand, true);
	}

	if(enemy)
	{
		if(enemy->GetHealth() <= 0)
		{
			enemy->SetDead(true);
			if(shadow)
				shadow->SetDead(true);
		}
	}
}

void IICHANItemAllocator::ProcessExplosion(ITEM* item)
{
	if(!item)
		return;
	EXPLOSION* explosion = (EXPLOSION*)item;
	ANIMATEDSPRITE* explosion_sprite = explosion->GetSprite();

	if(explosion_sprite)
	{
		for(vector<int>::iterator it = _players.begin();
			it != _players.end();
			it++)
		{
			int id = *it;
			PLAYER* player = (PLAYER*)GetItem(id);
			ANIMATEDSPRITE* player_sprite = NULL;
			if(player)
				player_sprite = player->GetSprite();

			if(player_sprite)
			{
				explosion_sprite->SetMirror(player_sprite->GetMirror());
				
				if(explosion_sprite->GetMirror())
					explosion_sprite->SetX(player_sprite->GetX() - FLASH_DX_LEFT);
				else
					explosion_sprite->SetX(player_sprite->GetX() + FLASH_DX_RIGHT);

				int dy = FLASH_DY;
				if(player_sprite->GetJumping())
					explosion_sprite->SetY(player_sprite->GetY() + FLASH_DY - FLASH_JUMP_DY);
			}
		}

		if(explosion_sprite->GetDoneAnimation())
			item->SetDead(true);
	}
}

void IICHANItemAllocator::ProcessBullet(ITEM* item)
{
	if(!item)
		return;

	BULLET* bullet = (BULLET*)item;
	SPRITE* bullet_sprite = (SPRITE*)bullet->GetSprite();
	if(!bullet_sprite)
		return;

	if(BULLET_OUT_OF_SCREEN)
	{
		bullet->SetDead(true);
		return;
	}

	if(bullet_sprite)
	{
		for(vector<int>::iterator it = _enemies.begin();
			it != _enemies.end();
			it++)
		{
			int id = *it;
			CHARACTER* enemy = (CHARACTER*)GetItem(id);
			if(!enemy->IsDead())
			{
				ANIMATEDSPRITE* enemy_sprite = NULL;
				if(enemy)
					enemy_sprite = enemy->GetSprite();

				if(enemy_sprite)
				{
					RECT bullet_rect;
					RECT enemy_rect;
					RECT intersect;
					bullet_sprite->GetMirror() ? bullet_rect = bullet_sprite->GetMirrorCollideRect() :
						bullet_rect = bullet_sprite->GetCollideRect();
					enemy_sprite->GetMirror() ? enemy_rect = enemy_sprite->GetMirrorCollideRect() :
						enemy_rect = enemy_sprite->GetCollideRect();
					if(IntersectRect(&intersect, &bullet_rect, &enemy_rect))
					{
						enemy->SetHealth(enemy->GetHealth() - cfg.bullet_damage);
						item->SetDead(true);
					}
				}
			}
		}
	}
}

void IICHANItemAllocator::Move()
{
	int now_tick = current_time;
	if(now_tick - _prev_tick >= PROCESS_DELAY)
	{
		for(UINT i = 0; i < items.size(); i++)
		{
			ITEM* item = items[i];

			item->Move();

			switch(item->GetType())
			{
			case itemPlayer: // PROCESSING PLAYER
				{
					ProcessPlayer(item);
					break;
				}
			case itemEnemy: // PROCESSING ENEMY
				{
					ProcessEnemy(item);
					break;
				}
			case itemPowerup: // PROCESSING POWERUP
				{
					ProcessPowerup(item);
					break;
				}
			case itemExplosion: // PROCESSING EXPLOSION
				{
					ProcessExplosion(item);
					break;
				}
			case itemBullet: // PROCESSING BULLET
				{
					ProcessBullet(item);
					break;
				}

			//case itemSprite: // PROCESSING OTHER SPRITE
			//	{
			//		ProcessSprite(item);
			//		break;
			//	}
			}
			
			if(item->IsDead())					
				DeleteItem(item);
		}
		_prev_tick = now_tick;
	}
}

void IICHANItemAllocator::CreatePlayer(RESOURCE* res, int health, int ammo, int score,
									   int x, int y)
{
	PLAYER* player = new PLAYER(health, ammo, score);

	ANIMATEDSPRITE* player_sprite = new ANIMATEDSPRITE();
	player->SetSprite(player_sprite);
	player_sprite->SetX(x);
	player_sprite->SetY(y);
	player_sprite->SetYStart(y);
	player_sprite->LoadFromResource(res);
	player_sprite->SetCurrentAnimation(animStand, true);
	player_sprite->SetZ(PLAYER_Z);
	AddItem(player);

	SHADOW* shadow = new SHADOW();
	player->SetShadow(shadow);
	RESOURCE* resource = ResourceByName("shadow");
	if(resource)
	{
		SPRITE* shadow_sprite = new SPRITE();
		shadow_sprite->LoadFromResource(resource);
		shadow_sprite->SetX(player_sprite->GetX() + player_sprite->GetShadowX());
		shadow_sprite->SetY(player_sprite->GetY() + player_sprite->GetShadowY());
		shadow_sprite->SetZ(SHADOW_Z);
		shadow_sprite->SetCollidable(false);
		shadow->SetSprite((ANIMATEDSPRITE*)shadow_sprite);
	}

	AddItem(shadow);
}

void IICHANItemAllocator::CreateEnemy(RESOURCE* res, int x, int y, int health)
{
	CHARACTER* enemy = new CHARACTER();
	enemy->SetType(itemEnemy);
	enemy->SetHealth(health);
	ANIMATEDSPRITE* enemy_sprite = new ANIMATEDSPRITE();
	enemy->SetSprite(enemy_sprite);
	enemy_sprite->LoadFromResource(res);
	enemy_sprite->SetX(x);
	enemy_sprite->SetY(y);
	enemy_sprite->SetCurrentAnimation(animStand, true);
	enemy_sprite->SetZ(PLAYER_Z);
	enemy_sprite->SetMirror(true);
	AddItem(enemy);

	SHADOW* shadow = new SHADOW();
	enemy->SetShadow(shadow);
	RESOURCE* resource = ResourceByName("shadow");
	if(resource)
	{
		SPRITE* shadow_sprite = new SPRITE();
		shadow_sprite->LoadFromResource(resource);
		shadow_sprite->SetX(enemy_sprite->GetX() + enemy_sprite->GetShadowX());
		shadow_sprite->SetY(enemy_sprite->GetY() + enemy_sprite->GetShadowY());
		shadow_sprite->SetZ(SHADOW_Z);
		shadow_sprite->SetCollidable(false);
		shadow->SetSprite((ANIMATEDSPRITE*)shadow_sprite);
	}

	AddItem(shadow);
}

void IICHANItemAllocator::CreatePowerup(RESOURCE* res, enum POWERUP_TYPE power_type,
										int count, int x, int y)
{
	POWERUP* pwr = new POWERUP(power_type, count);
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	spr->LoadFromResource(res);
	pwr->SetSprite(spr);
	spr->SetX(x);
	spr->SetY(y);
	spr->SetCurrentPattern(spr->GetStartPattern());
	spr->SetZ(DEFAULT_Z);
	AddItem(pwr);
}

void IICHANItemAllocator::CreateBullet(int x, int y, bool mirror)
{
	BULLET* bullet = new BULLET();
	bullet->SetSpeed(cfg.bullet_speed);
	bullet->SetDamage(cfg.bullet_damage);
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	bullet->SetSprite(spr);
	spr->LoadFromResource(ResourceByName("bullet"));
	spr->SetX(x);
	spr->SetY(y);		
	spr->SetMirror(mirror);
	AddItem(bullet);
}

void IICHANItemAllocator::CreateExplosion(int x, int y, bool reverse, bool mirror)
{
	EXPLOSION* exp = new EXPLOSION();
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	spr->LoadFromResource(ResourceByName("explosion"));
	spr->SetZ(TOP_Z);
	exp->SetSprite(spr);
	spr->SetDoneAnimation(false);
	spr->SetCollidable(false);
	spr->SetX(x);
	spr->SetY(y);
	spr->SetReverse(reverse);
	if(reverse)
		spr->SetStartPattern(2);
	else
		spr->SetStartPattern(0);
	spr->SetLooped(false);
	spr->SetMirror(mirror);
	spr->SetDead(false);

	AddItem(exp);
}

void IICHANItemAllocator::CreateSprite(RESOURCE* res, int x, int y)
{
	SPRITE* spr = new SPRITE();
	spr->SetX(x);
	spr->SetY(y);
	spr->SetCollidable(false);
	spr->LoadFromResource(res);

	ITEM* item = NULL;
	item = new ITEM();
	item->SetSprite((ANIMATEDSPRITE*)spr);
	AddItem(item);
}

ITEM* IICHANItemAllocator::GetFirstPlayer()
{
	if(_players.size() > 0)
		return GetItem(_players[0]);
	else
		return NULL;
}
