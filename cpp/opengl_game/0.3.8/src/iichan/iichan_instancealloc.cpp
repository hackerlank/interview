#include "stdafx.h"
#include "iichan_instancealloc.h"
#include "../config.h"
#include "../gui/gui.h"


extern unsigned int game_time;
extern unsigned int current_state;
extern config cfg;
extern bool keys[256];
extern BOOL GLOBAL_QUAKE_ENABLED;
extern int GLOBAL_QUAKE_POWER;
extern bool weather_on;
extern GUI* gui;

extern BYTE GLOBAL_COLOR_RED;
extern BYTE GLOBAL_COLOR_GREEN;
extern BYTE GLOBAL_COLOR_BLUE;
extern BYTE GLOBAL_COLOR_ALPHA;

#define DEFAULT_QUAKE_POWER 5

#ifdef _DEBUG
string dbgKeyboard = "Keyboard on";
string dbgQuake = "Quake off";
string dbgAI = "AI on";
string dbgHUD = "HUD on";
string dbgPause = "Pause off";
string dbgWeather = "Weather off";
string dbgMenu = "Menu off";
string dbgDialog = "Dialog off";
#endif

void iiInstanceAllocator::SetPause(bool new_value) 
{ 
	pause = new_value;
#ifdef _DEBUG
	pause ? dbgPause = "Pause on" : dbgPause = "Pause off";
#endif
}

bool iiInstanceAllocator::GetPause() const
{ 
	return pause;
}

void iiInstanceAllocator::StartDialog(Dialog* dialog)
{
	assert(dialog);

	dialog->SetSpriteAllocator(GetSpriteAllocator());
	dialog->SetScreenTextAllocator(GetScreenTextAllocator());

	// showing first message
	dialog->ShowMessage(0);
	SetCurrentDialog(dialog);
	_prev_message_time = current_time;

	// disabling keyboard, AI and hiding hud
	SetHUDEnabled(false);
	SetAI(false);
	//SetPause(true);
	SetKeyboardEnabled(false);

	GetCurrentDialog()->SetDone(false);
	
#ifdef _DEBUG
	dbgDialog = "Dialog on";
#endif
}

void iiInstanceAllocator::StartBriefing(Briefing* briefing)
{
	assert(briefing);

	SetCurrentBriefing(briefing);

	//SetPause(true);
	SetKeyboardEnabled(false);
	SetAI(false);
	SetHUDEnabled(false);
	if(GetSpriteAllocator())
		GetSpriteAllocator()->HideAll();
}

void iiInstanceAllocator::StartQuake(int power)
{
	GLOBAL_QUAKE_ENABLED = 1;
	GLOBAL_QUAKE_POWER = power;

#ifdef _DEBUG
	dbgQuake = "Quake on";
#endif
}

void iiInstanceAllocator::StopQuake()
{
	GLOBAL_QUAKE_ENABLED = 0;
	GLOBAL_QUAKE_POWER = 0;

#ifdef _DEBUG
	dbgQuake = "Quake off";
#endif
}

void iiInstanceAllocator::WeatherStart(Weather* weather, int power)
{
	StartWeather(weather, power);
}

void WeatherStop()
{
	StopWeather();
}

//void iiInstanceAllocator::StartSnow(int power)
//{
//	StartWeather(wSnow, power);
//
//#ifdef _DEBUG
//	dbgWeather = "Weather on";
//#endif
//}

//void iiInstanceAllocator::StopSnow()
//{
//	StopWeather();
//
//#ifdef _DEBUG
//	dbgWeather = "Weather off";
//#endif
//}



// Some useful macro's to make more readable code
// --------------------------------------------------------------------------
#define DIALOG_EXIST GetCurrentDialog()
#define DIALOG_NOT_FINISHED !GetCurrentDialog()->GetDone()
#define NOT_LAST_MESSAGE GetCurrentDialog()->GetCurrentMessageId() < \
	GetCurrentDialog()->GetMessageCount() - 1

// TODO: fix current_time to game_time
#define MESSAGE_EXPIRED current_time - _prev_message_time >= GetCurrentDialog()->GetMessageById( \
					GetCurrentDialog()->GetCurrentMessageId())->time * 1000
