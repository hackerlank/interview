#ifndef __ITEM_H_
#define __ITEM_H_

#include "phys/phys_basebody.h"
#include "iichan_animatedsprite.h"



enum ITEM_TYPE {
	itemNone,
	itemPlayer,
	itemEnemy,
	itemPowerup,
	itemExplosion,
	itemBullet,
	itemSprite,
	itemShadow,
	itemAnimSprite	
};

class ITEM
{
private:
	ITEM_TYPE type;
	ANIMATEDSPRITE* sprite;
	PhysBaseBody* phys_body;
	bool dead;
	int id;
	

public:
	//bool is_new;

	void SetType(ITEM_TYPE new_type) { type = new_type; }
	ITEM_TYPE GetType() const { return type; }

	void SetSprite(ANIMATEDSPRITE* new_sprite) { sprite = new_sprite; }
	ANIMATEDSPRITE* GetSprite() const { return sprite; }

	void SetPhysBody(PhysBaseBody* new_sprite) { phys_body = new_sprite; }
	PhysBaseBody* GetPhysBody() const { return phys_body; }

	void SetDead(bool new_value) { dead = new_value; }
	bool IsDead() const { return dead; }

	int GetId() const { return id; }
	void SetId(int new_id) { id = new_id; }

	virtual void Move();

	ITEM();
};

#endif
