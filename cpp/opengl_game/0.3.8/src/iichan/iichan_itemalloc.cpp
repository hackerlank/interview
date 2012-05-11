#include "stdafx.h"
#include "iichan_itemalloc.h"
#include "../misc.h"
#include "../config.h"

#include "../osl/osl.h"

#include "iichan_small_types.h"
#include "iichan_resource.h"
//#include "iichan_bullet.h"
#include "../sound/snd.h"

#include "phys/phys_basebody.h"
#include "phys/phys_dynamicbody.h"
#include "phys/phys_staticbody.h"
#include "phys/phys_collisionsolver.h"

extern SND *snd;

extern config cfg;
extern bool keys[256];
extern int WORLD_X;
extern int WORLD_Y;
extern int WORLD_MAX_X;
extern int WORLD_MIN_X;
extern unsigned int current_time;

#ifdef USING_SAP_PHYSICS
//#include "phys/sap/OPC_ArraySAP.h"
#include "phys/sap/SweepAndPrune.h"
using namespace Opcode;
ArraySAP* SAP;
#endif // USING_SAP_PHYSICS

//#define TRACE_RELEASE

#ifdef TRACE_RELEASE
#define TRACESTART LogToFile("debug.log", "Entering: " + string(__FUNCTION__));
#define TRACEEND LogToFile("debug.log", "Exiting: " + string(__FUNCTION__));
#define TRACEMSG(msg) LogToFile("debug.log", "In: " + string(msg));
#endif
#define TRACEMSG(msg, x1, x2) {string _mesg (msg);char _buf[100]; itoa(x1, _buf, 10);_mesg += _buf; _mesg += ", "; itoa(x2, _buf, 10);_mesg += _buf;LogToFile("debug.log", "In: " + _mesg );}

// --------------------------
void PositionExplosion(ANIMATEDSPRITE* player_sprite, int& flash_x, int& flash_y)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
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

#ifdef TRACE_RELEASE
		TRACEEND
#endif
}

//////////////////////////////////////////////////////////////////////////

// Выполняет перемещение физического тела и спрайта данного предмета
void iiItemAllocator::ProcessAndMovePhysBody(ITEM* item)
{
	assert (item);
	PhysBaseBody* phys_body = item->GetPhysBody();
	SPRITE* spr = item->GetSprite();

	if (!spr || !phys_body)
		return;	

	// TODO: Выполнять, только если изменилась анимация (это случается достаточно редко)
	phys_body->SetWidth(spr->GetRealWidth() *0.5f);
	phys_body->SetHeight(spr->GetRealHeight() *0.5f);

	phys_body->Process(&_phys_bodies);

#ifdef USING_SAP_PHYSICS
	physSAP.UpdateObject(phys_body->sap_handle, phys_body->aabb.GetASAP_AABB());
#endif // USING_SAP_PHYSICS

	MoveSprite(item);

}

void iiItemAllocator::MoveSprite(ITEM* item)
{
	assert (item);
	PhysBaseBody* phys_body = item->GetPhysBody();
	SPRITE* spr = item->GetSprite();
	assert(spr);
	assert(phys_body);

	int rx, ry;
	if (spr->GetMirror())
	{
		rx = spr->GetRealMirrorX();
		ry = spr->GetRealMirrorY();
	}
	else
	{
		rx = spr->GetRealX();
		ry = spr->GetRealY();
	}

	spr->SetX(phys_body->aabb.p.x - spr->GetRealWidth()*0.5f - rx);
	spr->SetY(phys_body->aabb.p.y - spr->GetRealHeight()*0.5f - ry);
}

// ---------------------------
void iiItemAllocator::BatchCreate()
{
	for(vector<ITEM*>::iterator it = _created.begin();
		it != _created.end();
		it++)
	{
		ITEM* item = *it;
		item->SetId(_count);
		items.push_back(item);
		TRACEMSG("Добавлен item: ", _count, (int)item->GetType());
		if(item->GetSprite())
			GetSpriteAllocator()->AddSprite(item->GetSprite());

		PhysBaseBody* pb = item->GetPhysBody();
		if (pb)
		{
			_phys_bodies.push_back(pb);
			pb->SetId(_count);
			TRACEMSG("Добавлен physbody: ", _count, (int)pb->GetType());
#ifdef USING_SAP_PHYSICS

			pb->sap_handle = physSAP.AddObject(pb, pb->id, pb->aabb.GetASAP_AABB());
			TRACEMSG("Добавлен physbody в SAP: ", _count, pb->sap_handle);
#endif // USING_SAP_PHYSICS

		}

		switch(item->GetType())
		{
		case itemPlayer: _players.push_back((PLAYER*)item); break;
		case itemEnemy: _enemies.push_back((ENEMY*)item); break;
		case itemPowerup: _powerups.push_back((POWERUP*)item); break;
		case itemBullet: _bullets.push_back((BULLET*)item); break;
		case itemExplosion: _explosions.push_back((EXPLOSION*)item); break;
		case itemAnimSprite: break;
		}
		_count++;
	}
	_created.clear();
}

void iiItemAllocator::BatchRemove()
{
	for(vector<ITEM*>::iterator it = _created.begin();
		it != _created.end();
		it++)
	{
		ITEM* item = *it;
		assert(item);
		int id = item->GetId();

		// first of all, remove its id from cache
		switch(item->GetType())
		{
		case itemPlayer:
			_players.erase(_players.begin() + GetIndexById(&_players, id));
			break;
		case itemEnemy:
			_enemies.erase(_enemies.begin() + GetIndexById(&_enemies, id));
			break;
		case itemPowerup:
			_powerups.erase(_powerups.begin() + GetIndexById(&_powerups, id));
			break;
		case itemBullet:
			_bullets.erase(_bullets.begin() + GetIndexById(&_bullets, id));
			break;
		case itemExplosion:
			_explosions.erase(_explosions.begin() + GetIndexById(&_explosions, id));
			break;
		}



		// mark sprite as dead
		// (it will be free'd by SpriteAllocator later)
		SPRITE* sprite = item->GetSprite();
		if(sprite)
			sprite->SetDead(true);

		PhysBaseBody* pb = item->GetPhysBody();
		if (pb)
		{
#ifdef USING_SAP_PHYSICS
			physSAP.RemoveObject(pb->sap_handle);
			TRACEMSG("Удален physbody из SAP: ", pb->id, pb->sap_handle);
#endif // USING_SAP_PHYSICS		
			_phys_bodies.erase(_phys_bodies.begin() + GetIndexById(&_phys_bodies, id));


			TRACEMSG("Удален physbody: ", pb->id, (int)pb->GetType());
			delete pb;
		}

		int index = GetIndexById(&items, id);
		assert(index != -1);
		TRACEMSG("Удален item: ", id, (int)item->GetType());
		delete item;
		item = NULL;

		items.erase(items.begin() + index);
	}
	_removed.clear();
}


