#ifndef __IICHAN_SMALL_TYPES_H_
#define __IICHAN_SMALL_TYPES_H_
#include "animatedsprite.h"
#include "font.h"
#include "draw.h"

typedef struct tagBULLET : SPRITE {
	int life;
	int speed;
	int damage;

	void SetLife(int new_value) { life = new_value; }
	int GetLife() const { return life; }

	void SetSpeed(int new_value) { speed = new_value; }
	int GetSpeed() const { return speed; }

	void SetDamage(int new_value) { damage = new_value; }
	int GetDamage() const { return damage; }
} BULLET;

enum POWERUP_TYPE { pwrHealth, pwrScore, pwrAmmo, pwrNone };

typedef struct tagPOWERUP : ANIMATEDSPRITE {
	POWERUP_TYPE type;
	int count;
	bool dead;

	POWERUP_TYPE GetType() const { return type; }
	void SetType(enum POWERUP_TYPE new_type) { type = new_type; }

	int GetCount() const { return count; }
	void SetCount(int new_count) { count = new_count; }

	bool GetDead() const { return dead; }
	void SetDead(bool new_dead) { dead = new_dead; }

	tagPOWERUP(enum POWERUP_TYPE type, int count);
} POWERUP;

typedef struct tagPHRASE : SPRITE {
	string text;
	int time;
	bool dead;

	string GetText() const { return text; }
	void SetText(string new_value) { text = new_value; }

	int GetTime() const { return time; }
	void SetTime(int new_value) { time = new_value; }

	bool GetDead() const { return dead; }
	void SetDead(bool new_value) { dead = new_value; }

	virtual void Draw();

	tagPHRASE(string text, int time)
	{
		SetText(text);
		SetTime(time);
		SetDead(false);
	}
} PHRASE;

typedef struct tagEXPLOSION : ANIMATEDSPRITE {
	bool dead;

	bool GetDead() const { return dead; }
	void SetDead(bool new_dead) { dead = new_dead; }

	virtual void Move()
	{

		if(!GetReverse())
		{		
			if(GetCurrentPattern() - GetStartPattern() == GetPatternCount() - 1)
			{
				SetDead(true);
			}
			else
				SetCurrentPattern(GetCurrentPattern() + 1);
		}
		else
		{
			if(GetStartPattern() - GetCurrentPattern() == GetPatternCount()-1)
			{
				SetDead(true);
			}
			else
			{
				if(GetCurrentPattern() == 0)
					SetCurrentPattern(GetStartPattern());
				else
					SetCurrentPattern(GetCurrentPattern() - 1);
			}
		}
	}
	tagEXPLOSION()
	{
		__super::tagANIMATEDSPRITE();
	}
} EXPLOSION;

#endif