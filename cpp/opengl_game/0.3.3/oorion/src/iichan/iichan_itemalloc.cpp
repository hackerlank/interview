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
	/*ANIMATED*/SPRITE* sprite = item->GetSprite();
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

#define PLAYER_READY !player->GetShooting() && \
	!player_sprite->GetJumping() && \
	!player_sprite->GetMoving() && \
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

#define PLAYER_GET_POWERUP PointInBox(powerup_sprite->GetX() + powerup_sprite->GetPatternWidth() / 2, \
				powerup_sprite->GetY() + powerup_sprite->GetPatternHeight() / 2, \
				player_sprite->GetX(), player_sprite->GetY(), \
				player_sprite->GetPatternWidth() / 2, player_sprite->GetPatternHeight())

#define ENEMY_RIGHT enemy_sprite->GetX() > player_sprite->GetX()

// this is epic
#define ENEMY_HITTED PointInBox(bullet_sprite->GetMirror() ? bullet_sprite->GetX() + \
	bullet_sprite->GetRealX() : bullet_sprite->GetX() + bullet_sprite->GetRealX() + \
	bullet_sprite->GetRealWidth(), bullet_sprite->GetY(), enemy_sprite->GetX() + \
	enemy_sprite->GetRealX(), enemy_sprite->GetY() + enemy_sprite->GetRealY(), \
	enemy_sprite->GetRealWidth(), enemy_sprite->GetRealHeight())

// --------------------------------------------------------------------------