void iiItemAllocator::AddItem(ITEM* item)
{
	_created.push_back(item);
	/*
	#ifdef TRACE_RELEASE
	TRACESTART
	#endif
	item->SetId(_count);
	items.push_back(item);
	TRACEMSG("Добавлен item: ", _count, (int)item->GetType())
	if(item->GetSprite())
	GetSpriteAllocator()->AddSprite(item->GetSprite());

	PhysBaseBody* pb = item->GetPhysBody();
	if (pb)
	{
	_phys_bodies.push_back(pb);
	pb->SetId(_count);
	TRACEMSG("Добавлен physbody: ", _count, (int)pb->GetType())
	#ifdef USING_SAP_PHYSICS

	pb->sap_handle = physSAP.AddObject(pb, pb->id, pb->aabb.GetASAP_AABB());
	TRACEMSG("Добавлен physbody в SAP: ", _count, pb->sap_handle)
	#endif // USING_SAP_PHYSICS

	}

	switch(item->GetType())
	{
	case itemPlayer: _players.push_back((PLAYER*)item); break;
	case itemEnemy: _enemies.push_back((ENEMY*)item); break;
	case itemPowerup: _powerups.push_back((POWERUP*)item); break;
	case itemBullet: _bullets.push_back((BULLET*)item); break;
	case itemExplosion: _explosions.push_back((EXPLOSION*)item); break;
	case itemAnimSprite: break;
	}
	_count++;
	#ifdef TRACE_RELEASE
	TRACEEND
	#endif
	*/
}

void iiItemAllocator::DeleteItem(ITEM* item)
{
	_removed.push_back(item);
	/*
	#ifdef TRACE_RELEASE
	TRACESTART
	#endif

	assert(item);
	int id = item->GetId();

	// first of all, remove its id from cache
	switch(item->GetType())
	{
	case itemPlayer:
	_players.erase(_players.begin() + GetIndexById(&_players, id));
	break;
	case itemEnemy:
	_enemies.erase(_enemies.begin() + GetIndexById(&_enemies, id));
	break;
	case itemPowerup:
	_powerups.erase(_powerups.begin() + GetIndexById(&_powerups, id));
	break;
	case itemBullet:
	_bullets.erase(_bullets.begin() + GetIndexById(&_bullets, id));
	break;
	case itemExplosion:
	_explosions.erase(_explosions.begin() + GetIndexById(&_explosions, id));
	break;
	}



	// mark sprite as dead
	// (it will be free'd by SpriteAllocator later)
	SPRITE* sprite = item->GetSprite();
	if(sprite)
	sprite->SetDead(true);

	PhysBaseBody* pb = item->GetPhysBody();
	if (pb)
	{
	#ifdef USING_SAP_PHYSICS
	physSAP.RemoveObject(pb->sap_handle);
	TRACEMSG("Удален physbody из SAP: ", pb->id, pb->sap_handle)
	#endif // USING_SAP_PHYSICS		
	_phys_bodies.erase(_phys_bodies.begin() + GetIndexById(&_phys_bodies, id));


	TRACEMSG("Удален physbody: ", pb->id, (int)pb->GetType())
	delete pb;
	}

	int index = GetIndexById(&items, id);
	assert(index != -1);
	TRACEMSG("Удален item: ", id, (int)item->GetType())
	delete item;
	item = NULL;

	items.erase(items.begin() + index);

	#ifdef TRACE_RELEASE
	TRACEEND
	#endif*/
}

ITEM* iiItemAllocator::GetItem(int id)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		for(vector<ITEM*>::iterator it = items.begin();
			it != items.end();
			it++)
		{
			ITEM* item = *it;
			if(item)
				if(item->GetId() == id)
					return item;
		}
#ifdef TRACE_RELEASE
		TRACEEND
#endif
			return NULL;
}

iiItemAllocator::iiItemAllocator()
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
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
	current_level = NULL;
	_prev_player_tick = 0;
	_lowest_player_speed = 0;
	_player_tick = 0;

#ifdef USING_SAP_PHYSICS
	SAP = &physSAP;
#ifdef __DEBUG_SAP_PHYSICS_
	pairsToDraw = NULL;
	pairsToDrawCount = 0;
#endif // __DEBUG_SAP_PHYSICS_
#endif // USING_SAP_PHYSICS

#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

iiItemAllocator::~iiItemAllocator()
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
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
					WriteToLog("Exception in iiItemAllocator::~iiItemAllocator()");
				}
			}
		}
		items.clear();
		_players.clear();
		_bullets.clear();
		_powerups.clear();
		_explosions.clear();
		_enemies.clear();

#ifdef USING_SAP_PHYSICS
		SAP = NULL;
#endif // USING_SAP_PHYSICS

#ifdef TRACE_RELEASE
		TRACEEND
#endif
}

