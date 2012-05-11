#include "stdafx.h"
#include "../misc.h"
#include "../draw/texture.h"
#include "../draw/font.h"
#include "../draw/draw.h"


#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"
#include "gui_control.h"



// EXTERNS
// --------------------------------------------------------------------------
extern int mouseX;
extern int mouseY;
extern bool mouse[3];
extern bool keys[256];
extern vector<UCHAR> pressed_keys;
extern unsigned int current_time;
// --------------------------------------------------------------------------



// GUIControl Implementation
// --------------------------------------------------------------------------
UINT GUIControl::GetPatternCount() { return pattern_count; }
UINT GUIControl::GetCurrentPattern() { return current_pattern; }
bool GUIControl::IsEnabled() { return enabled; }
int GUIControl::GetLastCursorPos() { return last_cursor_pos; }
int GUIControl::GetLastPressState() { return last_press_state; }
int GUIControl::GetLastSentMsg() { return last_sent_msg; }
UCHAR GUIControl::GetLastPressedKey() { return last_pressed_key; }
char* GUIControl::GetCaption() { return caption; }
Font* GUIControl::GetCaptionFont() { return caption_font; }
COLOR4 GUIControl::GetCaptionColor() { return caption_color; }
COLOR4 GUIControl::GetSelectedCaptionColor() { return selected_caption_color; }
COLOR4 GUIControl::GetPressedCaptionColor() { return pressed_caption_color; }
COLOR4 GUIControl::GetCurrentCaptionColor() { return current_caption_color; }
COLOR4 GUIControl::GetBackgroundColor() { return background_color; }

int GUIControl::GetCaptionX() { return caption_x; }
int GUIControl::GetCaptionY() { return caption_y; }


void GUIControl::SetPatternCount(UINT new_pattern_count)
{ pattern_count = new_pattern_count; }
void GUIControl::SetCurrentPattern(UINT new_current_pattern)
{ current_pattern = new_current_pattern; }
void GUIControl::SetEnabled(bool new_enabled)
{ enabled = new_enabled; }
void GUIControl::SetLastCursorPos(int new_last_cursor_pos)
{ last_cursor_pos = new_last_cursor_pos; }
void GUIControl::SetLastPressState(int new_last_press_state)
{ last_press_state = new_last_press_state; }
void GUIControl::SetLastSentMsg(int new_last_sent_msg)
{ last_sent_msg = new_last_sent_msg; }
void GUIControl::SetLastPressedKey(UCHAR new_last_pressed_key)
{ last_pressed_key = new_last_pressed_key; }
void GUIControl::SetCaption(char *new_caption)
{ caption = new_caption; }
void GUIControl::SetCaptionFont(Font* new_caption_font)
{ caption_font = new_caption_font; }
void GUIControl::SetCaptionColor(COLOR4 new_caption_color)
{ caption_color = new_caption_color; }
void GUIControl::SetSelectedCaptionColor(COLOR4 new_selected_caption_color)
{ selected_caption_color = new_selected_caption_color; }
void GUIControl::SetPressedCaptionColor(COLOR4 new_pressed_caption_color)
{ pressed_caption_color = new_pressed_caption_color; }
void GUIControl::SetCurrentCaptionColor(COLOR4 new_current_caption_color)
{ current_caption_color = new_current_caption_color; }
void GUIControl::SetBackgroundColor(COLOR4 new_background_color)
{ background_color = new_background_color; }
void GUIControl::SetCaptionX(int new_caption_x)
{ caption_x = new_caption_x; }
void GUIControl::SetCaptionY(int new_caption_y)
{ caption_y = new_caption_y; }

void GUIControl::AddSgn(UINT sgn, UINT param_1, UINT param_2)
{
	GUISignal d;

	d.sgn = sgn;
	d.sgn_param_1 = param_1;
	d.sgn_param_2 = param_2;

	sgns.push_back(d);
}

