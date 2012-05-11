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

	// TODO: Переделать для сложного HUD'а (если его вообще использовать будут)
	void SetHealthCount(int new_value) { healthCount = new_value; }
	int GetHealthCount() const { return healthCount; } 

	void SetAmmoCount(int new_value) { ammoCount = new_value; }
	int GetAmmoCount() const { return ammoCount; } 

	void SetScoreCount(int new_value) { scoreCount = new_value; }
	int GetScoreCount() const { return scoreCount; } 

	HUD();
	~HUD();
};

#endif