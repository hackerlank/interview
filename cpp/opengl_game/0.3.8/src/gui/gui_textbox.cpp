#include "stdafx.h"
#include "../draw/texture.h"
#include "../draw/font.h"
#include "../draw/draw.h"
#include "../misc.h"
#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"
#include "gui_control.h"
#include "gui_textbox.h"
#include "../console.h"

extern Console *console;
extern bool keys[256];

const UINT DEFAULT_MAX_TEXT_LENGTH = 12;



// GUITextBox implementation
// --------------------------------------------------------------------------

char* GUITextBox::GetText() { return text; }
Font* GUITextBox::GetTextFont() { return text_font; }
COLOR4 GUITextBox::GetTextColor() { return text_color; }
UINT GUITextBox::GetTextCaretPos() { return text_caret_pos; }
int GUITextBox::GetTextX() { return text_x; }
int GUITextBox::GetTextY() { return text_y; }
bool GUITextBox::GetShift() { return shift; }
UINT GUITextBox::GetTextLength() { return text_length; }
UINT GUITextBox::GetMaxTextLength() { return max_text_length; }

void GUITextBox::SetText(char* new_text)
{ text = new_text; }
void GUITextBox::SetTextFont(Font* new_text_font)
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
		COLOR4 c = GetBackgroundColor();
		glColor4ub(c.r, c.g, c.b, c.a);

		glRecti(GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight());

		COLOR4 color = GetColor();
		glColor4ub(color.r, color.g, color.b, color.a);

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


void GUITextBox::OnKeyDown(UCHAR key, UCHAR ascii)
{
	/*char *c = new char[10];
	sprintf(c, "%d\ - %C\0", ascii, ascii);
	extern GUI *gui;
	gui->LogOnScreen(c);*/
	if(GetText())
	{
		if((ascii >= 32) &&  (GetTextLength() < GetMaxTextLength()))
		{
			char* new_text = new char[strlen(GetText())+2];
			sprintf(new_text, "%s%c\0", GetText(), ascii);
			
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
				char* new_text = new char[len];
				memcpy(new_text, GetText(), len-1);
				*(new_text+len-1) = '\0';
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
		else if (key == VK_RETURN)
		{
			if (GetName() == CONSOLE_NAME)
				console->Execute();
		}
	}
	else
	{
		char* new_text = new char[1];
		new_text[0] = ascii;
		SetText(new_text);
	}
}


void GUITextBox::OnKeyUp(UCHAR key)
{
	if(key == VK_SHIFT)
		SetShift(false);
}