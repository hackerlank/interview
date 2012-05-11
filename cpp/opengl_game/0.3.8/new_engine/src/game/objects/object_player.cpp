#include "StdAfx.h"

#include "../../misc.h"

#include "../camera.h"

#include "object_player.h"
#include "../../config.h"
#include "../phys/sap/SweepAndPrune.h"

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ObjPlayer* _PLAYER = NULL;			// Указатель на объект игрока для быстрого поиска

#ifdef TWO_CHANGABLE_PLAYERS
ObjPlayer* _PLAYER_ONE = NULL;
ObjPlayer* _PLAYER_TWO = NULL;

int on_change_player_processor = LUA_NOREF;

bool player_change_blocked = false;

#endif // TWO_CHANGABLE_PLAYERS


//////////////////////////////////////////////////////////////////////////

BOOL LoadPlayerFromProto(Proto* proto, ObjPlayer* player)
{
	player->weapon = CreateWeapon(proto->main_weapon);
	player->alt_weapon = CreateWeapon(proto->alt_weapon);

	player->cur_weapon = player->weapon;

	if (proto->health > 0)
		player->health = (SHORT)proto->health;

	return FALSE;
}

ObjPlayer* CreatePlayer(Proto* proto, Vector2 coord, bool overr_z, float z)
{
	if (!proto)
		return NULL;
#ifdef TWO_CHANGABLE_PLAYERS
	if (_PLAYER_ONE && _PLAYER_TWO)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Уже созданы два игрока");
		return NULL;
	}
#endif // TWO_CHANGABLE_PLAYERS

	ObjPlayer* player = new ObjPlayer;
	if (LoadObjectFromProto(proto, (GameObject*)player) || 
		LoadPlayerFromProto(proto, player) || 
		CheckAnimations((GameObject*)player, objPlayer))
	{
		DELETESINGLE(player);
		return NULL;
	}

	player->aabb.p = coord;

	// TODO: Кривой хак. Хоя вероятно и перерастет в постояный. Но лучше ничего не придумал.
	// В этот момент объектв в SAP еще не добавлен, поэтому в SetAnimation нельзя еще обновлять
	// его состояние в SAP.
	player->ClearPhysic();
	SetAnimation((GameObject*)player, "idle",true, true);
	player->SetPhysic();

	if (overr_z)
		player->sprite->z = z;

	AddObject(player);

#ifdef TWO_CHANGABLE_PLAYERS
	if (!_PLAYER_ONE)
	{
		_PLAYER = player;
		_PLAYER_ONE = player;
	}
	else
	{
		_PLAYER_TWO = player;
		player->sprite->ClearVisible();
		player->SetSleep();
		player->ClearBulletCollidable();
	}
#else
	_PLAYER = player;
#endif // TWO_CHANGABLE_PLAYERS
	
	return player;
}

ObjPlayer* CreatePlayer(const char* proto_name, Vector2 coord)
{
	if (!proto_name)
		return NULL;

	return CreatePlayer(GetProto(proto_name), coord, false, 0);
}
ObjPlayer* CreatePlayer(const char* proto_name, Vector2 coord, float z)
{
	if (!proto_name)
		return NULL;

	return CreatePlayer(GetProto(proto_name), coord, true, z);
}

//////////////////////////////////////////////////////////////////////////

