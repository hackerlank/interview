#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "iichan_item.h"

enum CHARACTERCOLOR { clr1, clr2, clr3, clr4, clrNone };

class CHARACTER : public ITEM
{
private:
	int health;
	int ammo;
	bool shooting;
	int shoots;
	CHARACTERCOLOR color;
	float weight;

public:
	void Shoot();
	float GetWeight() const { return weight; }
	void SetWeight(float new_value) { weight = new_value; }
	bool GetShooting() const { return shooting; }
	void SetShooting(bool new_value) { shooting = new_value; }
	int GetShoots() const { return shoots; }
	void SetShoots(int new_value) { shoots = new_value; }
	int GetHealth() const { return health; }
	void SetHealth(int new_value) { health = new_value; }
	int GetAmmo() const { return ammo; }
	void SetAmmo(int new_value) { ammo = new_value; }
	CHARACTER();
};

#endif