bool iiItemAllocator::GetPlayerPosState()
{
	static int x;
	static int y;

	ITEM* player = GetFirstPlayer();
	assert(player);

	ANIMATEDSPRITE* sprite = player->GetSprite();
	assert(sprite);

	bool result;

	result = (sprite->GetX() != x || sprite->GetY() != y);

	x = sprite->GetX();
	y = sprite->GetY();

	return result;
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

#define PRESSED_FIRE KEYDOWN(cfg.fire)
#define PRESSED_LEFT KEYDOWN(cfg.left)
#define PRESSED_RIGHT KEYDOWN(cfg.right)
#define PRESSED_JUMP KEYDOWN(cfg.jump)
#define PRESSED_DOWN KEYDOWN(cfg.down)


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

void iiItemAllocator::ProcessPlayer(ITEM* item)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		assert(item != NULL);
	PLAYER* player = (PLAYER*)item;

	ANIMATEDSPRITE* player_sprite = item->GetSprite();
	assert(player_sprite != NULL);

	SHADOW* shadow = player->GetShadow();
	SPRITE* shadow_sprite = NULL;
	if(shadow) shadow_sprite = shadow->GetSprite();

	PhysDynamicBody *pb = (PhysDynamicBody*)player->GetPhysBody();
	assert(pb!=NULL);

	// TODO: Как-то нормально надо
	GetHUD()->SetHealthCount(player->GetHealth());

	if (player->GetHealth() <= 0)
	{
		player_sprite->SetDying(true);
	}

	if(player_sprite->GetDying())
	{
		player_sprite->SetWalking(false);
		player_sprite->SetLanding(false);
		if(player_sprite->GetJumping())
		{
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
	if(player_sprite->GetWalking() && !player_sprite->GetJumping() && !player_sprite->GetLanding())
	{
		if(player_sprite->GetCurrentAnimation() != animWalk)
		{
			// first time
			player_sprite->SetCurrentAnimation(animWalk, true);
		}
	}

	if (!player_sprite->GetJumping() && !pb->on_plane)
	{
		player_sprite->SetJumping(true);
	}
	if(player_sprite->GetJumping() && !player_sprite->GetLanding())
	{
		if(player_sprite->GetCurrentAnimation() != animJump)
		{
			// Первый раз
			player_sprite->SetCurrentAnimation(animJump, false);
		}
		// Направление прыжка
		player_sprite->SetJumpDirection(pb->vel.y >= 0);
	}
	if (player_sprite->GetJumping() && !player_sprite->GetLanding() && pb->on_plane)
	{
		player_sprite->SetLanding(true);
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
				player_sprite->SetJumping(false);
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
			CreateBullet(flash_x, flash_y, player_sprite->GetMirror(), (CHARACTER*) player);
			player->SetShoots(1);

			if(snd)
				snd->PlaySnd(player->GetShootSound(), false);

		}
		else
		{
			if(player->GetShoots() < player->GetReloadSpeed())
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
		if(PRESSED_JUMP && pb->on_plane && !player_sprite->GetLanding() && player->isJumpKeyRel)
		{
			player_sprite->SetJumping(true);
			player_sprite->SetSitting(false);
			pb->AddVelocity(-pb->jump_speed);
			pb->on_plane = false;
			player->isJumpKeyRel = false;
		}
		else
		{
			if (!PRESSED_JUMP)
				player->isJumpKeyRel = true;
		}
		if(PRESSED_RIGHT)
		{
			player_sprite->SetMirror(false);
			player_sprite->SetWalking(true);
			player_sprite->SetSitting(false);
			player_sprite->SetWalkingDirection(false);
			if (pb->on_plane)
				pb->AddVelocity(pb->walk_acc);
			else
				pb->AddVelocity(pb->walk_acc*0.2f);
		}
		if(PRESSED_LEFT)
		{
			player_sprite->SetMirror(true);
			player_sprite->SetWalking(true);
			player_sprite->SetSitting(false);
			player_sprite->SetWalkingDirection(true);
			if (pb->on_plane)
				pb->AddVelocity(-pb->walk_acc);
			else
				pb->AddVelocity(-pb->walk_acc*0.2f);
		}
	}

	if((!PRESSED_LEFT && PLAYER_STILL_MOVING_LEFT) ||
		(!PRESSED_RIGHT && PLAYER_STILL_MOVING_RIGHT) ||
		!KEYBOARD_ENABLED)
	{
		player_sprite->SetWalking(false);
		player_sprite->SetDoneAnimation(true);
		//pb->vel = (Vector2(0, pb->vel.y)); // Не надо так сурово... в диз доке же было про инерцию. (!)
	}
	if(!PRESSED_DOWN && player_sprite->GetSitting() || !KEYBOARD_ENABLED)
	{
		player_sprite->SetSitting(false);
		player_sprite->SetDoneAnimation(true);
	}

	if(PLAYER_READY)
	{
		if(player_sprite->GetCurrentAnimation() != animStand)
		{
			// first time
			player_sprite->SetCurrentAnimation(animStand, true);
		}
	}


	ProcessAndMovePhysBody(player);

	if(shadow_sprite)
	{
		shadow_sprite->SetX(player_sprite->GetX() + player_sprite->GetShadowX());
		shadow_sprite->SetY(player_sprite->GetY() + player_sprite->GetShadowY());
	}


#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::ProcessPowerup(ITEM* item)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		assert(item);
	POWERUP* powerup = (POWERUP*)item;
	ANIMATEDSPRITE* powerup_sprite = powerup->GetSprite();

	if(powerup_sprite)
	{
		for(vector<PLAYER*>::iterator it = _players.begin();
			it != _players.end();
			it++)
		{
			//int id = *it;
			PLAYER* player = *it;
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
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::ProcessEnemy(ITEM *item)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		assert(item);
	ENEMY* enemy = (ENEMY*) item;
	ANIMATEDSPRITE* enemy_sprite = enemy->GetSprite();
	SHADOW* shadow = enemy->GetShadow();
	SPRITE* shadow_sprite = NULL;
	if (shadow)
		shadow_sprite = shadow->GetSprite();

	PhysDynamicBody* phys_body = NULL;
	phys_body = (PhysDynamicBody*)enemy->GetPhysBody();

	if (enemy_sprite && phys_body)
	{
		// Умираем ли?
		if (enemy->GetHealth() <= 0 )
		{
			enemy_sprite->SetDying(true);
			enemy_sprite->SetWalking(false);
			enemy_sprite->SetShooting(false);
			if (enemy_sprite->GetCurrentAnimation() != animDie)
			{
				// Начало гибели
				enemy_sprite->SetCurrentAnimation(animDie, false);
				phys_body->vel = (Vector2::Blank());
			}
			else if (enemy_sprite->GetDoneAnimation())
			{
				// Все, мертвы
				enemy->SetDead(true);
				if(shadow)
					shadow->SetDead(true);
			}
			// TODO: Тут возможно стоит сделать, чтобы враги исчезали не сразу а по прошествии некоторого времени
			return;
		}

		// Находим количество игроков и ближайшего игрока, а ткаже положение врага по отношению к нему
		//bool on_screen = ENEMY_ON_SCREEN;
		bool enemy_left = false;						// Слева от ближайшего игрока
		bool enemy_right = false;						// Справа от ближайшего игрока
		int player_count = 0;							// Количество игроков
		PLAYER* near_player = NULL;						// Ближайший игрок
		ANIMATEDSPRITE* near_player_sprite = NULL;		// Спрайт ближайшего игрока
		int near_dist;									// Расстояние до ближайшего игрока

		for(vector<PLAYER*>::iterator it = _players.begin();
			it != _players.end();
			it++)
		{
			int distance = 0;
			PLAYER* player = *it;
			ANIMATEDSPRITE* player_sprite = NULL;
			if (player)
				player_sprite = player->GetSprite();

			if (player_sprite)
			{
				player_count++;
				bool e_right = ENEMY_RIGHT;
				bool e_left = ENEMY_LEFT;

				if (e_right)
				{
					distance = enemy_sprite->GetX() - player_sprite->GetX() + player_sprite->GetRealX();
				}
				else if (e_left)
				{
					distance = player_sprite->GetX() - enemy_sprite->GetX() - enemy_sprite->GetRealX();
				}

				// TODO: Тут можно сделать более сложные проверки, типа если враг - стрелок и ближайший игрок
				// не на линии огня, то игнорировать этого игрока, а искать другого
				if (player_count == 1 || near_dist > distance)
				{
					near_dist = distance;
					enemy_left = e_left;
					enemy_right = e_right;
					near_player = player;
					near_player_sprite = player_sprite;
				}
			}
		}

		if (GetInstanceAllocator()->GetAI() &&
			enemy->GetAIEnabled() &&
			player_count > 0)
		{
			// На дистанции атаки?
			if (enemy->GetAttackRange() > near_dist)
			{
				enemy_sprite->SetShooting(true);
				if (enemy_sprite->GetCurrentAnimation() != animShoot)
				{
					// Начало анимации стрельбы
					phys_body->vel = (Vector2(0.0, phys_body->vel.y));
					enemy_sprite->SetCurrentAnimation(animShoot, false);
					enemy_sprite->SetWalking(false);
					if (enemy_right) enemy_sprite->SetMirror(true);
					else if (enemy_left) enemy_sprite->SetMirror(false);
				}
				else if (enemy_sprite->GetDoneAnimation())
				{
					// Окончание анимации стрельбы
					enemy_sprite->SetShooting(false);
					enemy_sprite->SetWalking(true);
					enemy_sprite->SetCurrentAnimation(animNone, false);
				}

				if (enemy->GetEnemyType() == enemyShooter)
				{	// Стрелок
					if (enemy->GetShoots() == 0)
					{	// Перезарядился
						if (enemy_sprite->GetCurrentPattern() == enemy_sprite->GetShootAttackPattern())
						{
							// Cоздаем снаряд
							int flash_x = enemy_sprite->GetX();
							int flash_y = enemy_sprite->GetY();

							// TODO: Переделать PositionExplosion для любого персонажа
							//PositionExplosion(player_sprite, flash_x, flash_y);							
							if(!enemy_sprite->GetMirror())
							{
								flash_x += enemy_sprite->GetFlashStandX() + enemy_sprite->GetRealX();
								flash_y += enemy_sprite->GetFlashStandY() + enemy_sprite->GetRealY();;
							}
							else
							{
								flash_x += enemy_sprite->GetFlashStandMirrorX() + enemy_sprite->GetRealMirrorX();
								flash_y += enemy_sprite->GetFlashStandMirrorY() + enemy_sprite->GetRealMirrorY();
							}

							// TODO: Переделать CreateExplosion для любого персонажа
							//CreateExplosion(flash_x, flash_y, false, enemy_sprite->GetMirror());
							CreateBullet(flash_x, flash_y, enemy_sprite->GetMirror(), (CHARACTER*) enemy);

							enemy->SetShoots(1);
						}
					}
					else
					{
						// Идет перезарядка
						if(enemy->GetShoots() < enemy->GetReloadSpeed())
						{
							enemy->SetShoots(enemy->GetShoots() + 1);
						}
						else
						{
							enemy->SetShoots(0);
						}
					}
				}
			}
			else
			{
				// Не на дистанции атаки, подходим
				// TODO: Тут надо сделать проверку, а можно ли вообще подойти (может на разных платформах стоят)
				// И тут же поведение, если подойти и выстрелить нельзя
				if (enemy_right)
				{
					enemy_sprite->SetCurrentAnimation(animWalk, true);
					enemy_sprite->SetWalking(true);
					//enemy_sprite->SetX(enemy_sprite->GetX() - int(enemy_sprite->GetSpeedX()));
					phys_body->AddVelocity(-phys_body->walk_acc);

					enemy_sprite->SetMirror(true);
				}
				else if (enemy_left)
				{
					enemy_sprite->SetCurrentAnimation(animWalk, true);
					enemy_sprite->SetWalking(true);
					//enemy_sprite->SetX(enemy_sprite->GetX() + int(enemy_sprite->GetSpeedX()));
					phys_body->AddVelocity(phys_body->walk_acc);

					enemy_sprite->SetMirror(false);
				}
				else
				{	// Вобще-то враг не справа и не слева, если совпадает с игроком, а тогда он, наверное,
					// атаковать должен или отходить. Ну да ладно, пусть пока будет.
					if(enemy_sprite->GetCurrentAnimation() != animStand)
					{
						enemy_sprite->SetCurrentAnimation(animStand, true);
						enemy_sprite->SetStanding(true);
						phys_body->vel = (Vector2(0, phys_body->vel.y));
					}
				}
			}

			// Персечение игрока и врага
			RECT intersect;
			RECT player_rect;
			RECT enemy_rect;

			near_player_sprite->GetMirror() ? player_rect = near_player_sprite->GetMirrorCollideRect() :
				player_rect = near_player_sprite->GetCollideRect();

			enemy_sprite->GetMirror() ? enemy_rect = enemy_sprite->GetMirrorCollideRect() :
				enemy_rect = enemy_sprite->GetCollideRect();

			if(IntersectRect(&intersect, &player_rect, &enemy_rect) &&
				near_player->GetVulnerable() && !near_player_sprite->GetDamaging())
			{
				// Пересеклись
				if (enemy->GetSquashable() && near_player_sprite->GetJumping() && near_player_sprite->GetJumpDirection())
				{	// Игрок давит противника
					enemy->SetHealth(0);
				}
				else if (enemy->GetAttackCollide() && near_player->GetHealth() > 0)
				{	// Игрок наткнулся на противника и повреждается
					near_player_sprite->SetDamaging(true);
					near_player->Ouch(enemy->GetDamage());
				}
				if (enemy->GetEnemyType() == enemyFighter && near_player->GetHealth() > 0 &&
					enemy_sprite->GetCurrentPattern() == enemy_sprite->GetShootAttackPattern())
				{	// Враг - боец и сейчас момент анимации, когда наносится поврежение
					near_player_sprite->SetDamaging(true);
					near_player->Ouch(enemy->GetDamage());
				}			
			}			
		}
		else
		{
			//Если АИ отключен или нет игроков
			enemy_sprite->SetStanding(true);
			enemy_sprite->SetCurrentAnimation(animStand, true);
			phys_body->vel = (Vector2(0.0, phys_body->vel.y));
		}

		ProcessAndMovePhysBody(enemy);

		if(shadow_sprite)
		{
			shadow_sprite->SetX(enemy_sprite->GetX() + enemy_sprite->GetShadowX());
			shadow_sprite->SetY(enemy_sprite->GetY() + enemy_sprite->GetShadowY());
		}

	}
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::ProcessExplosion(ITEM* item)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		assert(item);
	EXPLOSION* explosion = (EXPLOSION*)item;
	ANIMATEDSPRITE* explosion_sprite = explosion->GetSprite();

	if(explosion_sprite)
	{
		if(explosion_sprite->GetDoneAnimation())
		{
			item->SetDead(true);
			return;
		}
		for(vector<PLAYER*>::iterator it = _players.begin();
			it != _players.end();
			it++)
		{
			PLAYER* player = *it;
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

		
	}
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::ProcessBullet(ITEM* item)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif

		assert(item);

	/*BULLET* bullet = (BULLET*)item;
	SPRITE* bullet_sprite = (SPRITE*)bullet->GetSprite();
	PhysDynamicBody* phys_body = (PhysDynamicBody*)bullet->GetPhysBody();
	if(!bullet_sprite && !phys_body)
	return;
	*/

	/*	if(BULLET_OUT_OF_SCREEN)
	{
	bullet->SetDead(true);
	return;
	}
	*/

	ProcessAndMovePhysBody(item);

#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::Move()
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
bool update = false;
		if(current_time - _prev_tick >= TICK)
		{
			update = true;
			for(UINT i = 0; i < items.size(); i++)
			{
				ITEM* item = items[i];

				// Кривое решение проблемы
				// Объект начинает обрабатываться только на следующем ходу после своего создания
				// Иначе возможны проблемы с SAP, который еще не успел обновиться
				// Вобще итемам нужно такое же добавление пачкой, как и в SAP 
				/*	if (item->is_new)
				{
				item->is_new = false;
				continue;
				}*/

				switch(item->GetType())
				{
				case itemEnemy: // PROCESSING ENEMY
					{
						item->Move();
						ProcessEnemy(item);
						break;
					}
				case itemPowerup: // PROCESSING POWERUP
					{
						item->Move();
						ProcessPowerup(item);
						break;
					}
				case itemExplosion: // PROCESSING EXPLOSION
					{
						item->Move();
						ProcessExplosion(item);
						break;
					}
				case itemBullet: // PROCESSING BULLET
					{
						item->Move();
						ProcessBullet(item);
						break;
					}
				case itemAnimSprite:
					{
						item->Move();
						break;
					}
				}	
				if(item->IsDead())					
					DeleteItem(item);
			}
			_prev_tick = current_time;
		}

		// отдельно парсим плееров, с собственным тиком (для плавного перемещения)
		// если их нет, выходим
		if(!GetFirstPlayer())
			return;
		_prev_player_tick = 0;
		//_lowest_player_speed = GetFirstPlayer()->GetSprite()->GetSpeedX();
		_lowest_player_speed = ((PhysDynamicBody*)GetFirstPlayer()->GetPhysBody())->max_x_vel;	// Хреновое название переменой
		_player_tick = int(TICK / _lowest_player_speed);
		// -------------------------------------------------------------------------

		PhysBaseBody *phys_body = NULL;

		if(current_time - _prev_player_tick >= _player_tick)
		{
			update = true;
			for(vector<PLAYER*>::iterator it = _players.begin();
				it != _players.end();
				it++)
			{
				PLAYER* player = *it;
				// TODO: fix dangerous code:
				ANIMATEDSPRITE* player_sprite = player->GetSprite();
				phys_body = player->GetPhysBody();

				if (!phys_body || !player_sprite)
					continue;

				SHADOW* shadow = player->GetShadow();
				SPRITE* shadow_sprite = NULL;
				if (shadow)
					shadow_sprite = shadow->GetSprite();

				player->Move();
				ProcessPlayer(player);


				


				// TODO: Переделать нормально камеру
				int rx, ry;
				if (player_sprite->GetMirror())
				{
					rx = player_sprite->GetRealMirrorX();
					ry = player_sprite->GetRealMirrorY();
				}
				else
				{
					rx = player_sprite->GetRealX();
					ry = player_sprite->GetRealY();
				}

				WORLD_X = player_sprite->GetX() + rx + player_sprite->GetRealWidth()/2 - cfg.scr_width/2;
				WORLD_Y = -(player_sprite->GetY() + ry + player_sprite->GetRealHeight()/2 - cfg.scr_height * 0.7);


				/*if(player_sprite->GetWalking())
				{
				if(!player_sprite->GetWalkingDirection())
				{
				if(PLAYER_NOT_OUT_OF_SCREEN)
				{
				player_sprite->SetX(player_sprite->GetX() + int(player_sprite->GetSpeedX()));
				if(shadow_sprite)
				shadow_sprite->SetX(shadow_sprite->GetX() + int(player_sprite->GetSpeedX()));
				}
				if(PLAYER_OUT_OF_SCREEN)
				{
				WORLD_X += int(player_sprite->GetSpeedX());
				}
				}
				else
				{
				if(player_sprite->GetX() - player_sprite->GetSpeedX() >= 0)
				{
				player_sprite->SetX(player_sprite->GetX() - int(player_sprite->GetSpeedX()));
				if(shadow_sprite)
				shadow_sprite->SetX(shadow_sprite->GetX() - int(player_sprite->GetSpeedX()));
				}
				if((player_sprite->GetX() - WORLD_X - player_sprite->GetSpeedX() <= BORDER_MARGIN) &&
				(WORLD_X - player_sprite->GetSpeedX() >= WORLD_MIN_X))
				{
				WORLD_X -= int(player_sprite->GetSpeedX());
				}
				}
				}*/


			}
			_prev_player_tick = current_time;
		}

		if (update)
		{
			BatchCreate();
			BatchRemove();
#ifdef USING_SAP_PHYSICS

			ASAP_Pair* pairs;
			UINT pairsCount = physSAP.DumpPairs(NULL, NULL, NULL, &pairs);
			if (pairsCount)
			{
#ifdef __DEBUG_SAP_PHYSICS_
				DELETEARRAY(pairsToDraw);
				pairsToDraw = new Vector2[pairsCount*2];
				pairsToDrawCount = pairsCount;
#endif // __DEBUG_SAP_PHYSICS_

				PhysBaseBody** b = new PhysBaseBody*[pairsCount * 2];
				for (UINT i = 0; i < pairsCount; i++)
				{
					PhysBaseBody* pb0 = (PhysBaseBody*)pairs[i].object0;
					PhysBaseBody* pb1 = (PhysBaseBody*)pairs[i].object1;

					SolveCollision(pb0, pb1);

					if (pb0->GetType() == bodyDynamic)
					{
						MoveSprite( ((PhysDynamicBody*)pb0)->item );
					}
					if (pb1->GetType() == bodyDynamic)
					{
						MoveSprite( ((PhysDynamicBody*)pb1)->item );
					}
					b[i*2 + 0] = pb0;
					b[i*2 + 1] = pb1;
#ifdef __DEBUG_SAP_PHYSICS_
					pairsToDraw[i*2+0] = pb0->aabb.p;
					pairsToDraw[i*2 + 1] = pb1->aabb.p;
#endif // __DEBUG_SAP_PHYSICS_
				}
				for (UINT i = 0; i < pairsCount*2; i+=2)
				{
					physSAP.UpdateObject(b[i]->sap_handle, b[i]->aabb.GetASAP_AABB());
					physSAP.UpdateObject(b[i+1]->sap_handle, b[i+1]->aabb.GetASAP_AABB());
				}	
				DELETEARRAY(b);
			}
#endif // USING_SAP_PHYSICS
		}
#ifdef TRACE_RELEASE
		TRACEEND
#endif
}

bool iiItemAllocator::LoadLevel(string file_name)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		if(current_level)
		{
			delete current_level;
			current_level = NULL;
		}

		current_level = new iiLevel();

		FILE* file = NULL;
		file = fopen(file_name.c_str(), "rb");
		if(!file)
		{
			WriteToLog(DEFAULT_OSL_LOG_NAME, "Failed to open binary level: " + string(file_name));
			return false;
		}

		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);

		unsigned __int32 num_types;

		fread (&num_types, 4, 1, file);
		if (num_types == 0)
		{
			WriteToLog(DEFAULT_OSL_LOG_NAME, "Level is empty" + string(file_name));
			return false;
		}
		obj* ob = new obj[num_types];
		char buf[256];
		unsigned int i, j;
		for (i = 0; i < num_types; i++)
		{
			j=-1;
			// В цикле читаем имя типа
			do 
			{
				j++;
				fread(&buf[j], 1, 1, file);
			} while ((buf[j] != '\0')&&(j < 256));

			//ob[i].name = new char[strlen(buf)];
			//strcpy(ob[i].name, buf);
			ob[i].name = string(buf);

			fread (&ob[i].count, 4, 1, file);				// КОличество обьектов этого типа
			ob[i].c = new coord[ob[i].count];
			ob[i].m = new mnum[ob[i].count];
			ob[i].ob_id = new int[ob[i].count];
			for (j=0; j < ob[i].count; j++)					// ЧИтаем каждый из обьктов в массив структур
			{
				fread(&(ob[i].c[j].x), 4, 1, file);			
				fread(&(ob[i].c[j].y), 4, 1, file);
				fread(&(ob[i].m[j].n1), 2, 1, file);
				fread(&(ob[i].m[j].n2), 2, 1, file);
				fread(&(ob[i].ob_id[j]), 4, 1, file);
			}
		}
		fseek(file, 3, SEEK_CUR);							// Пропусти метку "EOL". Вообщем-то она не нужна.
		long script_begin = ftell(file);
		char *script = new char[length - script_begin+1];	
		fread(script, length - script_begin, 1, file);		// Читаем скрипт до самого конца файла.
		script[length - script_begin] = '\0';
		fclose(file);

		for (i = 0; i < num_types; i++)
		{
			//RESOURCE* res = ResourceByName(ob[i].name);
			RESOURCE* res = ResourceByName(ob[i].name.c_str());
			if(!res)
			{
				WriteToLog(DEFAULT_LOG_NAME, "Resource not loaded: " + string(ob[i].name));
			}
			else
			{
				switch (res->behaviour)						// Выбираем, тип обьекта, что бы выбрать, что грузить.
				{
				case resPlayer:
					{
#ifdef TRACE_RELEASE
						TRACEMSG("case resPlayer")
#endif
							iiPlayer e;
						e.res = ob[i].name;

						// Эти присовения временны, надо из файла уровня брать значения.
						e.health = 100;
						e.ammo = 20;
						e.score = 0;

						for(j=0; j < ob[i].count; j++)		// Грузим каждый из обьктов этого типа
						{
							e.x = ob[i].c[j].x;
							e.y = ob[i].c[j].y;
							//e.health = ob[i].m[j].n1;
							//e.ammo = ob[i].m[j].n2;						
							current_level->players.push_back(e);
							CreatePlayer(res, e.health, e.ammo, e.score, e.x, e.y);
						}
						break;
					}
				case resEnemy:
					{
#ifdef TRACE_RELEASE
						TRACEMSG("case resEnemy")
#endif
							iiEnemy e;
						e.res = ob[i].name;

						// Эти присовения временны, надо из файла уровня брать значения.
						e.health = 100;

						for(j=0; j < ob[i].count; j++)		// Грузим каждый из обьктов этого типа
						{
							e.x = ob[i].c[j].x;
							e.y = ob[i].c[j].y;
							//e.health = ob[i].m[j].n1;
							current_level->enemies.push_back(e);
							CreateEnemy(res, e.x, e.y, e.health);
						}
						break;
					}
				case resPowerup:
					{
#ifdef TRACE_RELEASE
						TRACEMSG("case resPowerup")
#endif
							iiPowerup e;
						e.res = ob[i].name;
						// Эти присовения временны, надо из файла уровня брать значения.

						//if (!strcmp(ob[i].name, "pwr_ammo"))
						if(ob[i].name == "pwr_ammo")
						{
							e.power = 20;
							e.type = pwrAmmo;
						}
						//else if (!strcmp(ob[i].name,"pwr_health"))
						else if(ob[i].name == "pwr_health")
						{
							e.power = 5;
							e.type = pwrHealth;
						}
						//else if (!strcmp(ob[i].name,"pwr_score"))
						else if(ob[i].name == "pwr_score")
						{
							e.power = 30;
							e.type = pwrScore;
						}
						else
						{
							e.power = 0;
							e.type = pwrNone;
						}


						for(j=0; j < ob[i].count; j++)			// Грузим каждый из обьктов этого типа
						{
							e.x = ob[i].c[j].x;
							e.y = ob[i].c[j].y;
							//e.power = ob[i].m[j].n1;

							current_level->powerups.push_back(e);
							CreatePowerup(res, e.power, e.x, e.y);
						}
						break;
					}
				case resSprite:
					{
#ifdef TRACE_RELEASE
						TRACEMSG("case resSprite")
#endif
							iiTile e;
						e.res = ob[i].name;

						// Эти присовения временны, надо из файла уровня брать значения.
						e.gravity = 0;
						e.height = 0;
						e.width = 0;
						e.plank = 0;

						for(j=0; j < ob[i].count; j++)				// Грузим каждый из обьктов этого типа
						{
							e.x = ob[i].c[j].x;
							e.y = ob[i].c[j].y;
							current_level->tiles.push_back(e);
							CreateSprite(res, e.x, e.y);
						}
						break;
					}
				case resAnimSprite:
					{
#ifdef TRACE_RELEASE
						TRACEMSG("case resAnimSprite")
#endif
							iiTile e;
						e.res = ob[i].name;

						// Эти присовения временны, надо из файла уровня брать значения.
						e.gravity = 0;
						e.height = 0;
						e.width = 0;
						e.plank = 0;

						for(j=0; j < ob[i].count; j++)				// Грузим каждый из обьктов этого типа
						{
							e.x = ob[i].c[j].x;
							e.y = ob[i].c[j].y;
							current_level->tiles.push_back(e);
							CreateAnimSprite(res, e.x, e.y);
						}
						break;
					}

				default:
					{
#ifdef TRACE_RELEASE
						TRACEMSG("case default")
#endif
							break;
					}
				}
			}
		}

		osl_Load(string(script).c_str(), true);						// Отправлям скрипт на обработку и выполнение.

		delete[] script;

#ifdef TRACE_RELEASE
		TRACEEND
#endif

			BatchCreate();
#ifdef USING_SAP_PHYSICS
			ASAP_Pair* pairs;
		UINT pairsCount = physSAP.DumpPairs(NULL, NULL, NULL, &pairs);
#endif // USING_SAP_PHYSICS

		return true;
}


void iiItemAllocator::CreatePlayer(RESOURCE* res, int health, int ammo, int score,
								   int x, int y)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		PLAYER* player = new PLAYER(health, ammo, score);
	ANIMATEDSPRITE* player_sprite = new ANIMATEDSPRITE();
	PhysDynamicBody* phys_body = new PhysDynamicBody();

	player->SetSprite(player_sprite);
	if (res->bullet_type != "")
		player->SetBulletType( ResourceByName(res->bullet_type.c_str()) );
	if (res->shoot_sound != "")
		player->SetShootSound(res->shoot_sound);
	player->SetReloadSpeed(res->reload_speed);

	player->SetPhysBody(phys_body);

	phys_body->SetAABB(x + res->real_stand_x, y + res->real_stand_y, res->real_stand_width, res->real_stand_height);

	phys_body->jump_speed = (Vector2(0.0, res->phys_jump_speed));
	phys_body->walk_acc = (Vector2(res->phys_walk_vel, 0.0));
	phys_body->max_x_vel = (res->phys_max_x_speed);
	phys_body->max_y_vel = (res->phys_max_y_speed);
	phys_body->SetCollidable(res->phys_collidable != 0);
	phys_body->SetBulletCollidable(res->bullet_collidable != 0);
	phys_body->item = player;
	//phys_body->mass = (res->phys_mass);

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
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::CreateEnemy(RESOURCE* res, int x, int y, int health)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		ENEMY* enemy = new ENEMY(health, (ENEMY_TYPE)res->enemy_type, res->enemy_squashable, res->enemy_attack_collide);
	if (res->bullet_type != "")
		enemy->SetBulletType(ResourceByName(res->bullet_type.c_str()));
	if (res->shoot_sound != "")
		enemy->SetShootSound(res->shoot_sound);
	enemy->SetReloadSpeed(res->reload_speed);
	enemy->SetAttackRange(res->attack_range);
	enemy->SetDamage(res->damage);

	PhysDynamicBody* phys_body = new PhysDynamicBody();
	enemy->SetPhysBody(phys_body);

	phys_body->SetAABB(x + res->real_stand_x, y + res->real_stand_y, res->real_stand_width, res->real_stand_height);

	phys_body->jump_speed = (Vector2(0.0, res->phys_jump_speed));
	phys_body->walk_acc = (Vector2(res->phys_walk_vel, 0.0));
	phys_body->max_x_vel = (res->phys_max_x_speed);
	phys_body->max_y_vel = (res->phys_max_y_speed);
	phys_body->SetCollidable(res->phys_collidable != 0);
	phys_body->SetBulletCollidable(res->bullet_collidable != 0);
	phys_body->item = enemy;

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
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::CreatePowerup(RESOURCE* res, int count, int x, int y)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif

		POWERUP_TYPE power_type;
	if (!strcmp(res->name.c_str(), "pwr_ammo"))
		power_type = pwrAmmo;
	else if (!strcmp(res->name.c_str(), "pwr_health"))
		power_type = pwrHealth;
	else if (!strcmp(res->name.c_str(), "pwr_score"))
		power_type = pwrScore;
	else 
		power_type = pwrNone;

	POWERUP* pwr = new POWERUP(power_type, count);
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	spr->LoadFromResource(res);
	pwr->SetSprite(spr);
	spr->SetX(x);
	spr->SetY(y);
	spr->SetCurrentPattern(spr->GetStartPattern());
	spr->SetZ(DEFAULT_Z);
	AddItem(pwr);
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::CreateBullet(int x, int y, bool mirror, CHARACTER* shooter)
{	
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		RESOURCE* res = shooter->GetBulletType();

	BULLET* bullet = new BULLET();
	bullet->SetDamage(res->damage);
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	bullet->SetSprite(spr);
	bullet->SetShooter(shooter);
	spr->LoadFromResource(shooter->GetBulletType());
	spr->SetX(x);
	spr->SetY(y);		
	spr->SetMirror(mirror);

	PhysDynamicBody* phys_body = new PhysDynamicBody();
	bullet->SetPhysBody(phys_body);
	phys_body->walk_acc = (Vector2(res->phys_walk_vel, 0.0));
	phys_body->max_x_vel = (res->phys_max_x_speed);
	phys_body->max_y_vel = (res->phys_max_y_speed);
	phys_body->SetCollidable(res->phys_collidable != 0);
	phys_body->SetBulletCollidable(res->bullet_collidable != 0);
	phys_body->bullet = true;
	phys_body->item = bullet;
	phys_body->gravity = (Vector2(0.0, 0.0));

	if (mirror)
	{
		phys_body->SetAABB(x+res->real_mirror_x, y + res->real_mirror_y, res->real_width, res->real_height);
		phys_body->acc = (-phys_body->walk_acc);
	}
	else
	{
		phys_body->SetAABB(x+res->real_x, y + res->real_y, res->real_width, res->real_height);
		phys_body->acc = (phys_body->walk_acc);
	}

	AddItem(bullet);
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::CreateExplosion(int x, int y, bool reverse, bool mirror)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
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
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::CreateSprite(RESOURCE* res, int x, int y)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		SPRITE* spr = new SPRITE();
	spr->SetX(x);
	spr->SetY(y);
	spr->LoadFromResource(res);

	ITEM* item = NULL;
	item = new ITEM();
	item->SetSprite((ANIMATEDSPRITE*)spr);
	item->SetType(itemSprite);

	if (res->phys_collidable != 0 || res->bullet_collidable != 0)
	{
		PhysStaticBody* phys_body = new PhysStaticBody();

		phys_body->SetAABB(x+res->real_x, y + res->real_y, res->real_width, res->real_height);

		phys_body->SetCollidable(res->phys_collidable != 0);
		phys_body->SetBulletCollidable(res->bullet_collidable != 0);
		item->SetPhysBody(phys_body);
	}

	AddItem(item);
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

void iiItemAllocator::CreateAnimSprite(RESOURCE* res, int x, int y)
{
#ifdef TRACE_RELEASE
	TRACESTART
#endif
		ITEM* item = new ITEM();
	ANIMATEDSPRITE* spr = new ANIMATEDSPRITE();
	item->SetSprite(spr);
	spr->LoadFromResource(res);
	spr->SetX(x);
	spr->SetY(y);
	spr->SetCurrentPattern(spr->GetStartPattern());
	spr->SetZ(DEFAULT_Z);
	spr->SetCollidable(false);
	item->SetType(itemAnimSprite);
	AddItem(item);
#ifdef TRACE_RELEASE
	TRACEEND
#endif
}

ITEM* iiItemAllocator::GetFirstPlayer()
{
	if(_players.size() > 0)	
		return _players[0];
	else
		return NULL;
}

void iiItemAllocator::Draw()
{
#ifdef __DEBUG_SAP_PHYSICS_
	DrawPairs();

#endif // __DEBUG_SAP_PHYSICS_

#ifdef __DEBUG_PHYSICS_
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for(vector<PhysBaseBody*>::iterator it = _phys_bodies.begin();
		it != _phys_bodies.end();
		it++)
	{
		PhysBaseBody *body = *it;
		AABB &t = (body->aabb);
		glColor3f(0.9f, 0.8f, 0.7f);
		if (body->id == 49 || body->id == 3)
			glColor3f(0.9f, 0.4f, 0.5f);

		glVertex2f(t.p.x - t.W - WORLD_X, t.p.y - t.H + WORLD_Y);
		glVertex2f(t.p.x + t.W - WORLD_X, t.p.y - t.H + WORLD_Y);

		glVertex2f(t.p.x + t.W - WORLD_X, t.p.y - t.H + WORLD_Y);
		glVertex2f(t.p.x + t.W - WORLD_X, t.p.y + t.H + WORLD_Y);

		glVertex2f(t.p.x + t.W - WORLD_X, t.p.y + t.H + WORLD_Y);
		glVertex2f(t.p.x - t.W - WORLD_X, t.p.y + t.H + WORLD_Y);

		glVertex2f(t.p.x - t.W - WORLD_X, t.p.y + t.H + WORLD_Y);
		glVertex2f(t.p.x - t.W - WORLD_X, t.p.y - t.H + WORLD_Y);
	}
	glEnd();
#endif
}

#ifdef USING_SAP_PHYSICS
#ifdef __DEBUG_SAP_PHYSICS_

void iiItemAllocator::DrawPairs()
{
	if (!pairsToDraw)
		return;
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for (UINT i = 0; i < pairsToDrawCount*2; i+=2)
		//for(Vector2* i = pairsToDraw; i < &pairs[pairsToDrawCount*2]; i++)
	{
		Vector2 v1 = pairsToDraw[i];
		Vector2 v2 = pairsToDraw[i+1];

		glColor3f(1.0f, 0.0f, 0.0f);

		glVertex2f(v1.x - WORLD_X, v1.y + WORLD_Y);
		glVertex2f(v2.x - WORLD_X, v2.y + WORLD_Y);
	}
	glEnd();
	//DELETEARRAY(pairsToDraw);
	//pairsToDrawCount = 0;

}
#endif //__DEBUG_SAP_PHYSICS_
#endif // USING_SAP_PHYSICS