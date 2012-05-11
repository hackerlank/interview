#include "stdafx.h"
#include "iichan_eventalloc.h"
#include "iichan_character.h"
#include "../misc.h"
#include "../sound/snd.h"
#include "../osl/osl.h"
#include "../osl/osl_misc.h"

extern SND* snd;
extern unsigned int current_time;
extern unsigned int game_time;
extern int WORLD_X;
extern int WORLD_Y;

#ifdef _DEBUG
string dbgEventSystemTime;
#endif


#define EVENT_SYSTEM_TIME game_time - _its_time

iiEventAllocator::iiEventAllocator()
{
	events.resize(0);
	_count = 0;
	__count = 0;
	SetItemAllocator(NULL);
	SetScreenTextAllocator(NULL);
	SetInstanceAllocator(NULL);
	_its_time = game_time;
}

iiEventAllocator::~iiEventAllocator()
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
				WriteToLog("Exception in iiEventAllocator::~iiEventAllocator()");
			}
		}
	}
	events.clear();
	FreeMappings();
}

void iiEventAllocator::AddEvent(EVENT* e)
{
	__count++;
	assert(e);
	events.push_back(e);
	e->id = __count;
#ifdef _DEBUG
	LogToFile(string("Added event ") + IntToString(e->id));
#endif
	if(e->condition == EVENT_CONDITION_PERIOD)
	{
		e->periodic = true;
		e->call_prev = EVENT_SYSTEM_TIME;	
		e->call_period = e->condition_param1 * 1000;
		e->call_max = e->condition_param2;
	}
	else if(e->condition == EVENT_CONDITION_TIME_ELAPSED)
	{
		e->condition_param1 *= 1000;
	}
	_count++;
}

void iiEventAllocator::DeleteEvent(EVENT* e)
{
	assert(e);

	int id = e->id;
	id = GetIndexById(&events, id);
	delete e;
	e = NULL;

	_count--;

	events.erase(events.begin() + id);
}

void iiEventAllocator::MapCondition(int cond, condition_function func)
{
	conds[cond] = func;
}

void iiEventAllocator::MapAction(int action, action_function func)
{
	actions[action] = func;
}

void iiEventAllocator::Process()
{
	vector<EVENT*>::size_type count = events.size();		// Текущий размер вектора events
	int curr_id;											// EVENT::id текущего элемента вектора

#ifdef _DEBUG
	dbgEventSystemTime = "Events time: " + IntToString((EVENT_SYSTEM_TIME) / 1000);
#endif


	for(vector<EVENT*>::iterator it = events.begin();
		it != events.end();
		it++)
	{
		EVENT* e = *it;
		assert(e != NULL);
		curr_id = e->id;

		if((this->*conds[e->condition])(e))
		{
			(this->*actions[e->action])(e);
		#ifdef _DEBUG
			//LogToFile(IntToString(e->id) + " - executed");
		#endif
			if(e->periodic)
			{
				e->call_count++;
				e->call_prev = EVENT_SYSTEM_TIME;

				if(e->call_count == e->call_max)
				{
					e->solved = true;
				#ifdef _DEBUG
					//LogToFile(string("Solved periodic event ") + IntToString(e->id));
				#endif
				}
			}
			else
			{
				e->solved = true;
			#ifdef _DEBUG
				//LogToFile(string("Solved nonperiodic event ") + IntToString(e->id));
			#endif
			}
		}

		

		// Тут мы находим новое правильное положене итератора в случае, если 
		// изменился размер вектора events.
		if (count != events.size())
		{
			
		#ifdef _DEBUG
			//LogToFile(string("Iterator lost. Finding..."));
		#endif
			count = events.size();
			vector<EVENT*>::iterator iter;

			for(iter = events.begin();
				iter != events.end();
				iter++)
			{
				EVENT* e_temp = *iter;
				if (e_temp->id == curr_id)
					break;
			}
			it = iter;		
		}

	}

	// Помяенял направление обхода вектора, но зато теперь ни один из эвентов не пропускается.
	// Кажется, довольно изящно ^_^
	for(INT i = _count - 1; i >= 0 ; i--)
	{
		if(events[i]->solved)
		{
		#ifdef _DEBUG
			//LogToFile(string("Deleting event: ") + IntToString(events[i]->id));
		#endif
			DeleteEvent(events[i]);
		}		
	}

}

void iiEventAllocator::ExecuteNextEvent()
{
}


