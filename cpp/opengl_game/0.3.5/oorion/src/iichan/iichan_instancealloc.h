#ifndef __IICHAN_INSTANCEALLOC_H_
#define __IICHAN_INSTANCEALLOC_H_
//#include "iichan_itemalloc.h"
#include "iichan_dialog.h"

//#include "iichan_spritealloc.h"
#include "iichan_scrtextalloc.h"

class IICHANInstanceAllocator
{
private:
	bool pause;
	bool ai;
	bool hud_enabled;
	bool keyboard_enabled;

	int _prev_message_time;

	IICHANSpriteAllocator* SpriteAllocator;
	IICHANScreenTextAllocator* ScreenTextAllocator;

	DIALOG* current_dialog;

public:
	void SetPause(bool new_value) { pause = new_value; }
	bool GetPause() const { return pause; }

	void SetAI(bool new_value) { ai = new_value; }
	bool GetAI() const { return ai; }

	void SetHUDEnabled(bool new_value) { hud_enabled = new_value; }
	bool GetHUDEnabled() const { return hud_enabled; }

	void SetKeyboardEnabled(bool new_value) { keyboard_enabled = new_value; }
	bool GetKeyboardEnabled() const { return keyboard_enabled; }

	void SetSpriteAllocator(IICHANSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	IICHANSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	void SetScreenTextAllocator(IICHANScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	IICHANScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	void SetCurrentDialog(DIALOG* new_value) { current_dialog = new_value; }
	DIALOG* GetCurrentDialog() const { return current_dialog; }

	void StartDialog(DIALOG* dialog);

	void Process();

	IICHANInstanceAllocator()
	{
		SetPause(false);
		SetAI(false);
		SetHUDEnabled(true);
		//SetSpriteAllocator(NULL);
		SetScreenTextAllocator(NULL);
		SetCurrentDialog(NULL);

		_prev_message_time = 0;
	}
};

#endif