void IICHANItemAllocator::ProcessPlayer(ITEM* item)
{
	if(!item)
		return;
	ANIMATEDSPRITE* player_sprite = item->GetSprite();
	if(!player_sprite)
		return;
	PLAYER* player = (PLAYER*)item;
	if(PLAYER_READY)
		if(player_sprite->GetCurrentAnimation() != animStand)
			player_sprite->SetCurrentAnimation(animStand, true);

	if(PRESSED_FIRE)
	{
		if(!player->GetShooting())
		{
			if(player_sprite->GetJumping() || player_sprite->GetMoving())
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
					player_sprite->SetCurrentAnimation(animShoot, false);
				}
			}
		}
	}
	if(PRESSED_RIGHT)
	{
		player_sprite->SetMirror(false);
		if(!player_sprite->GetJumping())
		{
			player_sprite->SetMoving(true);
			player_sprite->SetMovingDirection(false);
		}

		if(PLAYER_NOT_OUT_OF_SCREEN)
			player_sprite->SetX(player_sprite->GetX() + (int)cfg.hero_speed);

		if(PLAYER_OUT_OF_SCREEN)
			WORLD_X += (int)cfg.hero_speed;
	}
	if(PRESSED_LEFT)
	{
		player_sprite->SetMirror(true);
		if(!player_sprite->GetJumping())
		{
			player_sprite->SetMoving(true);
			player_sprite->SetMovingDirection(true);
		}
		if(player_sprite->GetX() - (int)cfg.hero_speed >= 0)
			player_sprite->SetX(player_sprite->GetX() - (int)cfg.hero_speed);

		if((player_sprite->GetX() - WORLD_X - (int)cfg.hero_speed <= BORDER_MARGIN) &&
			(WORLD_X - (int)cfg.hero_speed >= WORLD_MIN_X))
			WORLD_X -= (int)cfg.hero_speed;
	}

	if(!PRESSED_LEFT && PLAYER_STILL_MOVING_LEFT)
	{
		player_sprite->SetMoving(false);
		if(player_sprite->GetCurrentAnimation() == animWalk)
			player_sprite->SetCurrentAnimation(animStand, true);
	}

	if(!PRESSED_RIGHT && PLAYER_STILL_MOVING_RIGHT)
	{
		player_sprite->SetMoving(false);
		if(player_sprite->GetCurrentAnimation() == animWalk)
			player_sprite->SetCurrentAnimation(animStand, true);
	}

	if(PRESSED_JUMP)
	{
		if(!player_sprite->GetJumping())
		{
			player_sprite->SetJumping(true);
			player_sprite->SetYSpeed(cfg.hero_speed);
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

	if(player_sprite->GetLanding())
	{
		player_sprite->SetCurrentAnimation(animLand, false);

		// here create puffs
		//
		//
		player_sprite->SetLanding(false);
	}

	if(player->GetShooting())
	{
		if(player->GetShoots() < cfg.hero_reload)
		{
			if(player->GetShoots() == 1)
			{
				player->Shoot();
				if(GetHUD())
					GetHUD()->RemoveAmmo();
				if(!player_sprite->GetMirror())
				{
					CreateExplosion(player_sprite->GetX() + FLASH_DX_RIGHT,
						player_sprite->GetY() + FLASH_DY,
						false,
						false);
					CreateBullet(player_sprite->GetX() + FLASH_DX_RIGHT,
						player_sprite->GetY() + FLASH_DY,
						false);
				}
				else
				{
					CreateExplosion(player_sprite->GetX() - FLASH_DX_LEFT,
						player_sprite->GetY() + FLASH_DY,
						false,
						true);
					CreateBullet(player_sprite->GetX() - FLASH_DX_LEFT,
						player_sprite->GetY() + FLASH_DY,
						true);
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

	if(powerup)
		powerup->Move();

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
				if(PLAYER_GET_POWERUP)
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

	if(enemy_sprite)
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
				if(ENEMY_RIGHT)
				{
					enemy_sprite->SetX(enemy_sprite->GetX() - cfg.enemy_speed);
					enemy_sprite->SetMirror(true);
				}
				else
				{
					enemy_sprite->SetX(enemy_sprite->GetX() + cfg.enemy_speed);
					enemy_sprite->SetMirror(false);
				}
		}
	}

	if(enemy)
	{
		if(enemy->GetHealth() <= 0)
		{
			enemy->SetDead(true);
			if(enemy_sprite)
				enemy_sprite->SetDead(true);
		}
	}
}

void IICHANItemAllocator::ProcessExplosion(ITEM* item)
{
	if(!item)
		return;
	EXPLOSION* explosion = (EXPLOSION*)item;
	ANIMATEDSPRITE* explosion_sprite = explosion->GetSprite();

	if(explosion)
		explosion->Move();

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
					explosion_sprite->SetY(player_sprite->GetY() + FLASH_DY - JUMP_DY);
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

	if(bullet)
		bullet->Move();

	if(bullet_sprite)
	{
		for(vector<int>::iterator it = _enemies.begin();
			it != _enemies.end();
			it++)
		{
			int id = *it;
			CHARACTER* enemy = (CHARACTER*)GetItem(id);
			if(!enemy->GetDead())
			{
				ANIMATEDSPRITE* enemy_sprite = NULL;
				if(enemy)
					enemy_sprite = enemy->GetSprite();

				if(enemy_sprite)
				{
					if(ENEMY_HITTED)
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
	int now_tick = GetTickCount();
	if(now_tick - _prev_tick >= PROCESS_DELAY)
	{
		for(UINT i = 0; i < items.size(); i++)
		{
			ITEM* item = items[i];

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
			
			if(item->GetDead())					
				DeleteItem(item);
		}
		_prev_tick = now_tick;
	}
}

void IICHANItemAllocator::CreatePowerup(OBJECT_RES* res, enum POWERUP_TYPE power_type,
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

void IICHANItemAllocator::CreateEnemy(OBJECT_RES* res, int x, int y, int health)
{
	CHARACTER* enemy = new CHARACTER();
	enemy->SetType(itemEnemy);
	enemy->SetHealth(health);
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	enemy->SetSprite(spr);
	spr->LoadFromResource(res);
	spr->SetX(x);
	spr->SetY(y);
	spr->SetCurrentPattern(spr->GetStartPattern());
	spr->SetCurrentAnimation(animWalk, true);
	spr->SetZ(DEFAULT_Z);
	AddItem(enemy);
}

void IICHANItemAllocator::CreateBullet(int x, int y, bool mirror)
{
	BULLET* bullet = new BULLET();
	bullet->SetLife(cfg.bullet_life);
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

ITEM* IICHANItemAllocator::GetFirstPlayer()
{
	if(_players.size() > 0)
		return GetItem(_players[0]);
	else
		return NULL;
}
