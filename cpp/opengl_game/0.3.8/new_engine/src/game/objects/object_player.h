#ifndef __OBJECT_PLAYER_H_
#define __OBJECT_PLAYER_H_

#include "object_character.h"

class ObjPlayer : public ObjCharacter
{
public:
	//USHORT health;
	bool controlEnabled;			// Может ли игрок управлять	
	Weapon* alt_weapon;				// Альтернативное оружие
	Weapon* cur_weapon;				// Текущее используемое оружие

	ObjPlayer()
	{
		this->controlEnabled = true;
		alt_weapon = NULL;
		cur_weapon = NULL;

		this->type = objPlayer;
		this->gravity = Vector2(0, 0.8);
		this->health = 50;	
	}

	~ObjPlayer();

	virtual void Process();
	void ChangeWeapon();
};

ObjPlayer* CreatePlayer(const char* proto_name, Vector2 coord);
ObjPlayer* CreatePlayer(const char* proto_name, Vector2 coord, float z);

#ifdef TWO_CHANGABLE_PLAYERS
void PlayerChanger();

void BlockPlayerChange();
void UnblockPlayerChange();

#endif // TWO_CHANGABLE_PLAYERS

#endif // __OBJECT_PLAYER_H_