#ifndef __ii_SPRITEALLOC_H_
#define __ii_SPRITEALLOC_H_

#include "iichan_sprite.h"
#include "../draw/vertex.h"

typedef map<int, UINT> order_map;

class iiSpriteAllocator
{
private:
	// сюда записывается, сколько спрайтов с каждым уровнем глубины
	// используется впоследствии для быстрой сортировки
	order_map ordermap;
	unsigned int __count;

	Vertex* vertices;
	Color* colors;
	Coord* coords;

protected:
	vector<SPRITE*> sprites;

public:
	void AddSprite(SPRITE* spr);
	SPRITE* GetSprite(int id);
	int GetIndexById(int id);

	unsigned int GetCount()
	{
		return sprites.size();
	}

	void DeleteSprite(SPRITE* spr);

	void Draw();
	void Move();

	void HideAll();
	void ShowAll();

	iiSpriteAllocator();
	~iiSpriteAllocator();
};

#endif