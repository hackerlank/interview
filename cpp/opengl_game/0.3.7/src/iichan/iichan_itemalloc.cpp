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


// --------------------------
void PositionExplosion(ANIMATEDSPRITE* player_sprite, int& flash_x, int& flash_y)
{
	if(player_sprite->GetStanding())
	{
		if(!player_sprite->GetMirror())
		{
			flash_x = player_sprite->GetFlashStandX();
			flash_y = player_sprite->GetFlashStandY();
		}
		else
		{
			flash_x = player_sprite->GetFlashStandMirrorX();
			flash_y = player_sprite->GetFlashStandMirrorY();
		}
	}
	if(player_sprite->GetSitting())
	{
		if(player_sprite->GetCurrentAnimation() != animSitShoot)
		{
			// first time after start shooting
			player_sprite->SetCurrentAnimation(animSitShoot, false);
			player_sprite->SetSitting(false);
		}
		if(!player_sprite->GetMirror())
		{
			flash_x = player_sprite->GetFlashSitX();
			flash_y = player_sprite->GetFlashSitY();
		}
		else
		{
			flash_x = player_sprite->GetFlashSitMirrorX();
			flash_y = player_sprite->GetFlashSitMirrorY();
		}
	}
	if(player_sprite->GetWalking())
	{
		if(!player_sprite->GetMirror())
		{
			flash_x = player_sprite->GetFlashWalkX();
			flash_y = player_sprite->GetFlashWalkY();
		}
		else
		{
			flash_x = player_sprite->GetFlashWalkMirrorX();
			flash_y = player_sprite->GetFlashWalkMirrorY();
		}
	}
	if(player_sprite->GetJumping())
	{
		if(!player_sprite->GetMirror())
		{
			flash_x = player_sprite->GetFlashJumpX();
			flash_y = player_sprite->GetFlashJumpY();
		}
		else
		{
			flash_x = player_sprite->GetFlashJumpMirrorX();
			flash_y = player_sprite->GetFlashJumpMirrorY();
		}
	}
	if(!player_sprite->GetMirror())
	{
		flash_x += player_sprite->GetX() + player_sprite->GetRealX();
		flash_y += player_sprite->GetY() + player_sprite->GetRealY();
	}
	else
	{
		flash_x += player_sprite->GetX() + player_sprite->GetRealMirrorX();
		flash_y += player_sprite->GetY() + player_sprite->GetRealMirrorY();
	}
}



// ---------------------------

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

#define PLAYER_READY  /*!player_sprite->GetShooting() &&*/ \
	!player_sprite->GetJumping() && \
	!player_sprite->GetWalking() && \
	!player_sprite->GetSitting() && \
	!player_sprite->GetDamaging() &&\
	!player_sprite->GetDying() && \
	!player_sprite->GetLanding() && \
	player_sprite->GetDoneAnimation()

#define WORLD_END player_sprite->GetX() - WORLD_X <= cfg.scr_width / 2
#define WORLD_NOT_END WORLD_X >= WORLD_MIN_X && WORLD_X <= WORLD_MAX_X

#define PLAYER_NOT_OUT_OF_SCREEN player_sprite->GetX() - WORLD_X + player_sprite->GetSpeedX() \
	<= cfg.scr_width - player_sprite->GetPatternWidth()

#define PLAYER_OUT_OF_SCREEN (player_sprite->GetX() - WORLD_X + player_sprite->GetSpeedX() >= \
	cfg.scr_width - BORDER_MARGIN) && (WORLD_X + cfg.scr_width + player_sprite->GetSpeedX() <= WORLD_MAX_X)

#define PLAYER_STILL_MOVING_LEFT (player_sprite->GetWalking() && player_sprite->GetWalkingDirection() == true)
#define PLAYER_STILL_MOVING_RIGHT (player_sprite->GetWalking() && player_sprite->GetWalkingDirection() == false)

#define PRESSED_FIRE keys[cfg.fire]
#define PRESSED_LEFT keys[cfg.left]
#define PRESSED_RIGHT keys[cfg.right]
#define PRESSED_JUMP keys[cfg.jump]
#define PRESSED_DOWN keys[cfg.down]