ObjPlayer::~ObjPlayer()
{
	if (this == _PLAYER)
		_PLAYER = NULL;

#ifdef TWO_CHANGABLE_PLAYERS
	if (this == _PLAYER_ONE)
		_PLAYER_ONE = NULL;
	else if (this == _PLAYER_TWO)
		_PLAYER_TWO = NULL;

	SCRIPT::RemoveFromRegistry(on_change_player_processor);
	on_change_player_processor = LUA_NOREF;
#endif // TWO_CHANGABLE_PLAYERS

	DELETESINGLE(alt_weapon);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

extern config cfg;

//#define KEYDOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)


extern bool keys[256];

#define KEYPRESSED(vk_code) keys[vk_code]

#define PRESSED_FIRE KEYPRESSED(cfg.fire)
#define PRESSED_LEFT KEYPRESSED(cfg.left)
#define PRESSED_RIGHT KEYPRESSED(cfg.right)
#define PRESSED_JUMP KEYPRESSED(cfg.jump)
#define PRESSED_SIT KEYPRESSED(cfg.sit)
#define PRESSED_DOWN KEYPRESSED(cfg.down)
#define PRESSED_UP KEYPRESSED(cfg.up)

#ifdef TWO_CHANGABLE_PLAYERS
#define PRESSED_CHANGE_PLAYER KEYPRESSED(cfg.change_player)
#endif // TWO_CHANGABLE_PLAYERS

#define PRESSED_CHANGE_WEAPON KEYPRESSED(cfg.change_weapon)

#define PLAYER_STILL_WALKING_LEFT (this->movement == omtWalking && !this->movementDirectionX)
#define PLAYER_STILL_WALKING_RIGHT (this->movement == omtWalking && this->movementDirectionX)
#define PLAYER_STILL_AIMING_UP (this->movement == omtAimingUp || this->movement == omtWalkingAimingUp)

//#define PRESSED_FIRE KEYDOWN(cfg.fire)
//#define PRESSED_LEFT KEYDOWN(cfg.left)
//#define PRESSED_RIGHT KEYDOWN(cfg.right)
//#define PRESSED_JUMP KEYDOWN(cfg.jump)
//#define PRESSED_DOWN KEYDOWN(cfg.down)
//#define PRESSED_UP KEYDOWN(cfg.up)
//#define PLAYER_STILL_WALKING_LEFT (this->movement == omtWalking && !this->movementDirectionX)
//#define PLAYER_STILL_WALKING_RIGHT (this->movement == omtWalking && this->movementDirectionX)
//#define PLAYER_STILL_AIMING_UP (this->movement == omtAimingUp || this->movement == omtWalkingAimingUp)



void ObjPlayer::Process()
{
#ifdef TWO_CHANGABLE_PLAYERS
	if (this != _PLAYER)
		return;
#endif // TWO_CHANGABLE_PLAYERS	

	if (IsSleep())
		return;

	Sprite* s = this->sprite;
	ASSERT(s);

	ObjectMovementType last_movement = this->movement;
	gunDirection = cgdNone;

	if (this->health <= 0)
	{
		activity = oatDying;
	}
	if (activity == oatDying)
	{
		movement = omtIdling;
		SetAnimation((GameObject*)this, "die", false, false);
		if (s->IsAnimDone())
		{
			this->SetDead();
		}
		return;
	}

	// Обработка нажатий на клавиши
	// TODO: Обернуть потом в проверку на включеность клавиатуры/ввода игрока
	float dvx = 0.0f;	// Изменение скорости, применится в конце в зависимости от типа движения

	//if (PRESSED_FIRE && this->controlEnabled) 
	//	this->activity = oatShooting;

	if (movement == omtJumping && IsOnPlane()) 
	{
		//	movement = omtLanding;
		movement = omtIdling;
	}

	if (this->controlEnabled)
	{
		if (PRESSED_FIRE) 
			this->activity = oatShooting;
		else if (this->activity == oatShooting)
			this->activity = oatIdling;

		if (PRESSED_SIT && movement != omtJumping) 
			movement = omtSitting;

		if (PRESSED_UP && !PRESSED_DOWN)
			gunDirection = cgdUp;

		if (PRESSED_DOWN && !PRESSED_UP)
			gunDirection = cgdDown;

		if (PRESSED_JUMP && /*!PRESSED_DOWN &&*/ IsOnPlane() && movement != omtLanding)
		{
			movement = omtJumping;
			this->vel.y -= this->jump_vel;
			this->ClearOnPlane();
		}

		if (!PRESSED_SIT && movement == omtSitting) 
			movement = omtIdling;


		if (PRESSED_RIGHT && !PRESSED_LEFT)
		{
			dvx += walk_acc * (IsOnPlane() ? 0.5f : 1);
			movementDirectionX = true;
		}
		if (PRESSED_LEFT && !PRESSED_RIGHT)
		{
			dvx -= walk_acc * (IsOnPlane() ? 0.5f : 1);
			movementDirectionX = false;
		}

		

		// Пока не будет улучшений для клавиатуры, будет использоваться
		// вот такой костыль. Хотя работает не плохо.
		// Но для подобных штук плодить такие костыли - уныло.
		static bool ready_for_change_weapon = false;
		if (PRESSED_CHANGE_WEAPON)
			ready_for_change_weapon = true;

		if (ready_for_change_weapon && !PRESSED_CHANGE_WEAPON)
		{
			this->ChangeWeapon();
			if (activity == oatShooting)
				activity = oatIdling;
			ready_for_change_weapon = false;
		}
	}


	// Различные проверки для определения типа передвижения
	if (movement != omtSitting && movement != omtJumping && 
		movement != omtLanding && 
		!IsNear(vel.x, 0, 0.1))
	{
		movement = omtWalking;
	}

	if(this->controlEnabled &&  
		((!PRESSED_LEFT && PLAYER_STILL_WALKING_LEFT) ||
		(!PRESSED_RIGHT && PLAYER_STILL_WALKING_RIGHT)/* ||
		(!PRESSED_UP && PLAYER_STILL_AIMING_UP))*/))
	{
		movement = omtIdling;
	}

	if (!IsOnPlane() && movement != omtJumping) 
		movement = omtJumping;

	// Запуск анимаций
	if (this->activity == oatShooting)
	{
		if (cur_weapon && cur_weapon->IsReady() &&
			!((this->sprite->cur_anim == "gunliftaimup" || this->sprite->cur_anim == "gunliftaimdown") && !this->sprite->IsAnimDone()))
		{
			if (movement == omtSitting)
			{
				SetAnimation((GameObject*)this, "sitshoot", false, false);
				//CreateBullet("bullet", this->aabb.p, (ObjCharacter*)this, (movementDirectionX ? wdRight : wdLeft));
				cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdRight : wdLeft);
			}
			else if (movement == omtWalking)
			{
				//SetAnimation((GameObject*)this, "shoot", false, false);
				if (this->gunDirection == cgdUp)
				{					
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdUpRight : wdUpLeft);
				}
				else if (this->gunDirection == cgdDown)
				{
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdDownRight : wdDownLeft);
				}
				else
				{
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdRight : wdLeft);
				}
			}
			else if (movement == omtJumping)
			{
				if (this->gunDirection == cgdDown)
				{
					SetAnimation((GameObject*)this, "jumpgunaimdownshoot", false, false);
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdDownRight : wdDownLeft);
				}
				else if (this->gunDirection == cgdUp)
				{
					SetAnimation((GameObject*)this, "jumpgunaimupshoot", false, false);
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdUpRight : wdUpLeft);
				}
				else
				{
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdRight : wdLeft);
				}
			}
			else if (movement != omtLanding/* && movement != omtJumping*/)
			{		

				if (this->gunDirection == cgdUp)
				{
					SetAnimation((GameObject*)this, "gunaimupshoot", false, true);
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdUpRight : wdUpLeft);
				}
				else if (this->gunDirection == cgdDown)
				{
					SetAnimation((GameObject*)this, "gunaimdownshoot", false, true);
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdDownRight : wdDownLeft);
				}
				else
				{
					SetAnimation((GameObject*)this, "shoot", false, true);
					cur_weapon->Fire(this, this->aabb.p, movementDirectionX ? wdRight : wdLeft);
				}
			}

			//this->gunReloadTick = this->gunReloadTime;
		}

		if (s->IsAnimDone())
		{
			// Анимация закончилась - выстрел завершен
			activity = oatIdling;
		}
	}

	if (this->movement == omtWalking)
	{		
		if (gunDirection == cgdUp)
		{
			SetAnimation((GameObject*)this, "walkgunaimup", true, false);
		}
		else if (gunDirection == cgdDown)
		{
			SetAnimation((GameObject*)this, "walkgunaimdown", true, false);
		}
		else
		{
			SetAnimation((GameObject*)this, "walk", true, false);
		}
	}	
	else if (this->movement == omtJumping)
	{
		if (gunDirection == cgdDown)
		{
			if (this->sprite->cur_anim == "jumpgunliftaimdown" && this->sprite->IsAnimDone())
				SetAnimation((GameObject*)this, "jumpgunaimdown", false, false);
			
			if (this->sprite->cur_anim == "jumpgunaimdownshoot")
			{
				if (this->sprite->IsAnimDone())
					SetAnimation((GameObject*)this, "jumpgunaimdown", false, false);
			}
			else if (this->sprite->cur_anim != "jumpgunaimdown")
				SetAnimation((GameObject*)this, "jumpgunliftaimdown", false, false);
			
		}
		else if (gunDirection == cgdUp)
		{
			if (this->sprite->cur_anim == "jumpgunliftaimup" && this->sprite->IsAnimDone())
				SetAnimation((GameObject*)this, "jumpgunaimup", false, false);

			if (this->sprite->cur_anim == "jumpgunaimupshoot")
			{
				if (this->sprite->IsAnimDone())
					SetAnimation((GameObject*)this, "jumpgunaimup", false, false);
			}
			else if (this->sprite->cur_anim != "jumpgunaimup")
				SetAnimation((GameObject*)this, "jumpgunliftaimup", false, false);

		}
		else
		{
			SetAnimation((GameObject*)this, "jump", false, false);			
		}
	}
	else if (this->movement == omtLanding)
	{
		SetAnimation((GameObject*)this, "land", false, false);
		if (s->IsAnimDone())
		{
			// Анимация закончилась - мы приземлились
			movement = omtIdling;
		}

	}
	else if (this->movement == omtSitting)
	{
		if (this->activity != oatShooting)
			SetAnimation((GameObject*)this, "sit", true, false);
	}

	if (this->movement == omtIdling /*&& !PRESSED_FIRE*//*this->activity != oatShooting*/)
	{
		if (gunDirection == cgdUp)
		{
			if (this->sprite->cur_anim == "gunliftaimup" && this->sprite->IsAnimDone())
				SetAnimation((GameObject*)this, "gunaimup", false, false);

			if (this->sprite->cur_anim == "gunaimupshoot")
			{
				if (this->sprite->IsAnimDone())
					SetAnimation((GameObject*)this, "gunaimup", false, false);
			}
			else if (this->sprite->cur_anim != "gunaimup")
				SetAnimation((GameObject*)this, "gunliftaimup", false, false);

			if (last_movement == omtWalking)
				this->sprite->SetLastAnimationFrame();
		}
		else if (gunDirection == cgdDown)
		{
			if (this->sprite->cur_anim == "gunliftaimdown" && this->sprite->IsAnimDone())
				SetAnimation((GameObject*)this, "gunaimdown", false, false);

			if (this->sprite->cur_anim == "gunaimdownshoot")
			{
				if (this->sprite->IsAnimDone())
					SetAnimation((GameObject*)this, "gunaimdown", false, false);
			}
			else if (this->sprite->cur_anim != "gunaimdown")
				SetAnimation((GameObject*)this, "gunliftaimdown", false, false);

			if (last_movement == omtWalking)
				this->sprite->SetLastAnimationFrame();
		}
		else if (PRESSED_FIRE)
		{
			if (!(this->sprite->cur_anim == "shoot" && !this->sprite->IsAnimDone()))
				SetAnimation((GameObject*)this, "aim", true, false);
		}
		else
		{
			SetAnimation((GameObject*)this, "idle", true, false);
		}
	}

	


	if (movement != omtSitting && movement != omtLanding)
	{
		vel.x += dvx;
	}

	// Разворачиваем спрайт по направлению
	if (movementDirectionX)
		s->ClearMirrored();
	else
		s->SetMirrored();

}

