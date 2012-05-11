#ifndef __ii_EVENTALLOC_H_
#define __ii_EVENTALLOC_H_

#include "iichan_itemalloc.h"
#include "iichan_scrtextalloc.h"
#include "iichan_instancealloc.h"

struct EVENT {
	int id;
	
	int condition;
	int action;

	int condition_param1;
	int condition_param2;
	int action_param_int;
	string action_param_str;

	bool solved;
	bool periodic;
	int call_period;
	int call_prev;
	int call_max;
	int call_count;

	int GetId() { return id; }

	EVENT()
	{
		periodic = false;
		solved = false;
		call_period = 0;
		call_prev = 0;
		call_max = 0;
		call_count = 0;
		id = 0;
		condition = 0;
		action = 0;
		condition_param1 = 0;
		condition_param2 = 0;
		action_param_int = 0;
		action_param_str = "";

		
	}

};

const int EVENT_CONDITION_TIME_ELAPSED = 1;
const int EVENT_CONDITION_HERO_X_GR = 2; // hero x is greater than ...
const int EVENT_CONDITION_ID_X_GR = 3;
const int EVENT_CONDITION_ID_X_EQ = 4;
const int EVENT_CONDITION_GAME_STARTED = 5;
const int EVENT_CONDITION_PERIOD = 6;

const int EVENT_ACTION_HERO_SAY = 25;
const int EVENT_ACTION_START_DIALOG = 26;
const int EVENT_ACTION_SET_AI = 27;
const int EVENT_ACTION_SET_PAUSE = 28;
const int EVENT_ACTION_DISABLE_KEYBOARD = 29;
const int EVENT_ACTION_HIDE_HUD = 30;
const int EVENT_ACTION_DISABLE_ID_AI = 31;
const int EVENT_ACTION_ENABLE_ID_AI = 32;
const int EVENT_ACTION_MOVE_ID = 33;
const int EVENT_ACTION_STOP_ID = 34;
const int EVENT_ACTION_SOUND_PLAY = 50;
const int EVENT_ACTION_SOUND_STOP = 51;
const int EVENT_ACTION_MUSIC_PLAY = 52;
const int EVENT_ACTION_MUSIC_STOP = 53;
const int EVENT_ACTION_ALL_SOUNDS_STOP = 54;
const int EVENT_ACTION_EXEC_OSL_STRING = 60;
const int EVENT_ACTION_START_QUAKE = 70;
const int EVENT_ACTION_STOP_QUAKE = 71;


#define EVENT_UPDATE_TICK 100

class iiEventAllocator;

typedef bool (iiEventAllocator::*condition_function)(EVENT* e);
typedef map<int, condition_function> condition_map;

typedef void (iiEventAllocator::*action_function)(EVENT* e);
typedef map<int, action_function> action_map;

class iiEventAllocator
{
private:
	int _its_time;

	bool condTimeElapsed(EVENT* e);
	bool condGameStarted(EVENT* e);
	bool condHeroXGr(EVENT* e);
	bool condIdXGr(EVENT* e);
	bool condIdXEq(EVENT* e);
	bool condPeriod(EVENT* e);

	void actHeroSay(EVENT* e);
	void actSetAI(EVENT* e);
	void actSetPause(EVENT* e);
	void actStartDialog(EVENT* e);
	void actDisableKeyboard(EVENT* e);
	void actHideHUD(EVENT* e);
	void actDisableIdAI(EVENT* e);
	void actEnableIdAI(EVENT* e);
	void actMoveId(EVENT* e);
	void actStopId(EVENT* e);
	void actExecOSLString(EVENT* e);
	void actStartQuake(EVENT* e);
	void actStopQuake(EVENT* e);
	void actSoundPlay(EVENT* e);
	void actSoundStop(EVENT* e);
	void actBackMusicPlay(EVENT* e);
	void actBackMusicStop(EVENT* e);
	void actAllSoundsStop(EVENT* e);

protected:
	vector<EVENT*> events;
	unsigned int _count; // Текущее количество событий
	unsigned int __count; // Общее количество добавленных событий
	iiItemAllocator* ItemAllocator;
	iiScreenTextAllocator* ScreenTextAllocator;
	iiInstanceAllocator* InstanceAllocator;

	condition_map conds;
	action_map actions;

public:
	void AddEvent(EVENT* e);
	void DeleteEvent(EVENT* e);

	int GetCount() const { return events.size(); }

	void SetItemAllocator(iiItemAllocator* new_value)
	{ ItemAllocator = new_value; }
	iiItemAllocator* GetItemAllocator() const
	{ return ItemAllocator; }

	void SetScreenTextAllocator(iiScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	iiScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	void SetInstanceAllocator(iiInstanceAllocator* new_value)
	{ InstanceAllocator = new_value; }
	iiInstanceAllocator* GetInstanceAllocator() const
	{ return InstanceAllocator; }

	void MapCondition(int cond, condition_function func);
	void MapAction(int action, action_function func);

	void Process();

	// Для упрощения отладки добавляем возможность вручную
	// запустить следующее запланированное событие
	void ExecuteNextEvent();

	void InitMappings();
	void FreeMappings();

	iiEventAllocator();
	~iiEventAllocator();
};

#endif