#define ENEMY_RIGHT (enemy_sprite->GetX() + enemy_sprite->GetRealX() + enemy_sprite->GetRealWidth()/2 > \
	player_sprite->GetX() + player_sprite->GetRealX() + player_sprite->GetRealWidth())
#define ENEMY_LEFT (enemy_sprite->GetX() + enemy_sprite->GetRealX() + enemy_sprite->GetRealWidth()/2 < \
	player_sprite->GetX() + player_sprite->GetRealX())

#define ENEMY_ON_SCREEN (enemy_sprite->GetX() + enemy_sprite->GetRealWidth() - WORLD_X >= -player_sprite->GetPatternWidth() && \
	enemy_sprite->GetX() - WORLD_X <= cfg.scr_width + enemy_sprite->GetPatternWidth())

#define BULLET_OUT_OF_SCREEN bullet_sprite->GetX() - WORLD_X + (int)cfg.bullet_speed >= cfg.scr_width \
	|| bullet_sprite->GetX() + WORLD_X <= 0

#define KEYBOARD_ENABLED GetInstanceAllocator()->GetKeyboardEnabled()

// --------------------------------------------------------------------------

void IICHANItemAllocator::ProcessPlayer(ITEM* item)
{
	assert(item != NULL);
	PLAYER* player = (PLAYER*)item;

	ANIMATEDSPRITE* player_sprite = item->GetSprite();
	assert(player_sprite != NULL);

	SHADOW* shadow = player->GetShadow();
	SPRITE* shadow_sprite = NULL;
	if(shadow) shadow_sprite = shadow->GetSprite();

	if(player_sprite->GetDying())
	{
		player_sprite->SetWalking(false);
		player_sprite->SetLanding(false);
		if(player_sprite->GetJumping())
		{
			player_sprite->SetY(player_sprite->GetYStart());
			player_sprite->SetJumping(false);
		}
		if(player_sprite->GetCurrentAnimation() != animDie)
		{
			// this is first time after start dying
			player_sprite->SetCurrentAnimation(animDie, false);
		}
		else if(player_sprite->GetDoneAnimation())
		{
			// now we are dead
			player->SetDead(true);
			if(shadow)
				shadow->SetDead(true);
		}
		return;
	}
	else if(player_sprite->GetDamaging())
	{
		if(player->GetVulnerable())
		{
			// this is first moment after damage
			// setting invulnerability
			player->SetVulnerable(false);
			player->SetVulnTicks(0);
			//if(!player_sprite->GetJumping())
			//	player_sprite->SetCurrentAnimation(animDamage, false);
		}
		else
		{
			// this is invulnerable period
			if(player->GetVulnTicks() < cfg.hero_vuln)
			{
				player->SetVulnTicks(player->GetVulnTicks() + 1);
				player_sprite->SetVisible(!player_sprite->GetVisible());
			}
			else
			{
				// end of invulnerable period
				// setting vulnerability and making sprite visible
				player->SetVulnerable(true);
				player_sprite->SetVisible(true);
				// setting off Damaging()
				player_sprite->SetDamaging(false);
			}
		}
	}
	else if(player_sprite->GetSitting())
	{
		if((player_sprite->GetCurrentAnimation() != animSit &&
			player_sprite->GetCurrentAnimation() != animSitShoot) ||
			player_sprite->GetDoneAnimation())
		{
			// first time after start sitting
			player_sprite->SetCurrentAnimation(animSit, true);
		}
	}
	else if(player_sprite->GetWalking() && !player_sprite->GetJumping() && !player_sprite->GetLanding())
	{
		if(player_sprite->GetCurrentAnimation() != animWalk)
		{
			// first time
			player_sprite->SetCurrentAnimation(animWalk, true);
		}
	}
	else if(player_sprite->GetIdling())
	{
		if(player_sprite->GetCurrentAnimation() != animIdle)
		{
			// first time
			player_sprite->SetCurrentAnimation(animIdle, false);
		}
		else
		{
			if(player_sprite->GetDoneAnimation())
			{
				player_sprite->SetIdling(false);
			}
		}
	}
	if(player_sprite->GetLanding())
	{
		if(player_sprite->GetCurrentAnimation() != animLand)
		{
			// first time after start landing
			player_sprite->SetCurrentAnimation(animLand, false);
		}
		else
		{
			// landing process
			if(player_sprite->GetDoneAnimation())
			{
				// here we can create puffs later
				player_sprite->SetLanding(false);
			}
		}
	}
	if(player_sprite->GetJumping())
	{
		// this is y-accelerate of player
		static const float ay = - (cfg.gravity * TICK / 1000);

		if(player_sprite->GetCurrentAnimation() != animJump)
		{
			// this is first time after jump
			player_sprite->SetCurrentAnimation(animJump, false);
			// moving up
			player_sprite->SetJumpDirection(false);
			// remembering start position
			player_sprite->SetYStart(player_sprite->GetY());

			// initting start speed
			static float start_speed_y = player_sprite->GetSpeedY();
			player_sprite->SetSpeedY(start_speed_y);

		}
		else
		{
			// jump process
			player_sprite->SetSpeedY(player_sprite->GetSpeedY() + ay);
			// moving up or down?
			player_sprite->SetJumpDirection(player_sprite->GetSpeedY() < 0);
			if(!player_sprite->GetJumpDirection())
			{
				// if moving up
				player_sprite->SetY(int(player_sprite->GetY() - player_sprite->GetSpeedY()));
			}
			else
			{
				// moving down
				// we cannot pass under our start position
				// LATER TODO: Fix with real world borders
				if(player_sprite->GetY() + player_sprite->GetSpeedY() < player_sprite->GetYStart())
				{
					player_sprite->SetY(player_sprite->GetY() - (int)player_sprite->GetSpeedY());
				}
				else
				{
					// position at start y-coord
					player_sprite->SetY(player_sprite->GetYStart());
					player_sprite->SetJumping(false);
					player_sprite->SetLanding(true);
				}
			}
		}
	}
	if(player_sprite->GetShooting())
	{
		if(player->GetShoots() == 0)
		{
			// TODO:
			// different animations of shoot
			// for sit, jump, etc

			int flash_x;
			int flash_y;

			PositionExplosion(player_sprite, flash_x, flash_y);

			player->Shoot();
			if(GetHUD())
				GetHUD()->RemoveAmmo();

			CreateExplosion(flash_x, flash_y, false, player_sprite->GetMirror());
			CreateBullet(flash_x, flash_y, player_sprite->GetMirror());
			player->SetShoots(1);
		}
		else
		{
			if(player->GetShoots() < cfg.hero_reload)
			{
				player->SetShoots(player->GetShoots() + 1);
			}
			else
			{
				player_sprite->SetShooting(false);
				player->SetShoots(0);
			}
		}
	}


	// Processing keyboard
	// ----------------------------------------------------------------------

	if(KEYBOARD_ENABLED)
	{
		if(PRESSED_FIRE)
		{
			if(player->GetAmmo() > 0)
				player_sprite->SetShooting(true);
		}
		if(PRESSED_DOWN && !player_sprite->GetJumping())
		{
			player_sprite->SetSitting(true);
		}
		if(PRESSED_JUMP && !player_sprite->GetLanding())
		{
			player_sprite->SetJumping(true);
			player_sprite->SetSitting(false);
		}
		if(PRESSED_RIGHT)
		{
			player_sprite->SetMirror(false);
			player_sprite->SetWalking(true);
			player_sprite->SetSitting(false);
			player_sprite->SetWalkingDirection(false);
		}
		if(PRESSED_LEFT)
		{
			player_sprite->SetMirror(true);
			player_sprite->SetWalking(true);
			player_sprite->SetSitting(false);
			player_sprite->SetWalkingDirection(true);
		}
	}

	if((!PRESSED_LEFT && PLAYER_STILL_MOVING_LEFT) ||
		(!PRESSED_RIGHT && PLAYER_STILL_MOVING_RIGHT) ||
		!KEYBOARD_ENABLED)
	{
		player_sprite->SetWalking(false);
		player_sprite->SetDoneAnimation(true);
	}
	if(!PRESSED_DOWN && player_sprite->GetSitting() || !KEYBOARD_ENABLED)
	{
		player_sprite->SetSitting(false);
		player_sprite->SetDoneAnimation(true);
	}

	/*if(!player->GetVulnerable())
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
	}*/

	if(PLAYER_READY)
	{
		if(player_sprite->GetCurrentAnimation() != animStand)
		{
			// first time
			player_sprite->SetCurrentAnimation(animStand, true);
		}
	}
	//assert(item != NULL);
	//ANIMATEDSPRITE* player_sprite = item->GetSprite();
	//assert(player_sprite != NULL);
	//PLAYER* player = (PLAYER*)item;
	//if(PLAYER_READY)
	//	if(player_sprite->GetCurrentAnimation() != animStand)
	//		player_sprite->SetCurrentAnimation(animStand, true);
	//SHADOW* shadow = player->GetShadow();
	//SPRITE* shadow_sprite = NULL;
	//if(shadow)
	//	 shadow_sprite = shadow->GetSprite();

	//assert(GetInstanceAllocator() != NULL);

	//if(player_sprite->GetDying())
	//{
	//	if(player_sprite->GetCurrentAnimation() != animDie)
	//		player_sprite->SetCurrentAnimation(animDie, false);

	//	if(player_sprite->GetDoneAnimation())
	//	{
	//		player->SetDead(true);
	//		if(shadow)
	//			shadow->SetDead(true);
	//	}
	//	return;
	//}

	//if(PRESSED_FIRE && KEYBOARD_ENABLED)
	//{
	//	if(!player_sprite->GetShooting())
	//	{
	//		if(player_sprite->GetJumping() || player_sprite->GetWalking())
	//		{
	//			if(player->GetAmmo() > 0)
	//			{
	//				player_sprite->SetShooting(true);
	//			}
	//		}
	//		else		
	//		{
	//			if(player->GetAmmo() > 0)
	//			{
	//				player_sprite->SetShooting(true);
	//				if(player_sprite->GetSitting())
	//					player_sprite->SetCurrentAnimation(animSitShoot, false);
	//				else
	//					player_sprite->SetCurrentAnimation(animShoot, false);
	//			}
	//		}
	//	}
	//}
	//if(PRESSED_RIGHT && KEYBOARD_ENABLED)
	//{
	//	player_sprite->SetMirror(false);
	//	if(!player_sprite->GetJumping())
	//	{
	//		player_sprite->SetWalking(true);
	//		player_sprite->SetWalkingDirection(false);
	//	}

	//	//if(PLAYER_NOT_OUT_OF_SCREEN)
	//	//{
	//	//	player_sprite->SetX(player_sprite->GetX() + cfg.hero_speed);
	//	//	if(shadow_sprite)
	//	//		shadow_sprite->SetX(shadow_sprite->GetX() + cfg.hero_speed);
	//	//}


	//	//if(PLAYER_OUT_OF_SCREEN)
	//	//	WORLD_X += (int)cfg.hero_speed;
	//}
	//if(PRESSED_LEFT && KEYBOARD_ENABLED)
	//{
	//	player_sprite->SetMirror(true);
	//	if(!player_sprite->GetJumping())
	//	{
	//		player_sprite->SetWalking(true);
	//		player_sprite->SetWalkingDirection(true);
	//	}
	//	//if(player_sprite->GetX() - (int)cfg.hero_speed >= 0)
	//	//{
	//	//	player_sprite->SetX(player_sprite->GetX() - cfg.hero_speed);
	//	//	if(shadow_sprite)
	//	//		shadow_sprite->SetX(shadow_sprite->GetX() - cfg.hero_speed);
	//	//}

	//	//if((player_sprite->GetX() - WORLD_X - (int)cfg.hero_speed <= BORDER_MARGIN) &&
	//	//	(WORLD_X - (int)cfg.hero_speed >= WORLD_MIN_X))
	//	//	WORLD_X -= (int)cfg.hero_speed;
	//}

	//if((!PRESSED_LEFT && PLAYER_STILL_MOVING_LEFT) || !KEYBOARD_ENABLED)
	//{
	//	player_sprite->SetWalking(false);
	//	if(player_sprite->GetCurrentAnimation() == animWalk)
	//		player_sprite->SetCurrentAnimation(animStand, true);
	//}

	//if((!PRESSED_RIGHT && PLAYER_STILL_MOVING_RIGHT) || !KEYBOARD_ENABLED)
	//{
	//	player_sprite->SetWalking(false);
	//	if(player_sprite->GetCurrentAnimation() == animWalk)
	//		player_sprite->SetCurrentAnimation(animStand, true);
	//}

	//if(PRESSED_JUMP && KEYBOARD_ENABLED)
	//{
	//	if(!player_sprite->GetJumping())
	//	{
	//		player_sprite->SetJumping(true);

	//		static float speed_y = player_sprite->GetSpeedY();
	//		player_sprite->SetSpeedY(speed_y);
	//	}
	//}

	//if((!PRESSED_DOWN && player_sprite->GetSitting()) || !KEYBOARD_ENABLED)
	//{
	//	player_sprite->SetSitting(false);
	//	if(player_sprite->GetCurrentAnimation() == animSit)
	//		player_sprite->SetCurrentAnimation(animStand, true);
	//}

	//if(PRESSED_DOWN && KEYBOARD_ENABLED)
	//{
	//	if(!player_sprite->GetJumping() && !player_sprite->GetWalking() &&
	//		!player_sprite->GetShooting())
	//	{
	//		player_sprite->SetSitting(true);
	//		if(player_sprite->GetCurrentAnimation() != animSit)
	//			player_sprite->SetCurrentAnimation(animSit, true);
	//	}
	//}

	//	
	//if(player_sprite->GetJumping())
	//{
	//	//if(player_sprite->GetWalking())
	//	//	player_sprite->SetWalking(false);
	//		player_sprite->SetCurrentAnimation(animJump, false);
	//		static float ay = - (cfg.gravity * TICK / 1000);
	//		player_sprite->SetSpeedY(player_sprite->GetSpeedY() + ay);
	//		player_sprite->SetJumpDirection(player_sprite->GetSpeedY() < 0);
	//		if(!player_sprite->GetJumpDirection())
	//			player_sprite->SetY(int(player_sprite->GetY() - player_sprite->GetSpeedY()));
	//		else
	//		{
	//			if(player_sprite->GetY() + player_sprite->GetSpeedY() < player_sprite->GetYStart())
	//				player_sprite->SetY(int(player_sprite->GetY() - player_sprite->GetSpeedY()));
	//			else
	//			{
	//				player_sprite->SetY(player_sprite->GetYStart());
	//				player_sprite->SetJumping(false);
	//				player_sprite->SetLanding(true);
	//			}
	//		}
	//}

	//if(!player->GetVulnerable())
	//{
	//	if(player_sprite->GetDoneAnimation())
	//		player_sprite->SetDamaging(false);

	//	if(player->GetVulnTicks() < cfg.hero_vuln)
	//	{
	//		player->SetVulnTicks(player->GetVulnTicks() + 1);
	//		player_sprite->SetVisible(!player_sprite->GetVisible());
	//	}
	//	else
	//	{
	//		player_sprite->SetVisible(true);
	//		player->SetVulnerable(true);
	//	}
	//}

	//if(player_sprite->GetDamaging() && player->GetVulnerable())
	//{
	//	if(player_sprite->GetCurrentAnimation() != animDamage)
	//		player_sprite->SetCurrentAnimation(animDamage, false);
	//	player->SetVulnerable(false);
	//	player->SetVulnTicks(0);
	//}

	//if(player_sprite->GetLanding())
	//{
	//	player_sprite->SetCurrentAnimation(animLand, false);
	//	player_sprite->SetLanding(false);
	//}

	//if(player_sprite->GetShooting())
	//{
	//	if(player->GetShoots() < cfg.hero_reload)
	//	{
	//		if(player->GetShoots() == 1)
	//		{
	//			player->Shoot();
	//			if(GetHUD())
	//				GetHUD()->RemoveAmmo();

	//			int flash_x;
	//			int flash_y;
	//			PositionExplosion(player_sprite, flash_x, flash_y);

	//			CreateExplosion(flash_x, flash_y, false, player_sprite->GetMirror());
	//			CreateBullet(flash_x, flash_y, player_sprite->GetMirror());
	//		}
	//		player->SetShoots(player->GetShoots() + 1);
	//	}
	//	else
	//	{
	//		player_sprite->SetShooting(false);
	//		player->SetShoots(0);
	//	}
	//}

	//if(player_sprite->GetWalking())
	//{
	//	player_sprite->SetCurrentAnimation(animWalk, true);
	//}


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
			{
				RECT player_rect = player_sprite->GetCollideRect();
				RECT powerup_rect = powerup_sprite->GetCollideRect();
				RECT intersect_rect;
				if(IntersectRect(&intersect_rect, &powerup_rect, &player_rect ))
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
						player->GetVulnerable() && !player_sprite->GetDamaging())
					{
						if(enemy_sprite->IsSlowpoke() && player_sprite->GetJumping())
						{
							enemy->SetDead(true);
							if(enemy->GetShadow())
								if(enemy->GetShadow())
									enemy->GetShadow()->SetDead(true);
							SCREEN_TEXT* phrase = new SCREEN_TEXT();
							phrase->time_remaining = 2;
							phrase->text = "Pwned!";
							phrase->x = enemy_sprite->GetX() - WORLD_X;
							phrase->y = enemy_sprite->GetY() - WORLD_Y;
							GetScreenTextAllocator()->AddItem(phrase);
						}
						if(player->GetHealth() > 1 && !(enemy_sprite->IsSlowpoke() && player_sprite->GetJumping()))
						{
							player_sprite->SetDamaging(true);
							//player->SetVulnerable(false);
							player->Ouch();
							GetHUD()->RemoveHealth();
						}
						else if(player->GetHealth() == 1 && 
							!(enemy_sprite->IsSlowpoke() && player_sprite->GetJumping()))
						{
							player_sprite->SetDying(true);
							//player->SetVulnerable(false);
							player->Ouch();
							GetHUD()->RemoveHealth();
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

				int flash_x;
				int flash_y;

				PositionExplosion(player_sprite, flash_x, flash_y);

				explosion_sprite->SetX(flash_x);
				explosion_sprite->SetY(flash_y);
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
	static int _prev_tick;
	if(current_time - _prev_tick >= TICK)
	{
		for(UINT i = 0; i < items.size(); i++)
		{
			ITEM* item = items[i];

			item->Move();

			switch(item->GetType())
			{
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
			}	
			if(item->IsDead())					
				DeleteItem(item);
		}
		_prev_tick = current_time;
	}

	// отдельно парсим плееров, с собственным тиком (для плавного перемещения)
	static unsigned int _prev_player_tick;
	// TODO: fix dangerous code:
	static int _lowest_player_speed = GetFirstPlayer()->GetSprite()->GetSpeedX();
	static unsigned int _player_tick = TICK / _lowest_player_speed;
	// -------------------------------------------------------------------------

	if(current_time - _prev_player_tick >= _player_tick)
	{
		for(vector<int>::iterator it = _players.begin();
			it != _players.end();
			it++)
		{
			PLAYER* player = (PLAYER*)GetItem(*it);
			// TODO: fix dangerous code:
			ANIMATEDSPRITE* player_sprite = player->GetSprite();
			SHADOW* shadow = player->GetShadow();
			SPRITE* shadow_sprite = shadow->GetSprite();
			player->Move();
			ProcessPlayer(player);
			if(player_sprite->GetWalking())
			{
				if(!player_sprite->GetWalkingDirection())
				{
					if(PLAYER_NOT_OUT_OF_SCREEN)
					{
						player_sprite->SetX(player_sprite->GetX() + player_sprite->GetSpeedX());
						if(shadow_sprite)
							shadow_sprite->SetX(shadow_sprite->GetX() + player_sprite->GetSpeedX());
					}
					if(PLAYER_OUT_OF_SCREEN)
					{
						WORLD_X += player_sprite->GetSpeedX();
					}
				}
				else
				{
					if(player_sprite->GetX() - player_sprite->GetSpeedX() >= 0)
					{
						player_sprite->SetX(player_sprite->GetX() - player_sprite->GetSpeedX());
						if(shadow_sprite)
							shadow_sprite->SetX(shadow_sprite->GetX() - player_sprite->GetSpeedX());
					}
					if((player_sprite->GetX() - WORLD_X - player_sprite->GetSpeedX() <= BORDER_MARGIN) &&
						(WORLD_X - player_sprite->GetSpeedX() >= WORLD_MIN_X))
					{
						WORLD_X -= player_sprite->GetSpeedX();
					}
				}
			}
		}
		_prev_player_tick = current_time;
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