void iiEventAllocator::InitMappings()
{

	MapCondition(EVENT_CONDITION_TIME_ELAPSED, &iiEventAllocator::condTimeElapsed);
	MapCondition(EVENT_CONDITION_GAME_STARTED, &iiEventAllocator::condGameStarted);
	MapCondition(EVENT_CONDITION_HERO_X_GR, &iiEventAllocator::condHeroXGr);
	MapCondition(EVENT_CONDITION_ID_X_GR, &iiEventAllocator::condIdXGr);
	MapCondition(EVENT_CONDITION_ID_X_EQ, &iiEventAllocator::condIdXEq);
	MapCondition(EVENT_CONDITION_PERIOD, &iiEventAllocator::condPeriod);

	MapAction(EVENT_ACTION_HERO_SAY, &iiEventAllocator::actHeroSay);
	MapAction(EVENT_ACTION_SET_AI, &iiEventAllocator::actSetAI);
	MapAction(EVENT_ACTION_SET_PAUSE, &iiEventAllocator::actSetPause);
	MapAction(EVENT_ACTION_DISABLE_KEYBOARD, &iiEventAllocator::actDisableKeyboard);
	MapAction(EVENT_ACTION_HIDE_HUD, &iiEventAllocator::actHideHUD);
	MapAction(EVENT_ACTION_DISABLE_ID_AI, &iiEventAllocator::actDisableIdAI);
	MapAction(EVENT_ACTION_ENABLE_ID_AI, &iiEventAllocator::actEnableIdAI);
	MapAction(EVENT_ACTION_MOVE_ID, &iiEventAllocator::actMoveId);
	MapAction(EVENT_ACTION_STOP_ID, &iiEventAllocator::actStopId);
	MapAction(EVENT_ACTION_SOUND_PLAY, &iiEventAllocator::actSoundPlay);
	MapAction(EVENT_ACTION_SOUND_STOP, &iiEventAllocator::actSoundStop);
	MapAction(EVENT_ACTION_ALL_SOUNDS_STOP, &iiEventAllocator::actAllSoundsStop);
	MapAction(EVENT_ACTION_MUSIC_PLAY, &iiEventAllocator::actBackMusicPlay);
	MapAction(EVENT_ACTION_MUSIC_STOP, &iiEventAllocator::actBackMusicStop);
	MapAction(EVENT_ACTION_EXEC_OSL_STRING, &iiEventAllocator::actExecOSLString);
	MapAction(EVENT_ACTION_START_QUAKE, &iiEventAllocator::actStartQuake);
	MapAction(EVENT_ACTION_STOP_QUAKE, &iiEventAllocator::actStopQuake);
	MapAction(EVENT_ACTION_START_DIALOG, &iiEventAllocator::actStartDialog);
}

void iiEventAllocator::FreeMappings()
{
	conds.clear();
	actions.clear();
}



// Функции проверяющие условия
// -------------------------------------------------------------------------

bool iiEventAllocator::condTimeElapsed(EVENT* e)
{
#pragma push
#pragma warning(disable : 4018)
	return(e->condition_param1 >= EVENT_SYSTEM_TIME);
#pragma pop
}

bool iiEventAllocator::condGameStarted(EVENT* e)
{
	return(EVENT_SYSTEM_TIME >= 0);
}

bool iiEventAllocator::condHeroXGr(EVENT* e)
{
	assert(GetItemAllocator());
	assert(GetItemAllocator()->GetFirstPlayer());
	assert(GetItemAllocator()->GetFirstPlayer()->GetSprite());

	int x = GetItemAllocator()->GetFirstPlayer()->GetSprite()->GetX();
	return(x > e->condition_param1);
}

bool iiEventAllocator::condIdXGr(EVENT* e)
{
	assert(GetItemAllocator());
	int id = e->action_param_int;
	CHARACTER* ch = (CHARACTER*)GetItemAllocator()->GetItem(id);
	if(ch)
	{
		ANIMATEDSPRITE* sprite = ch->GetSprite();
		assert(sprite);
		return(sprite->GetX() > e->condition_param1);
	}
	else
	{
		WriteToLog("Event error: Character sprite does not exist");
		return false;
	}
}

bool iiEventAllocator::condIdXEq(EVENT* e)
{
	assert(GetItemAllocator());
	int id = e->action_param_int;
	CHARACTER* ch = (CHARACTER*)GetItemAllocator()->GetItem(id);
	if(ch)
	{
		ANIMATEDSPRITE* sprite = ch->GetSprite();
		assert(sprite);
		return(sprite->GetX() == e->condition_param1);
	}
	else
	{
		WriteToLog("Event error: Character sprite does not exist");
		return false;
	}
}

