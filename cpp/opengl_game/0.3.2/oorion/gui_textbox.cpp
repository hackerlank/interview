#include "stdafx.h"
#include "texture.h"
#include "font.h"
#include "draw.h"
#include "misc.h"
#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"
#include "gui_control.h"
#include "gui_textbox.h"

extern bool keys[256];

const UINT DEFAULT_MAX_TEXT_LENGTH = 12;



// GUITextBox implementation
// --------------------------------------------------------------------------

char* GUITextBox::GetText() { return text; }
font_t* GUITextBox::GetTextFont() { return text_font; }
COLOR4 GUITextBox::GetTextColor() { return text_color; }
UINT GUITextBox::GetTextCaretPos() { return text_caret_pos; }
int GUITextBox::GetTextX() { return text_x; }
int GUITextBox::GetTextY() { return text_y; }
bool GUITextBox::GetShift() { return shift; }
UINT GUITextBox::GetTextLength() { return text_length; }
UINT GUITextBox::GetMaxTextLength() { return max_text_length; }

void GUITextBox::SetText(char* new_text)
{ text = new_text; }
void GUITextBox::SetTextFont(font_t* new_text_font)
{ text_font = new_text_font; }
void GUITextBox::SetTextColor(COLOR4 new_text_color)
{ text_color = new_text_color; }
void GUITextBox::SetTextCaretPos(UINT new_text_caret_pos)
{ text_caret_pos = new_text_caret_pos; }
void GUITextBox::SetTextX(int new_text_x)
{ text_x = new_text_x; }
void GUITextBox::SetTextY(int new_text_y)
{ text_y = new_text_y; }
void GUITextBox::SetShift(bool new_shift)
{ shift = new_shift; }
void GUITextBox::SetTextLength(UINT new_text_length)
{ text_length = new_text_length; }
void GUITextBox::SetMaxTextLength(UINT new_max_text_length)
{ max_text_length = new_max_text_length; }

void GUITextBox::Draw()
{
	if(IsVisible())
	{
		COLOR4 color = GetColor();
		Box(GetX(), GetY(), GetWidth(), GetHeight(),
			color.r, color.g, color.b, color.a);

		if(GetCaption() && GetCaptionFont())
		{
			COLOR4 c_color = GetCaptionColor();
			glColor4ub(c_color.r, c_color.g, c_color.b, c_color.a);
			PrintText(GetCaption(), GetCaptionFont(),
				GetX() + GetCaptionX(), GetY() + GetCaptionY());
		}

		if(GetTextFont())
		{
			COLOR4 t_color = GetTextColor();
			glColor4ub(t_color.r, t_color.g, t_color.b, t_color.a);
			PrintText(GetText(), GetTextFont(),
				GetX() + GetTextX(), GetY() + GetTextY());
		}
	}
}

GUITextBox::GUITextBox()
{
	Init();
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
	SetText("");
	SetTextFont(NULL);
	SetTextColor(DEFAULT_TEXT_COLOR);
	SetTextCaretPos(0);
	SetShift(false);
	SetTextLength(0);
	SetMaxTextLength(DEFAULT_MAX_TEXT_LENGTH);
}

GUITextBox::GUITextBox(string name)
{
	Init();
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
	SetText(NULL);
	SetTextFont(NULL);
	SetTextColor(DEFAULT_TEXT_COLOR);
	SetTextCaretPos(0);
	SetShift(false);
	SetName(name);
	SetTextLength(0);
	SetMaxTextLength(DEFAULT_MAX_TEXT_LENGTH);
}

void GUITextBox::OnKeyDown(UCHAR key)
{
	SetShift(keys[VK_SHIFT]);

	if(GetText())
	{
		if((IsAlpabetic(key) || key == VK_SPACE) &&
			GetTextLength() < GetMaxTextLength())
		{
			char* new_text = new char[strlen(GetText())+2];
			if(GetShift())
				sprintf(new_text, "%s%c\0", GetText(), key);
			else
				sprintf(new_text, "%s%c\0", GetText(), tolower(key));

			try
			{
				delete[] GetText();
			}
			catch(...)
			{
				WriteToLog("Exception in GUITextBox::OnKeyDown()");
			}
			SetText(new_text);
			SetTextLength(strlen(GetText()));
		}
		else if(key == VK_BACK)
		{
			UINT len = strlen(GetText());
			if(len > 0)
			{
				string s = string(GetText());
				char* new_text = new char[len];

				sprintf(new_text, "%s\0", s.substr(0, s.length()-1).c_str());
				

				try
				{
					delete[] GetText();
				}
				catch(...)
				{
					WriteToLog("Exception in GUITextBox::OnKeyDown()");
				}
				SetText(new_text);
				SetTextLength(strlen(GetText()));
			}
		}
	}
	else
	{
		char* new_text = new char[1];
		if(GetShift())
			new_text[0] = key;
		else
			new_text[0] = tolower(key);
		SetText(new_text);
	}
}

void GUITextBox::OnKeyUp(UCHAR key)
{
	if(key == VK_SHIFT)
		SetShift(false);
}