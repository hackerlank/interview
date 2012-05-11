#include "stdafx.h"
#include <algorithm>
#include "iichan_spritealloc.h"
#include "../misc.h"

bool UDLower(SPRITE* spr1, SPRITE* spr2)
{
	return spr1->GetZ() < spr2->GetZ();
}

bool MAPLower(int i1, int i2)
{
	return i1 < i2;
}


void iiSpriteAllocator::AddSprite(SPRITE *spr)
{
	assert(spr);
	spr->SetId(__count);
	__count++;

	sprites.push_back(spr);

	// ��������� Z-ORDER map
	int z = spr->GetZ();

	ordermap[z]++;

	// ���������� ����� �� ����������� ������ �� Z, � ����
	// �����, �� ���������� ������� ����������
	SPRITE* last_sprite = NULL;
	if(sprites.size() > 1)
		last_sprite = *(sprites.end()-2);

	// ���� ���� �� ���� ������ ��� ����
	if(last_sprite)
	{
		// ��������� ������� ����������
		vector<SPRITE*>::iterator begin;
	
		// ��������� ���������� ��� ���
		bool to_sort = false;

		// ���� ����� ������ ����� Z >= ����������� �������,
		// �� ����������� �� �����
		// (������������� ��� ������ ������ ������������)
		if(z >= last_sprite->GetZ())
		{
			to_sort = false;
		}
		else
		{
			// ���� ����� ������ ����� ������� Z, ����� ����������
			to_sort = true;

			// ��������� ���������� �������� �� ����� Z �� ������ Z
			// ���������� ��� �������� ���������� ��������� ����������
			UINT all_z_count = 0;

			order_map::iterator it = ordermap.find(z);
			order_map::iterator end = ordermap.end();
			end--;

			bool found = false;

			if(it == end)
			{
				found = ((*it).first == z);
			}
			else
				found = true;

			if(it != end)
				it++;

			order_map::iterator first = ordermap.begin();

			if(found)
				first = it;

			for(order_map::iterator it2 = ordermap.begin();
				it2 != it;
				it2++)
			{
				all_z_count += (*it2).second;
			}

			begin = sprites.begin() + all_z_count;

		}
		// ���������
		if(to_sort)
			sort(begin, sprites.end(), UDLower);
	}
}

void iiSpriteAllocator::DeleteSprite(SPRITE* spr)
{
	assert(spr);
	
	int index = GetIndexById(spr->GetId());
	sprites.erase(sprites.begin() + index);
	ordermap[spr->GetZ()]--;

	delete spr;
	spr = NULL;
}

SPRITE* iiSpriteAllocator::GetSprite(int id)
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

int iiSpriteAllocator::GetIndexById(int id)
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



void iiSpriteAllocator::Draw()
{

	for(UINT i = 0; i < sprites.size(); i++)
	{
		SPRITE* spr = sprites[i];

		if(spr->IsDead())
			DeleteSprite(spr);
		else
			spr->Draw();
	}
}

void iiSpriteAllocator::Move()
{
	for(UINT i = 0; i < sprites.size(); i++)
	{
		SPRITE* spr = sprites[i];
		spr->Move();
	}

	// � background �� ������� �� �����
}

iiSpriteAllocator::iiSpriteAllocator()
{
	sprites.resize(0);
	__count = 0;

	vertices = NULL;
	colors = NULL;
	coords = NULL;

}

iiSpriteAllocator::~iiSpriteAllocator()
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
				WriteToLog("Exception in iiSpriteAllocator::~iiSpriteAllocator()");
			}
		}
	}
	sprites.clear();
	ordermap.clear();

	if(vertices)
		delete[] vertices;
	if(colors)
		delete[] colors;
	if(coords)
		delete[] coords;

	vertices = NULL;
	colors = NULL;
	coords = NULL;

}

void iiSpriteAllocator::HideAll()
{
	for(vector<SPRITE*>::iterator it = sprites.begin();
		it != sprites.end();
		it++)
		(*it)->SetVisible(false);
}

void iiSpriteAllocator::ShowAll()
{
	for(vector<SPRITE*>::iterator it = sprites.begin();
		it != sprites.end();
		it++)
		(*it)->SetVisible(true);
}