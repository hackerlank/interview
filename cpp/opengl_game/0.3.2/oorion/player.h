#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "character.h"

typedef struct tagPLAYER : CHARACTER {
	int score;

	int GetScore() const { return score; }
	void SetScore(int new_value) { score = new_value; }

	void CollectPowerUp(enum POWERUP_TYPE type, int count);

	tagPLAYER(int health, int ammo,	int score)
	{
		__super::tagCHARACTER();
		SetHealth(health);
		SetAmmo(ammo);
		SetScore(score);
	}

} PLAYER;
#endif