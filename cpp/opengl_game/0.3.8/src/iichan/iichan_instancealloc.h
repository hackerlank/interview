#ifndef __ii_INSTANCEALLOC_H_
#define __ii_INSTANCEALLOC_H_
#include "iichan_dialog.h"
#include "iichan_scrtextalloc.h"
#include "iichan_briefing.h"
#include "iichan_weather.h"

class iiInstanceAllocator;

// Об использовании коллбеков:
//	http://alenacpp.blogspot.com/2007/04/blog-post.html
//  http://www.parashift.com/c++-faq-lite/pointers-to-members.html#faq-33.2

typedef void (iiInstanceAllocator::*p_function)();
typedef map<int, p_function> hotkey_map;

class iiInstanceAllocator
{
private:
	bool pause;
	bool ai;
	bool hud_enabled;
	bool keyboard_enabled;
	bool _total_stop;

	int _state;
	bool _kb;
	bool _hud;
	bool _ps;
	int _q;
	bool _ai;
	bool _current_quake_state;

	unsigned int _prev_message_time;

	iiSpriteAllocator* SpriteAllocator;
	iiScreenTextAllocator* ScreenTextAllocator;

	Dialog* current_dialog;
	Briefing* current_briefing;
	hotkey_map hotkeys;

public:
	void SetSpriteAllocator(iiSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	iiSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	void SetScreenTextAllocator(iiScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	iiScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	void SetCurrentDialog(Dialog* new_value) { current_dialog = new_value; }
	Dialog* GetCurrentDialog() const { return current_dialog; }

	void SetCurrentBriefing(Briefing* new_value) { current_briefing = new_value; }
	Briefing* GetCurrentBriefing() const { return current_briefing; }

	void StartDialog(Dialog* dialog);
	void StartBriefing(Briefing* briefing);
	void StartQuake(int power);
	void StopQuake();
	//void StartSnow(int power);
	//void StopSnow();
	void WeatherStart(Weather* weather, int power);
	void WeatherStop();
	void SetPause(bool new_value);
	bool GetPause() const;
	void SetAI(bool new_value) { ai = new_value; }
	bool GetAI() const { return ai; }
	void SetHUDEnabled(bool new_value) { hud_enabled = new_value; }
	bool GetHUDEnabled() const { return hud_enabled; }
	void SetKeyboardEnabled(bool new_value) { keyboard_enabled = new_value; }
	bool GetKeyboardEnabled() const { return keyboard_enabled; }

	// Несколько функций для назначения хоткеям
	// Просто переключают флаги
	void SwitchAI();
	void SwitchHUD();
	void SwitchKeyboard();
	void SwitchQuake(); // TODO: продумать как передать power
	//void SwitchSnow(); // TODO: продумать как передать power
	void SwitchPause();
	void SwitchMenu();
	void SwitchLight();

	void ResetLight();

	// Это просто назначает хоткей (добавляет в map)
	void MapHotkey(int vk_key, p_function function);

	// Хендлит нажатую кнопку (проверяет, есть ли она в map)
	// Не очень хорошо проверять *каждую* кнопку,
	// поэтому используется ProcessHotkeys(),
	// которая прогоняет все *назначенные* хоткеи (проверяет
	// нажаты они или нет)
	void HandleHotkey(int vk_key);

	// Основной цикл обработки хоткеев, используется в Process
	void ProcessHotkeys();

	void FreeHotkeys();


	void Process();
	void Draw();

	iiInstanceAllocator();
	~iiInstanceAllocator();
};

#endif