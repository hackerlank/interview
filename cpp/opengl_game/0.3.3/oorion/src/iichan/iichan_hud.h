#ifndef __HUD_H_
#define __HUD_H_

#include "iichan_sprite.h"

class HUD;

class HUD {
private:
	vector<SPRITE*> health;
	vector<SPRITE*> ammo;
	vector<SPRITE*> score;
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