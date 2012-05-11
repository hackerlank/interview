#ifndef __HUD_H_
#define __HUD_H_

#include "iichan_sprite.h"

class HUD;

class HUD {
private:
#ifdef SIMPLE_HUD
	SPRITE* health;
	SPRITE* ammo;
	SPRITE* score;
#else
	vector<SPRITE*> health;
	vector<SPRITE*> ammo;
	vector<SPRITE*> score;
#endif
	int healthCount;
	int ammoCount;
	int scoreCount;
public:
	void AddHealth();
	void AddAmmo();
	void AddScore();

	void RemoveHealth();
	void RemoveAmmo();
	void RemoveScore();

	void Draw();

	HUD();
	~HUD();
};

#endif