// Переключает основное и альтернативное оружие
void ObjPlayer::ChangeWeapon()
{
	if (alt_weapon && cur_weapon == this->weapon)
		cur_weapon = alt_weapon;
	else
		cur_weapon = weapon;
}

//////////////////////////////////////////////////////////////////////////

#ifdef TWO_CHANGABLE_PLAYERS



void PlayerChanger()
{
	if (!(_PLAYER && _PLAYER_ONE && _PLAYER_TWO))
		return;

	if (player_change_blocked)
		return;

	// Пока не будет улучшений для клавиатуры, будет использоваться
	// вот такой костыль. Хотя работает не плохо.
	// Но для подобных штук плодить такие костыли - уныло.
	static bool ready_for_change_player = false;
	if (PRESSED_CHANGE_PLAYER)
		ready_for_change_player = true;

	if (ready_for_change_player && !PRESSED_CHANGE_PLAYER)
	{
		ObjPlayer* old = _PLAYER;
		if (_PLAYER == _PLAYER_ONE)
			_PLAYER = _PLAYER_TWO;
		else
			_PLAYER = _PLAYER_ONE;

		_PLAYER->aabb.p = old->aabb.p;
		_PLAYER->old_aabb = old->old_aabb;
		_PLAYER->vel = old->vel;
		_PLAYER->acc = old->acc;

		_PLAYER->controlEnabled = old->controlEnabled;
		_PLAYER->gravity = old->gravity;
		_PLAYER->movement = old->movement;

		old->IsOnPlane() ? _PLAYER->SetOnPlane() : _PLAYER->ClearOnPlane();

		old->IsBulletCollidable() ? 
			_PLAYER->SetBulletCollidable() : 
			_PLAYER->ClearBulletCollidable();

		old->ClearBulletCollidable();
		
		_PLAYER->movementDirectionX = old->movementDirectionX;
		/*old->sprite->IsMirrored() ? 
			_PLAYER->sprite->SetMirrored() : 
			_PLAYER->sprite->ClearMirrored();*/

		_PLAYER->sprite->SetVisible();
		old->sprite->ClearVisible();	

		_PLAYER->ClearSleep();
		old->SetSleep();

		if (GetCameraAttachedObject() == old)
			CameraAttachToObject(_PLAYER);

		ready_for_change_player = false;
	
		if (on_change_player_processor >= 0)
		{
			if (SCRIPT::ExecChunkFromReg(on_change_player_processor))
			{
				// В скрипте произошла какая-то ошибка. Будем испльзовать стандартный в следующий раз.
				SCRIPT::RemoveFromRegistry(on_change_player_processor);
				on_change_player_processor = LUA_NOREF;
			}
		}
			

	}
}

void BlockPlayerChange()
{
	player_change_blocked = true;
}

void UnblockPlayerChange()
{
	player_change_blocked = false;
}

#endif // TWO_CHANGABLE_PLAYERS