#define SHOW_NEXT_MESSAGE GetCurrentDialog()->ShowMessage( \
	GetCurrentDialog()->GetCurrentMessageId() + 1)
// ---
#define BRIEFING_EXIST GetCurrentBriefing()
// --------------------------------------------------------------------------

void iiInstanceAllocator::Process()
{
	if(_total_stop)
		return;

	if(DIALOG_EXIST)
	{
		if(DIALOG_NOT_FINISHED)
		{
			if(NOT_LAST_MESSAGE)
			{
				if(MESSAGE_EXPIRED)
				{
					SHOW_NEXT_MESSAGE;
					_prev_message_time = game_time;
				}
			}
			else
			{
				if(MESSAGE_EXPIRED)
				{
					GetCurrentDialog()->ClearCurrentMessage();
					GetCurrentDialog()->SetDone(true);
					SetCurrentDialog(NULL);

					// enabling keyboard, AI and hud
					SetHUDEnabled(true);
					SetAI(true);
					//SetPause(false);
					SetKeyboardEnabled(true);

#ifdef _DEBUG
					dbgDialog = "Dialog off";
#endif
				}
			}
		}
	}
	if(BRIEFING_EXIST)
	{
		if(!GetCurrentBriefing()->done)
			GetCurrentBriefing()->tick += GetCurrentBriefing()->speed;
		else
		{
			delete GetCurrentBriefing();
			SetCurrentBriefing(NULL);

			SetHUDEnabled(true);
			SetAI(true);
			//SetPause(false);
			SetKeyboardEnabled(true);
			if(GetSpriteAllocator())
				GetSpriteAllocator()->ShowAll();
		}
	}

	if(weather_on)
		UpdateWeather(0.1f);
}

void iiInstanceAllocator::Draw()
{
	if(DIALOG_EXIST)
	{
		DarkBox(0, 0, cfg.scr_width, 96); // TODO: fix magic number
	}
	if(BRIEFING_EXIST)
	{
		int i = 0;
		Briefing* br = GetCurrentBriefing();
		for(vector<string>::iterator it = br->lines.begin();
			it != br->lines.end();
			it++)
		{
			i++;
			static const Font* font = FontByName(DEBUG_FONT);
			string line = *it;

			glEnable(GL_SCISSOR_TEST);
			glScissor((int)br->position_x,
				cfg.scr_height - (int)(br->position_y + br->height),
				br->width,
				br->height);

			int y = (int)(br->position_y + br->height) + i * br->line_dy - (int)br->tick;
			// TODO: FIX THIS MAGIC NUMBER
			if(y > (int)br->position_y - 12)
				PrintText(line.c_str(), font, (int)br->position_x, y);
			else if(i == br->line_count)
				br->done = true;	

			glDisable(GL_SCISSOR_TEST);
		}
	}
	if(weather_on)
		DrawWeather();
}

void iiInstanceAllocator::SwitchAI()
{
	SetAI(!GetAI());

#ifdef _DEBUG
	GetAI() ? dbgAI = "AI on" : dbgAI = "AI off";
#endif
}

void iiInstanceAllocator::SwitchHUD()
{
	SetHUDEnabled(!GetHUDEnabled());

#ifdef _DEBUG
	GetHUDEnabled() ? dbgHUD = "HUD on" : dbgHUD = "HUD off";
#endif
}

void iiInstanceAllocator::SwitchKeyboard()
{
	SetKeyboardEnabled(!GetKeyboardEnabled());

#ifdef _DEBUG
	GetKeyboardEnabled() ? dbgKeyboard = "Keyboard on" : dbgKeyboard = "Keyboard off";
#endif
}

void iiInstanceAllocator::SwitchQuake()
{
	_current_quake_state = false;
	if(!_current_quake_state)
		StartQuake(GLOBAL_QUAKE_POWER);
	else
		StopQuake();
	_current_quake_state = !_current_quake_state;
}

void iiInstanceAllocator::SwitchLight()
{

	GLOBAL_COLOR_RED += 128;
	GLOBAL_COLOR_GREEN += 128;
	GLOBAL_COLOR_BLUE += 128;

	if(GLOBAL_COLOR_RED == 1) GLOBAL_COLOR_RED--;
	if(GLOBAL_COLOR_GREEN == 1) GLOBAL_COLOR_GREEN--;
	if(GLOBAL_COLOR_BLUE == 1) GLOBAL_COLOR_BLUE--;
}