bool iiEventAllocator::condPeriod(EVENT* e)
{
#pragma push
#pragma warning(disable : 4018)
	return(EVENT_SYSTEM_TIME - e->call_prev >= e->call_period);
#pragma pop
}
// -------------------------------------------------------------------------



// Функции выполняющие действия
// -------------------------------------------------------------------------

void iiEventAllocator::actHeroSay(EVENT* e)
{
	assert(GetItemAllocator());
	assert(GetItemAllocator()->GetFirstPlayer());
	assert(GetItemAllocator()->GetFirstPlayer()->GetSprite());
	assert(GetScreenTextAllocator());


	SCREEN_TEXT* scr_text = new SCREEN_TEXT();
	scr_text->x = GetItemAllocator()->GetFirstPlayer()->GetSprite()->GetX() - WORLD_X;
	scr_text->y = GetItemAllocator()->GetFirstPlayer()->GetSprite()->GetY() - WORLD_Y;
	scr_text->text = e->action_param_str;
	scr_text->floating = true;
	scr_text->time_remaining = e->action_param_int;

	GetScreenTextAllocator()->AddItem(scr_text);
}

void iiEventAllocator::actSetAI(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->SetAI(e->action_param_int != 0);
}

void iiEventAllocator::actSetPause(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->SetPause(e->action_param_int != 0);
}

void iiEventAllocator::actStartDialog(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->StartDialog(DialogByName(e->action_param_str.c_str()));
}

void iiEventAllocator::actDisableKeyboard(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->SetKeyboardEnabled(!e->action_param_int != 0);
}

void iiEventAllocator::actSoundPlay(EVENT* e)
{
	if(!snd)
		return;
	string sound_name = osl_RemoveQuotesStr(e->action_param_str);
	snd->PlaySnd(sound_name, e->action_param_int != 0);
}

void iiEventAllocator::actSoundStop(EVENT* e)
{
	if(!snd)
		return;
	string sound_name = osl_RemoveQuotesStr(e->action_param_str);
	snd->StopSnd(sound_name);
}

void iiEventAllocator::actAllSoundsStop(EVENT* e)
{
	if(!snd)
		return;
	snd->StopAll();
}

void iiEventAllocator::actBackMusicPlay(EVENT* e)
{
	if(!snd)
		return;
	string sound_name = osl_RemoveQuotesStr(e->action_param_str);
	snd->PlayBackMusic(sound_name);
}

void iiEventAllocator::actBackMusicStop(EVENT* e)
{
	if(!snd)
		return;
	snd->StopBackMusic();
}

void iiEventAllocator::actHideHUD(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->SetHUDEnabled(!e->action_param_int != 0);
}

void iiEventAllocator::actDisableIdAI(EVENT* e)
{
	assert(GetItemAllocator());
	ENEMY* ch = (ENEMY*)GetItemAllocator()->GetItem(e->action_param_int);
	assert(ch);
	ch->SetAIEnabled(false);
}

void iiEventAllocator::actEnableIdAI(EVENT* e)
{
	assert(GetItemAllocator());
	ENEMY* ch = (ENEMY*)GetItemAllocator()->GetItem(e->action_param_int);
	assert(ch);
	ch->SetAIEnabled(true);
}

void iiEventAllocator::actMoveId(EVENT* e)
{
	assert(GetItemAllocator());
	CHARACTER* ch = (CHARACTER*)GetItemAllocator()->GetItem(e->action_param_int);
	assert(ch);
	ANIMATEDSPRITE* sprite = ch->GetSprite();
	assert(sprite);
	sprite->SetWalking(true);
}

void iiEventAllocator::actStopId(EVENT* e)
{
	assert(GetItemAllocator());
	CHARACTER* ch = (CHARACTER*)GetItemAllocator()->GetItem(e->action_param_int);
	assert(ch);
	ANIMATEDSPRITE* sprite = ch->GetSprite();
	assert(sprite);
	sprite->SetWalking(false);
}

void iiEventAllocator::actExecOSLString(EVENT* e)
{
	// TODO: any assert here?
	string osl = osl_RemoveQuotesStr(e->action_param_str);
	osl_Load(osl.c_str(), true);
}

void iiEventAllocator::actStartQuake(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->StartQuake(e->action_param_int);
}

void iiEventAllocator::actStopQuake(EVENT* e)
{
	assert(GetInstanceAllocator());
	GetInstanceAllocator()->StopQuake();
}
// -------------------------------------------------------------------------