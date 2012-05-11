#ifndef __IICHAN_SMALL_TYPES_H_
#define __IICHAN_SMALL_TYPES_H_
#include "iichan_item.h"
#include "../draw/font.h"
#include "../draw/draw.h"

class BULLET : public ITEM
{
private:
	int life;
	int speed;
	int damage;

public:
	void SetLife(int new_value) { life = new_value; }
	int GetLife() const { return life; }

	void SetSpeed(int new_value) { speed = new_value; }
	int GetSpeed() const { return speed; }

	void SetDamage(int new_value) { damage = new_value; }
	int GetDamage() const { return damage; }

	virtual void Move();

	BULLET();
};

enum POWERUP_TYPE { pwrHealth, pwrScore, pwrAmmo, pwrNone };

class POWERUP : public ITEM
{
private:
	POWERUP_TYPE power_type;
	int count;
	bool dead;

public:
	POWERUP_TYPE GetPowerType() const { return power_type; }
	void SetPowerType(enum POWERUP_TYPE new_type) { power_type = new_type; }

	int GetCount() const { return count; }
	void SetCount(int new_count) { count = new_count; }

	bool GetDead() const { return dead; }
	void SetDead(bool new_dead) { dead = new_dead; }

	virtual void Move();

	POWERUP(enum POWERUP_TYPE power_type, int count);
};

class EXPLOSION : public ITEM
{
private:
	bool dead;

public:
	bool GetDead() const { return dead; }
	void SetDead(bool new_dead) { dead = new_dead; }

	virtual void Move();

	EXPLOSION();
};

#endif