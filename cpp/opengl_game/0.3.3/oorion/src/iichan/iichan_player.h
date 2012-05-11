#ifndef __PLAYER_H_
#define __PLAYER_H_
#include "iichan_character.h"

class PLAYER : public CHARACTER
{
private:
	int score;

public:
	int GetScore() const { return score; }
	void SetScore(int new_value) { score = new_value; }
	void CollectPowerup(enum POWERUP_TYPE power_type, int count);

	PLAYER(int health, int ammo, int score);

};
#endif