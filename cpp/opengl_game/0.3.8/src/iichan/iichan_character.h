#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "iichan_item.h"
#include "iichan_small_types.h"
#include "iichan_resource.h"

class CHARACTER : public ITEM
{
private:
	int health;
	int ammo;
	int shoots;
	int vuln_ticks;
	bool vulnerable;
	SHADOW* shadow;
	RESOURCE* bullet_type;
	string shoot_sound;
	int reload_speed;

public:
	void Shoot();
	void Ouch(int damage);

	int GetShoots() const { return shoots; }
	void SetShoots(int new_value) { shoots = new_value; }

	int GetReloadSpeed() const { return reload_speed; }
	void SetReloadSpeed(int new_value) { reload_speed = new_value; }

	int GetHealth() const { return health; }
	void SetHealth(int new_value) { health = new_value; }

	int GetAmmo() const { return ammo; }
	void SetAmmo(int new_value) { ammo = new_value; }

	bool GetVulnerable() const { return vulnerable; }
	void SetVulnerable(bool new_value) { vulnerable = new_value; }

	int GetVulnTicks() const { return vuln_ticks; }
	void SetVulnTicks(int new_value) { vuln_ticks = new_value; }

	SHADOW* GetShadow() const { return shadow; }
	void SetShadow(SHADOW* new_value) { shadow = new_value; }

	RESOURCE* GetBulletType() const { return bullet_type; }
	void SetBulletType(RESOURCE* new_value) { bullet_type = new_value; }

	string GetShootSound() const { return shoot_sound; }
	void SetShootSound(string new_value) { shoot_sound = new_value; }

	CHARACTER();

	virtual void Move();
};

#endif