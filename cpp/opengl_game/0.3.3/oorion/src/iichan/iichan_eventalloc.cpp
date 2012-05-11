#include "stdafx.h"
#include "iichan_eventalloc.h"
#include "../misc.h"

IICHANEventAllocator::IICHANEventAllocator()
{
	events.resize(0);
	_count = 0;
	_tick = 0;
	_time = 0;
	SetItemAllocator(NULL);
	SetScreenTextAllocator(NULL);
}

IICHANEventAllocator::~IICHANEventAllocator()
{
	for(vector<EVENT*>::iterator it = events.begin();
		it != events.end();
		it++)
	{
		EVENT* e = *it;
		if(e)
		{
			try
			{
				delete e;
			}
			catch(...)
			{
				WriteToLog("Exception in IICHANEventAllocator::~IICHANEventAllocator()");
			}
		}
	}
	events.clear();
}

int IICHANEventAllocator::GetIndexById(int id)
{
	UINT index = 0;
	for(vector<EVENT*>::iterator it = events.begin();
		it != events.end();
		it++)
	{
		EVENT* e = *it;
		if(e)
		{
			if(e->id == id)
				return index;
		}
		index++;
	}
	return -1;
}

void IICHANEventAllocator::AddEvent(EVENT* e)
{
	events.push_back(e);
	e->id = _count;
	_count++;
}

void IICHANEventAllocator::DeleteEvent(EVENT* e)
{
	if(!e)
		return;

	int id = e->id;
	id = GetIndexById(id);
	delete e;
	e = NULL;

	events.erase(events.begin() + id);
}

void IICHANEventAllocator::ExecuteEvent(EVENT* e)
{
	if(e->action == EVENT_ACTION_HERO_SAY)
	{
		if(GetItemAllocator())
		{
			if(GetItemAllocator()->GetFirstPlayer())
			{
				if(GetItemAllocator()->GetFirstPlayer()->GetSprite())
				{
					SCREEN_TEXT* scr_text = new SCREEN_TEXT();
					scr_text->x = GetItemAllocator()->GetFirstPlayer()->GetSprite()->GetX();
					scr_text->y = GetItemAllocator()->GetFirstPlayer()->GetSprite()->GetY();
					scr_text->text = e->action_param_str;
					GetScreenTextAllocator()->AddItem(scr_text);
				}
				else
					WriteToLog("Event error: player sprite does not exist");
			}
			else
				WriteToLog("Event error: player does not exist");
		}
		else
			WriteToLog("Event error: Item Allocator does not exist");
	}		
}

void IICHANEventAllocator::Process()
{
	int now = GetTickCount();
	if(now - _tick >= 1000)
	{
		_time++;
		_tick = now;
	}

	for(vector<EVENT*>::iterator it = events.begin();
		it != events.end();
		it++)
	{
		EVENT* e = *it;
		if(e)
		{
			if(e->condition == EVENT_CONDITION_TIME_ELAPSED)
			{
				if(e->condition_param == _time)
				{
					ExecuteEvent(e);
					e->solved = true;
				}
			}
		}
	}

	int i = 0;
	while(i < _count)
	{
		if(events[i]->solved)
		{
			delete events[i];
			events.erase(events.begin() + i);
			_count--;
			continue;
		}
		else
			i++;
	}

}

