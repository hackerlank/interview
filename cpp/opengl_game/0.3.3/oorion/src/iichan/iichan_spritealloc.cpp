#include "stdafx.h"
#include <algorithm>
#include "iichan_spritealloc.h"
#include "../misc.h"

bool UDLower(SPRITE* spr1, SPRITE* spr2)
{
	return spr1->GetZ() < spr2->GetZ();
}

void IICHANSpriteAllocator::AddSprite(SPRITE *spr)
{
	spr->SetId(_count);
	sprites.push_back(spr);

	// sort by Z-axis
	sort(sprites.begin(), sprites.end(), UDLower);
	_count++;
}

void IICHANSpriteAllocator::DeleteSprite(SPRITE* spr)
{
	if(!spr)
		return;

	int id = spr->GetId();
	id = GetIndexById(id);
	delete spr;
	spr = NULL;

	sprites.erase(sprites.begin() + id);
}

SPRITE* IICHANSpriteAllocator::GetSprite(int id)
{
	for(vector<SPRITE*>::iterator it = sprites.begin();
		it != sprites.end();
		it++)
	{
		SPRITE* spr = *it;
		if(spr)
		{
			if(spr->GetId() == id)
				return spr;
		}
	}
	return NULL;
}

int IICHANSpriteAllocator::GetIndexById(int id)
{
	UINT index = 0;
	for(vector<SPRITE*>::iterator it = sprites.begin();
		it != sprites.end();
		it++)
	{
		SPRITE* spr = *it;
		if(spr)
		{
			if(spr->GetId() == id)
				return index;
		}
		index++;
	}
	return -1;
}

void IICHANSpriteAllocator::Draw()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	for(vector<SPRITE*>::iterator it = sprites.begin();
		it != sprites.end();
		it++)
	{
		SPRITE* spr = *it;
		spr->Draw();
	}
}

void IICHANSpriteAllocator::Move()
{
	for(UINT i = 0; i < sprites.size(); i++)
	{
		SPRITE* spr = sprites[i];

		if(spr->GetDead())
			DeleteSprite(spr);
		else
			spr->Move();
	}
}

IICHANSpriteAllocator::IICHANSpriteAllocator()
{
	sprites.resize(0);
	_count = 0;
}

IICHANSpriteAllocator::~IICHANSpriteAllocator()
{
	for(vector<SPRITE*>::iterator it = sprites.begin();
		it != sprites.end();
		it++)
	{
		SPRITE* spr = *it;
		if(spr)
		{
			try
			{
				delete spr;
			}
			catch(...)
			{
				WriteToLog("Exception in IICHANSpriteAllocator::~IICHANSpriteAllocator()");
			}
		}
	}
	sprites.clear();
}