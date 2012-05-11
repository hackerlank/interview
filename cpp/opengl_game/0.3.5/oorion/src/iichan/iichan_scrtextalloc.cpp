#include "stdafx.h"
#include "iichan_scrtextalloc.h"
#include "../misc.h"
#include "../draw/draw.h"

extern unsigned int current_time;

IICHANScreenTextAllocator::IICHANScreenTextAllocator()
{
	items.resize(0);
	_count = 0;
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
	scr_text->_prev = current_time;
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
	int i = 0;
	while(i < _count)
	{
		if(current_time - items[i]->_prev_float >= 20)
		{
			if(items[i]->floating)
			{
				items[i]->y -= 2;
				if(items[i]->_d_red + 8 <= 192)
				{
					items[i]->_d_red += 8;
					items[i]->_d_green += 8;
					items[i]->_d_blue += 8;
				}
				items[i]->_prev_float = current_time;
			}
		}
		if(current_time - items[i]->_prev >= 1000)
		{
			if(items[i]->time_remaining <= 0)
			{
				delete items[i];
				items.erase(items.begin() + i);
				_count--;
				continue;
			}
			else
			{
				items[i]->_prev = current_time;
				items[i]->time_remaining--;
			}
		}
		i++;
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
		glColor3ub(255 - text->_d_red, 255 - text->_d_green, 255 - text->_d_blue);
		PrintText(text->text.c_str(), font, text->x, text->y);
	}
}