void iiInstanceAllocator::ResetLight()
{
	GLOBAL_COLOR_RED = 0;
	GLOBAL_COLOR_GREEN = 0;
	GLOBAL_COLOR_BLUE = 0;
}

void iiInstanceAllocator::SwitchPause()
{
	_q = false;
	if(current_state != GAME_GAMEMENU)
	{
		SetPause(!GetPause());

		// Т.к. функцию SetPause юзают многие другие функции, то "затенение"
		// изображение будет идти только в этой, которая используется лишь для
		// паузы всей игры (а не только на время диалога) и для выхода в меню
		
		SwitchLight();
		if(GLOBAL_QUAKE_ENABLED)
		{
			_q = true;
			StopQuake();
		}

		_total_stop = !_total_stop;
	}
	else
		if(_q)
		{
			SwitchQuake();
			_q = false;
		}
}

#define SHOW_MENU 0
#define HIDE_MENU 1

void iiInstanceAllocator::SwitchMenu()
{
	assert(gui);
	
	if(_state == SHOW_MENU)
	{
		// запоминаем все
		_kb = GetKeyboardEnabled();
		_hud = GetHUDEnabled();
		_ps = GetPause();
		_q = GLOBAL_QUAKE_ENABLED; // TODO: fix it
		_ai = GetAI();

		// все прячем
		SetAI(false);
		SetKeyboardEnabled(false);
		SetHUDEnabled(false);
		SetPause(true);
		GLOBAL_QUAKE_ENABLED = FALSE;

		gui->wrapperShowGroup(MENU_GAME);
		_state = HIDE_MENU;

		if(!_total_stop)
			SwitchLight();
#ifdef _DEBUG
		dbgMenu = "Menu on";
#endif	
		_total_stop = true;

		current_state = GAME_GAMEMENU;
	}
	else
	{
		// восстанавливаем все
		SetKeyboardEnabled(_kb);
		SetHUDEnabled(_hud);
		SetPause(false);
		SetAI(_ai);
		GLOBAL_QUAKE_ENABLED = _q;

		gui->wrapperHideGroup(MENU_GAME);
		_state = SHOW_MENU;

#ifdef _DEBUG
		dbgMenu = "Menu off";
#endif

		SwitchLight();
		_total_stop = false;
	}
}

void iiInstanceAllocator::MapHotkey(int vk_key, p_function function)
{
	hotkeys[vk_key] = function;
}

void iiInstanceAllocator::HandleHotkey(int vk_key)
{
	hotkey_map::const_iterator it = hotkeys.find(vk_key);
	hotkey_map::iterator end = hotkeys.end();
	end--;
	bool found = false;

	if(it == end)
	{
		found = ((*it).first == vk_key);
	}
	else
		found = true;

	if(found)
	{
		p_function function = (*it).second;
		(this->*function)();
	}
}

void iiInstanceAllocator::ProcessHotkeys()
{
	for each(hotkey_map::value_type elem in hotkeys)
	{
		if(keys[elem.first])
		{
			p_function function = elem.second;
			(this->*function)();
		}
	}
}

void iiInstanceAllocator::FreeHotkeys()
{
	hotkeys.clear();
}

iiInstanceAllocator::iiInstanceAllocator()
{
	SetPause(false);
	SetAI(false);
	SetHUDEnabled(true);
	GLOBAL_QUAKE_ENABLED = FALSE;
	SetKeyboardEnabled(true);
	SetSpriteAllocator(NULL);
	SetScreenTextAllocator(NULL);
	SetCurrentDialog(NULL);
	SetCurrentBriefing(NULL);
	ResetLight();

	_prev_message_time = 0;

	_total_stop = false;

	_state = SHOW_MENU;
	_kb = true;
	_hud = true;
	_ps = false;
	_q = 0;
	_ai = false;
	_current_quake_state = false;
}


iiInstanceAllocator::~iiInstanceAllocator()
{
	StopQuake();
	FreeHotkeys();

	if(GetCurrentDialog())
	{
		GetCurrentDialog()->ClearCurrentMessage();
		GetCurrentDialog()->SetDone(true);
		SetCurrentDialog(NULL);
	}
	StopWeather();
}

