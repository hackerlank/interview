#ifndef __ITEM_H_
#define __ITEM_H_

#include "iichan_animatedsprite.h"

enum ITEM_TYPE {
	itemPlayer,
	itemEnemy,
	itemPowerup,
	itemExplosion,
	itemBullet,
	itemSprite,
	itemShadow,
	itemNone
};

class ITEM
{
private:
	ITEM_TYPE type;
	ANIMATEDSPRITE* sprite;
	bool dead;
	int id;

public:
	void SetType(ITEM_TYPE new_type) { type = new_type; }
	ITEM_TYPE GetType() const { return type; }

	void SetSprite(ANIMATEDSPRITE* new_sprite) { sprite = new_sprite; }
	ANIMATEDSPRITE* GetSprite() const { return sprite; }

	void SetDead(bool new_value) { dead = new_value; }
	bool IsDead() const { return dead; }

	int GetId() const { return id; }
	void SetId(int new_id) { id = new_id; }

	virtual void Move();

	ITEM();
};

#endif
