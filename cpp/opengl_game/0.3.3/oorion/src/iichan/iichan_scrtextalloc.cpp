#include "stdafx.h"
#include "iichan_scrtextalloc.h"
#include "../misc.h"
#include "../draw/draw.h"

IICHANScreenTextAllocator::IICHANScreenTextAllocator()
{
	items.resize(0);
	_count = 0;
	_tick = 0;
}

IICHANScreenTextAllocator::~IICHANScreenTextAllocator()
{
	for(vector<SCREEN_TEXT*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		SCREEN_TEXT* text = *it;
		if(text)
		{
			try
			{
				delete text;
			}
			catch(...)
			{
				WriteToLog("Exception in IICHANScreenTextAllocator::~IICHANScreenTextAllocator()");
			}
		}
	}
	items.clear();
}

int IICHANScreenTextAllocator::GetIndexById(int id)
{
	UINT index = 0;
	for(vector<SCREEN_TEXT*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		SCREEN_TEXT* text = *it;
		if(text)
		{
			if(text->id == id)
				return index;
		}
		index++;
	}
	return -1;
}

void IICHANScreenTextAllocator::AddItem(SCREEN_TEXT* scr_text)
{
	items.push_back(scr_text);
	scr_text->id = _count;
	_count++;
}

void IICHANScreenTextAllocator::DeleteItem(SCREEN_TEXT* scr_text)
{
	if(!scr_text)
		return;

	int id = scr_text->id;
	id = GetIndexById(id);
	delete scr_text;
	scr_text = NULL;

	items.erase(items.begin() + id);
}

void IICHANScreenTextAllocator::Process()
{
	int now = GetTickCount();
	if(now - _tick >= 1000)
	{
		for(vector<SCREEN_TEXT*>::iterator it = items.begin();
			it != items.end();
			it++)
		{
			SCREEN_TEXT* text = *it;
			if(text)
			{
				text->time_remaining--;
			}
		}

		int i = 0;
		while(i < _count)
		{
			if(items[i]->time_remaining <= 0)
			{
				delete items[i];
				items.erase(items.begin() + i);
				_count--;
				continue;
			}
			else
				i++;
		}
		_tick = now;
	}
}

void IICHANScreenTextAllocator::Draw()
{
	static const font_t* font = FontByName(DEBUG_FONT);
	for(vector<SCREEN_TEXT*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		SCREEN_TEXT* text = *it;
		if(text)
		{
			PrintText(text->text.c_str(), font, text->x, text->y);
		}
	}
}

