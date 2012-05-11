#ifndef __RIBBON_H_
#define  __RIBBON_H_

#include "sprite.h"

class Ribbon
{
public:
	Sprite* sprite;		// Спрайт ленты
	CAABB aabb;			// Положени спрайта ленты

	float speed_factor;	// Фактор скорости

	bool use_bounds;	// Надо ли использовать границы

	// Координаты сторон границ
	// left, right, top, bottom
	float bl, br, bt, bb;	

	Ribbon()
	{
		sprite = NULL;
		speed_factor = 0;
		use_bounds = false;
		bl = br = bt = bb = 0;
	}

	void Draw();
};



void DrawRibbons();

void DeleteAllRibbons();

Ribbon* CreateRibbon(const char* proto_name, Vector2 coord, float z, float speed_factor, float x, float y, float w, float h);
Ribbon* CreateRibbon(const char* proto_name, Vector2 coord, float speed_factor, float x, float y, float w, float h);
//Ribbon* CreateRibbon(Proto* proto, Vector2 coord, bool overr_z, float z, float speed_factor, float x1, float y1, float x2, float y2);

#endif // __RIBBON_H_