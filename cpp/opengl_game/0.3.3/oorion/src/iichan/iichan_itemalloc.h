#ifndef __IICHAN_ITEMALLOC_H_
#define __IICHAN_ITEMALLOC_H_

#include "iichan_item.h"
#include "iichan_hud.h"
#include "iichan_spritealloc.h"

class IICHANItemAllocator
{
private:
	HUD* hud;
	IICHANSpriteAllocator* SpriteAllocator;

	int _count; // current count of items

	vector<int> _players; // cached players id's
	vector<int> _enemies; // cached enemies id's
	vector<int> _powerups;

	// removes an id from cached vector
	void _remove_cached_value(vector<int>* v, int i)
	{
		for(UINT a = 0; a < v->size(); a++)
			if(v->at(a) == i)
				v->erase(v->begin() + a);
	}

	int _prev_tick; // previous move

	void ProcessPlayer(ITEM* item);
	void ProcessPowerup(ITEM* item);
	void ProcessEnemy(ITEM* item);
	void ProcessExplosion(ITEM* item);
	void ProcessSprite(ITEM* item);
	void ProcessBullet(ITEM* item);

public:
	vector<ITEM*> items;

	void SetHUD(HUD* new_hud) { hud = new_hud; }
	HUD* GetHUD() const { return hud; }

	void AddItem(ITEM* item);
	ITEM* GetItem(int id);
	int GetIndexById(int id);

	void DeleteItem(ITEM* item);

	void SetSpriteAllocator(IICHANSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	IICHANSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	// GAME INTERFACE
	// ----------------------------------------------------------------------
	void CreatePowerup(OBJECT_RES* res, enum POWERUP_TYPE power_type, int count,
		int x, int y);
	void CreateExplosion(int x, int y, bool reverse, bool mirror);
	void CreateEnemy(OBJECT_RES* res, int x, int y, int health);
	void CreateBullet(int x, int y, bool mirror);
	void CreateSprite(OBJECT_RES* res, int x, int y);

	ITEM* GetFirstPlayer();
	// ----------------------------------------------------------------------


	void Move();

	IICHANItemAllocator();
	~IICHANItemAllocator();
};

#endif