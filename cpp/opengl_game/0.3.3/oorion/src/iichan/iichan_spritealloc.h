#ifndef __IICHAN_SPRITEALLOC_H_
#define __IICHAN_SPRITEALLOC_H_

#include "iichan_sprite.h"

class IICHANSpriteAllocator
{
protected:
	vector<SPRITE*> sprites;
	int _count; // current count of sprites

public:
	void AddSprite(SPRITE* spr);
	SPRITE* GetSprite(int id);
	int GetIndexById(int id);

	void DeleteSprite(SPRITE* spr);

	void Draw();
	void Move();

	IICHANSpriteAllocator();
	~IICHANSpriteAllocator();
};

#endif