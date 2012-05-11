#include "stdafx.h"
#include "iichan_eventalloc.h"
#include "iichan_character.h"
#include "../misc.h"
#include "../sound/snd.h"

extern SND* snd;
extern unsigned int current_time;

IICHANEventAllocator::IICHANEventAllocator()
{
	events.resize(0);
	_count = 0;
	_tick = 0;
	_time = 0;
	SetItemAllocator(NULL);
	SetScreenTextAllocator(NULL);
	SetInstanceAllocator(NULL);
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
					scr_text->floating = true;
					scr_text->time_remaining = 0;
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
	else if(e->action == EVENT_ACTION_SET_AI)
	{
		if(GetInstanceAllocator())
		{
			GetInstanceAllocator()->SetAI(e->action_param_int != 0);
		}
		else
			WriteToLog("Event error: Instance Allocator does not exist");
	}
	else if(e->action == EVENT_ACTION_SET_PAUSE)
	{
		if(GetInstanceAllocator())
		{
			GetInstanceAllocator()->SetPause(e->action_param_int != 0);
		}
		else
			WriteToLog("Event error: Instance Allocator does not exist");
	}
	else if(e->action == EVENT_ACTION_START_DIALOG)
	{
		if(GetInstanceAllocator())
		{
			GetInstanceAllocator()->StartDialog(DialogByName((char*)e->action_param_str.c_str()));
		}
		else
			WriteToLog("Event error: Instance Allocator does not exist");
	}
	else if(e->action == EVENT_ACTION_DISABLE_KEYBOARD)
	{
		assert(GetInstanceAllocator() != NULL);
		GetInstanceAllocator()->SetKeyboardEnabled(!e->action_param_int != 0);
	}
	else if(e->action == EVENT_ACTION_SOUND_PLAY)
	{
		if(GetItemAllocator())
		{
			if(snd)
				snd->PlaySnd(e->action_param_str, e->action_param_int != 0);
		}
		else
			WriteToLog("Event error: Item Allocator does not exist");
	}
	else if(e->action == EVENT_ACTION_HIDE_HUD)
	{
		if(GetInstanceAllocator())
		{
			GetInstanceAllocator()->SetHUDEnabled(!e->action_param_int != 0);
		}
		else
			WriteToLog("Event error: Instance Allocator does not exist");
	}
	else if(e->action == EVENT_ACTION_DISABLE_ID_AI)
	{
		if(GetItemAllocator())
		{
			int id = e->action_param_int;
			CHARACTER* enemy = (CHARACTER*)GetItemAllocator()->GetItem(id);
			if(enemy)
			{
				enemy->SetAI(aiOff);
			}
		}
	}
	else if(e->action == EVENT_ACTION_ENABLE_ID_AI)
	{
		if(GetItemAllocator())
		{
			int id = e->action_param_int;
			CHARACTER* enemy = (CHARACTER*)GetItemAllocator()->GetItem(id);
			if(enemy)
			{
				enemy->SetAI(aiOn);
			}
		}
	}
}

void IICHANEventAllocator::Process()
{
	if(current_time - _tick >= 1000)
	{
		_time++;
		_tick = current_time;
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
		{
			EVENT* e = events[i];
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
				else if(e->condition == EVENT_CONDITION_HERO_X_GR)
				{
					if(GetItemAllocator())
					{
						if(GetItemAllocator()->GetFirstPlayer())
						{
							if(GetItemAllocator()->GetFirstPlayer()->GetSprite())
							{
								if(GetItemAllocator()->GetFirstPlayer()->
									GetSprite()->GetX() > e->condition_param)
								{
									ExecuteEvent(e);
									e->solved = true;
								}
							}
							else
								WriteToLog("Event error: Player sprite does not exist");
						}
						else
							WriteToLog("Event error: Player does not exist");
					}
					else
						WriteToLog("Event error: Item Allocator does not exist");
				}							
			}
			i++;
		}
	}
}

