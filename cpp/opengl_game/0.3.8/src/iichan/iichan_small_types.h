#ifndef __ii_SMALL_TYPES_H_
#define __ii_SMALL_TYPES_H_
#include "iichan_item.h"
#include "../draw/font.h"
#include "../draw/draw.h"
//#include "iichan_character.h"



enum POWERUP_TYPE { pwrNone, pwrHealth, pwrScore, pwrAmmo };

class POWERUP : public ITEM
{
private:
	POWERUP_TYPE power_type;
	int count;

public:
	POWERUP_TYPE GetPowerType() const { return power_type; }
	void SetPowerType(enum POWERUP_TYPE new_type) { power_type = new_type; }

	int GetCount() const { return count; }
	void SetCount(int new_count) { count = new_count; }

	virtual void Move();

	POWERUP(enum POWERUP_TYPE power_type, int count);
};

class EXPLOSION : public ITEM
{
public:
	virtual void Move();

	EXPLOSION();
};

class SHADOW : public ITEM
{
public:
	virtual void Move();

	SHADOW();
};

#endif