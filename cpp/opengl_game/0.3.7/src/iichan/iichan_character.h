#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "iichan_item.h"
#include "iichan_small_types.h"

enum CHARACTER_AI {aiHuman, aiOn, aiOff };

class CHARACTER : public ITEM
{
private:
	int health;
	int ammo;
	int shoots;
	int vuln_ticks;
	bool vulnerable;
	CHARACTER_AI ai;
	SHADOW* shadow;

public:
	void Shoot();
	void Ouch();

	int GetShoots() const { return shoots; }
	void SetShoots(int new_value) { shoots = new_value; }

	int GetHealth() const { return health; }
	void SetHealth(int new_value) { health = new_value; }

	int GetAmmo() const { return ammo; }
	void SetAmmo(int new_value) { ammo = new_value; }

	bool GetVulnerable() const { return vulnerable; }
	void SetVulnerable(bool new_value) { vulnerable = new_value; }

	int GetVulnTicks() const { return vuln_ticks; }
	void SetVulnTicks(int new_value) { vuln_ticks = new_value; }

	CHARACTER_AI GetAI() const { return ai; }
	void SetAI(CHARACTER_AI new_value) { ai = new_value; }

	SHADOW* GetShadow() const { return shadow; }
	void SetShadow(SHADOW* new_value) { shadow = new_value; }

	CHARACTER();

	virtual void Move();
};

#endif