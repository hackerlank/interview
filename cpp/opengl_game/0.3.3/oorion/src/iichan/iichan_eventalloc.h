#ifndef __IICHAN_EVENTALLOC_H_
#define __IICHAN_EVENTALLOC_H_

#include "iichan_itemalloc.h"
#include "iichan_scrtextalloc.h"

struct EVENT {
	int id;
	
	int condition;
	int action;

	int condition_param;
	int action_param_int;
	string action_param_str;

	bool solved;

	EVENT()
	{
		solved = false;
	}

};

const int EVENT_CONDITION_TIME_ELAPSED = 1;
const int EVENT_CONDITION_HERO_X_GR = 2; // hero x is greater than ...

const int EVENT_ACTION_HERO_SAY = 25;
const int EVENT_ACTION_START_DIALOG = 26;

class IICHANEventAllocator
{
private:
	int _tick;
	int _time;

protected:
	vector<EVENT*> events;
	int _count; // current count of events
	IICHANItemAllocator* ItemAllocator;
	IICHANScreenTextAllocator* ScreenTextAllocator;

public:
	void AddEvent(EVENT* e);
	void DeleteEvent(EVENT* e);
	int GetIndexById(int id);
	void ExecuteEvent(EVENT* e);

	void SetItemAllocator(IICHANItemAllocator* new_value)
	{ ItemAllocator = new_value; }
	IICHANItemAllocator* GetItemAllocator() const
	{ return ItemAllocator; }

	void SetScreenTextAllocator(IICHANScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	IICHANScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	void Process();
	IICHANEventAllocator();
	~IICHANEventAllocator();
};

#endif