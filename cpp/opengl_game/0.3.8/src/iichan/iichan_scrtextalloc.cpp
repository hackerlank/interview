#include "stdafx.h"
#include "iichan_scrtextalloc.h"
#include "../misc.h"
#include "../draw/draw.h"

extern unsigned int current_time;

iiScreenTextAllocator::iiScreenTextAllocator()
{
	items.resize(0);
	_count = 0;
}

iiScreenTextAllocator::~iiScreenTextAllocator()
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
				WriteToLog("Exception in iiScreenTextAllocator::~iiScreenTextAllocator()");
			}
		}
	}
	items.clear();
}

int iiScreenTextAllocator::GetIndexById(int id)
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

SCREEN_TEXT* iiScreenTextAllocator::GetItemByText(string str)
{
	for(vector<SCREEN_TEXT*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		SCREEN_TEXT* text = *it;
		if(text)
		{
			if(text->text.find(str) != text->text.npos)
			{
				return text;
			}
		}
	}
	return NULL;
}

void iiScreenTextAllocator::AddItem(SCREEN_TEXT* scr_text)
{
	items.push_back(scr_text);
	scr_text->id = _count;
	scr_text->_prev = current_time;
	_count++;
}

void iiScreenTextAllocator::DeleteItem(SCREEN_TEXT* scr_text)
{
	if(!scr_text)
		return;

	int id = scr_text->id;
	id = GetIndexById(id);
	delete scr_text;
	scr_text = NULL;

	items.erase(items.begin() + id);
}

void iiScreenTextAllocator::Process()
{
	int i = 0;
	while(i < _count)
	{
		// TODO: fix magic numbers
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

void iiScreenTextAllocator::Draw()
{
	static const Font* font = FontByName(DEBUG_FONT);
	for(vector<SCREEN_TEXT*>::iterator it = items.begin();
		it != items.end();
		it++)
	{
		SCREEN_TEXT* text = *it;
		glPushAttrib(GL_CURRENT_BIT);
		glColor3ub(255 - text->_d_red, 255, 255 - text->_d_blue);
		PrintText(text->text.c_str(), font, text->x, text->y);
		glPopAttrib();
	}
}