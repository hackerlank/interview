#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "animatedsprite.h"

enum CHARACTERCOLOR { clr1, clr2, clr3, clr4, clrNone };

typedef struct tagCHARACTER : ANIMATEDSPRITE {
	int health;
	int ammo;



	bool shooting;
	int shoots;

	CHARACTERCOLOR color;




	bool dead;

	float weight;

	void Shoot();

	float GetWeight() const { return weight; }
	void SetWeight(float new_value) { weight = new_value; }

	CHARACTERCOLOR GetColor() const { return color; }
	void SetColor(enum CHARACTERCOLOR new_value) { color = new_value; }

	bool GetDead() const { return dead; }
	void SetDead(bool new_value) { dead = new_value; }




	bool GetShooting() const { return shooting; }
	void SetShooting(bool new_value) { shooting = new_value; }
	int GetShoots() const { return shoots; }
	void SetShoots(int new_value) { shoots = new_value; }

	int GetHealth() const { return health; }
	void SetHealth(int new_value) { health = new_value; }

	int GetAmmo() const { return ammo; }
	void SetAmmo(int new_value) { ammo = new_value; }
	tagCHARACTER()
	{
		__super::tagANIMATEDSPRITE();
		SetShoots(0);
		SetJumping(false);
		SetShooting(false);
		SetJumpDirection(false);
		SetDead(false);
		SetYSpeed(0);
		SetColor(clrNone);
		SetWeight(0);
	}

} CHARACTER;

#endif