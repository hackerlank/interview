#ifndef __ii_LEVEL_H_
#define __ii_LEVEL_H_

#include "../misc.h"

/*

      x,y _________width
         /        /
        /-plank--/
	   /        /
      /________/
   height
*/

struct iiObj {
	int x;
	int y;
	string res;
};

struct iiTile : iiObj {
	int width;
	int height;
	int plank;
	int gravity;
};

struct iiPlayer : iiObj  {
	int health;
	int ammo;
	int score;
	// anything else
	// 
};

struct iiPowerup : iiObj  {
	int type;
	int power;
};

struct iiEnemy : iiObj {
	int type;
	int health;
};

struct iiLevel {
	vector<iiTile> tiles;
	vector<iiPlayer> players;
	vector<iiPowerup> powerups;
	vector<iiEnemy> enemies;
};

struct mnum
{
	short n1;
	short n2;
};

struct coord
{
	unsigned __int32 x;
	unsigned __int32 y;
};

struct obj
{
	//char *name;
	string name;
	unsigned __int32 count;
	coord* c;
	mnum* m;
	int *ob_id;
};



#endif