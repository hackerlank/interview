#ifndef __ii_ITEMALLOC_H_
#define __ii_ITEMALLOC_H_

#include "iichan_item.h"
#include "iichan_hud.h"
#include "iichan_spritealloc.h"
#include "iichan_instancealloc.h"
#include "iichan_level.h"
#include "iichan_character.h"
#include "iichan_bullet.h"
#include "iichan_player.h"
#include "iichan_enemy.h"

#ifdef USING_SAP_PHYSICS
#include "phys/sap/SweepAndPrune.h"
using namespace Opcode;
//#include "phys/sap/OPC_ArraySAP.h"
#endif // USING_SAP_PHYSICS

class iiItemAllocator
{
private:
	HUD* hud;
	iiSpriteAllocator* SpriteAllocator;
	iiInstanceAllocator* InstanceAllocator;
	iiScreenTextAllocator* ScreenTextAllocator;

#ifdef USING_SAP_PHYSICS
	ArraySAP physSAP;
#ifdef __DEBUG_SAP_PHYSICS_
	Vector2* pairsToDraw;
	UINT pairsToDrawCount;
	void DrawPairs();
#endif // __DEBUG_SAP_PHYSICS_
#endif // USING_SAP_PHYSICS

	vector<ITEM*> items;
	int _count; // current count of items

	vector<PLAYER*> _players;
	vector<ENEMY*> _enemies; // cached enemies id's
	vector<POWERUP*> _powerups; // cached powerups id's
	vector<BULLET*> _bullets;
	vector<EXPLOSION*> _explosions;
	vector<PhysBaseBody*> _phys_bodies;
	//vector<int> _player_floors; // cached players floor id's

	// Возвращает true если позиция героя изменилась с последнего вызова функции
	bool GetPlayerPosState(); // TODO: несколько героев

	unsigned int _prev_tick; // previous move
	unsigned int _prev_player_tick;
	float _lowest_player_speed;
	unsigned int _player_tick;

	void ProcessPlayer(ITEM* item);
	void ProcessPowerup(ITEM* item);
	void ProcessEnemy(ITEM* item);
	void ProcessExplosion(ITEM* item);
	void ProcessSprite(ITEM* item);
	void ProcessBullet(ITEM* item);


	void ProcessAndMovePhysBody(ITEM* item);
	void MoveSprite(ITEM* item);



	iiLevel* current_level;

	void iiItemAllocator::BatchCreate();
	void iiItemAllocator::BatchRemove();
	vector<ITEM*> _created;
	vector<ITEM*> _removed;


public:

	void SetHUD(HUD* new_hud) { hud = new_hud; }
	HUD* GetHUD() const { return hud; }

	void AddItem(ITEM* item);
	ITEM* GetItem(int id);

	void DeleteItem(ITEM* item);

	void SetSpriteAllocator(iiSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	iiSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	void SetInstanceAllocator(iiInstanceAllocator* new_value)
	{ InstanceAllocator = new_value; }
	iiInstanceAllocator* GetInstanceAllocator() const
	{ return InstanceAllocator; }

	void SetScreenTextAllocator(iiScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	iiScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	// GAME INTERFACE
	// ----------------------------------------------------------------------
	void CreatePowerup(RESOURCE* res, int count, int x, int y);
	void CreateExplosion(int x, int y, bool reverse, bool mirror);
	void CreateEnemy(RESOURCE* res, int x, int y, int health);
	void CreateBullet(int x, int y, bool mirror, CHARACTER* shooter);
	void CreateSprite(RESOURCE* res, int x, int y);
	void CreateAnimSprite(RESOURCE* res, int x, int y);
	void CreatePlayer(RESOURCE* res, int health, int ammo, int score,
		int x, int y);

	ITEM* GetFirstPlayer();
	
	//TODO: LevelAllocator?
	bool LoadLevel(string file_name);
	iiLevel* GetCurrentLevel() const { return current_level; }

	// ----------------------------------------------------------------------


	void Move();
	void Draw();
	iiItemAllocator();
	~iiItemAllocator();
};

#endif