void GUIControl::Draw()
{
	if(IsVisible())
	{
		COLOR4 c = GetBackgroundColor();
		glColor4ub(c.r, c.g, c.b, c.a);

		glRecti(GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight());
		if(GetTexture())
		{	
			COLOR4 c = GetColor();
			glColor4ub(c.r, c.g, c.b, c.a);

			if(GetCurrentPattern() == 0)
			{
				DrawTexture(GetX(), GetY(),
					0, 0,
					GetWidth(), GetHeight(),
					GetTexture(),
					false, false, 100, 100);
			}
			else
			{
				DrawTexture(GetX(), GetY(),
					GetCurrentPattern() * GetWidth(),
					0,
					GetCurrentPattern() * GetWidth() + GetWidth(),
					GetHeight(),
					GetTexture(),
					false, false, 100, 100);
			}
		}

		if(GetCaption() && GetCaptionFont())
		{
			COLOR4 c = GetCurrentCaptionColor();
			glColor4ub(c.r, c.g, c.b, c.a);

			PrintText(GetCaption(),
				GetCaptionFont(),
				GetX() + GetCaptionX(),
				GetY() + GetCaptionY());
		}

		if(GetBorder())
		{
			if(GetBorderType() == btAlways)
			{
				Box(GetX(), GetY(), GetWidth(), GetHeight());
			}
			else if(GetBorderType() == btOnMouseMove)
			{
				if(_draw_border)
					Box(GetX(), GetY(), GetWidth(), GetHeight());
			}
		}
	}
}

void GUIControl::Process()
{
	__super::Process();

	if(IsVisible() && IsEnabled())
	{
		if(PointInBox(mouseX, mouseY, GetX(), GetY(), GetWidth(), GetHeight()))
		{

			int cursor = GetLastCursorPos();
			SetLastCursorPos(1); // mouse is in

			// here
			// 0 - init
			// 1 - mouse was IN control
			// 2 - mouse was OUT control

			if(cursor == 0 || cursor == 2)
			{
				OnMouseEnter();
			}
			else
			{
				OnMouseMove();
			}

			if(mouse[0] || mouse[1])
			{
				if(GetLastPressState() == 0)
				{
					OnMouseDown();

					if(mouse[0])
						SetLastPressState(1);
					else if(mouse[1])
						SetLastPressState(2);
				}
			}
			else
			{
				int press_state = GetLastPressState();
				SetLastPressState(0);

				// here
				// 0 - init
				// 1 - left mouse down
				// 2 - right mouse down
				// 3 - all up

				if(press_state == 1)
				{
					OnLeftClick();
					OnMouseUp();
				}
				else if(press_state == 2)
				{
					OnRightClick();
					OnMouseUp();
				}
			}
		}
		else
		{
			int cursor = GetLastCursorPos();
			SetLastCursorPos(2); // mouse is out

			if(cursor == 1)
			{
				OnMouseLeave();
				SetLastPressState(0);
			}
		}


		// TODO:
		// fix it somehow :(
		if(IsFocused())
		{
			static UCHAR count = 0;
			unsigned int delay = 0;
			BYTE keyState[256];
			WORD ascii[2];
			GetKeyboardState(keyState);
			for(UCHAR i = 0; i < 255; i++)
			{			
				if(GetAsyncKeyState(i) & 0x8000)
				{
					count++;
					static unsigned int prev;
					count == 1 ? delay = KEY_FIRST_DELAY : delay = KEY_DELAY;

					if(current_time - prev >= delay)
					{
						if((i != VK_SHIFT) && (i != VK_LSHIFT) && (i != VK_RSHIFT))
						{
							int kod = ToAscii(i, MapVirtualKey(i, 0), keyState, ascii, 1);
							if (kod == 0)			// Нельзя интепретировать символ
							{
								ascii[0] = '\0';	// Поэтому мы просто передадим 0, чтобы ничего не ввелось
							}
							OnKeyDown(i, (UCHAR)ascii[0]);
							prev = current_time;
						}
					}
				}
			}
		}
	}
}





