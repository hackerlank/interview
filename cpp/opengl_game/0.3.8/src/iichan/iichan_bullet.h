#ifndef __ii_BULLET_H_
#define __ii_BULLET_H_
#include "iichan_item.h"
#include "iichan_character.h"

class BULLET : public ITEM
{
private:
	int speed;
	int damage;
	CHARACTER* shooter;

public:

	void SetSpeed(int new_value) { speed = new_value; }
	int GetSpeed() const { return speed; }

	void SetDamage(int new_value) { damage = new_value; }
	int GetDamage() const { return damage; }

	void SetShooter(CHARACTER* new_value) { shooter = new_value; }
	CHARACTER* GetShooter() const { return shooter; }

	virtual void Move();

	BULLET();

	bool BulletHit(const PhysBaseBody* pb);
};

#endif