void GUIControl::SendMessage(GUIMessage msg)
{
	messages->Add(msg);
	SetLastSentMsg(msg.msg);
}

void GUIControl::OnMouseMove()
{
	if(sgns.size() > 0)
	{
		if(GetLastSentMsg() != GUI_MOUSE_MOVE)
		{
			for(vector<GUISignal>::iterator it = sgns.begin();
				it != sgns.end();
				it++)
			{
				GUIMessage msg;

				msg.sgn = *it;
				msg.msg = GUI_MOUSE_MOVE;
				msg.solved = false;
				msg.from = GetId();
				msg.a = 0;

				SendMessage(msg);
			}
		}
	}
	else
	{
		if(GetLastSentMsg() != GUI_MOUSE_MOVE)
		{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_MOVE;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
}

void GUIControl::OnMouseEnter()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_ENTER;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_ENTER;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
	}

	if(GetPatternCount() > 1)
		SetCurrentPattern(1);

	SetCurrentCaptionColor(GetSelectedCaptionColor());
}

void GUIControl::OnMouseLeave()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_LEAVE;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_LEAVE;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
	}

	if(GetPatternCount() > 0)
		SetCurrentPattern(0);

	SetCurrentCaptionColor(GetCaptionColor());
}

void GUIControl::OnMouseDown()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_DOWN;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_DOWN;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
	}


	if(GetPatternCount() >= 2)
		SetCurrentPattern(2);

	SetCurrentCaptionColor(GetPressedCaptionColor());
}

void GUIControl::OnMouseUp()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_UP;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_UP;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
	}


	if(GetPatternCount() >= 2)
		SetCurrentPattern(1);

	SetCurrentCaptionColor(GetSelectedCaptionColor());
}

void GUIControl::OnLeftClick()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_LEFT_CLICK;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_LEFT_CLICK;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
	}

}

void GUIControl::OnRightClick()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_RIGHT_CLICK;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_MOUSE_RIGHT_CLICK;
			msg.solved = false;
			msg.from = GetId();
			msg.a = 0;

			SendMessage(msg);
	}

}

void GUIControl::OnKeyUp(UCHAR key)
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_KEY_UP;
			msg.solved = false;
			msg.from = GetId();
			msg.a = key;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_KEY_UP;
			msg.solved = false;
			msg.from = GetId();
			msg.a = GetLastPressedKey();

			SendMessage(msg);
	}

}

void GUIControl::OnKeyDown(UCHAR key, UCHAR ascii)
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_KEY_DOWN;
			msg.solved = false;
			msg.from = GetId();
			msg.a = key;

			SendMessage(msg);
		}
	}
	else
	{
			GUIMessage msg;

			msg.sgn = NO_SIGNAL;
			msg.msg = GUI_KEY_DOWN;
			msg.solved = false;
			msg.from = GetId();
			msg.a = GetLastPressedKey();

			SendMessage(msg);
	}

}

GUIControl::GUIControl()
{
	prev_key_update = 0;
	SetPatternCount(0);
	SetCurrentPattern(0);
	SetFocus(true);
	SetEnabled(true);
	SetDead(false);
	SetLastCursorPos(0);
	SetLastPressState(0);
	SetLastSentMsg(0);
	SetLastPressedKey(0);
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
	SetSelectedCaptionColor(DEFAULT_TEXT_COLOR);
	SetPressedCaptionColor(DEFAULT_TEXT_COLOR);
	SetBackgroundColor(DEFAULT_BACKGROUND_COLOR);
	SetCaptionX(0);
	SetCaptionY(0);
	sgns.resize(0);
	_draw_border = false;
}

GUIControl::GUIControl(string name)
{
	GUIControl();
	SetName(name);
}
// --------------------------------------------------------------------------