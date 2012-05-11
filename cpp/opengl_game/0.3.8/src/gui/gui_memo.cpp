#include "stdafx.h"
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
#include "gui_memo.h"

// GUIMemo implementation
// --------------------------------------------------------------------------

vector<string>* GUIMemo::GetLines() { return &lines; }
Font* GUIMemo::GetLinesFont() { return lines_font; }
COLOR4 GUIMemo::GetLinesColor() { return lines_color; }
Point GUIMemo::GetCaretPos() { return caret_pos; }
int GUIMemo::GetDX() { return dx; }
int GUIMemo::GetDY() { return dy; }
int GUIMemo::GetLinesDY() { return lines_dy; }
bool GUIMemo::IsEditable() { return editable; }

void GUIMemo::SetLinesFont(Font* new_value)
{ lines_font = new_value; }
void GUIMemo::SetLinesColor(COLOR4 new_value)
{ lines_color = new_value; }
void GUIMemo::SetCaretPos(Point new_value)
{ caret_pos = new_value; }
void GUIMemo::SetDY(int new_value)
{ dy = new_value; }
void GUIMemo::SetDX(int new_value)
{ dx = new_value; }
void GUIMemo::SetLinesDY(int new_value)
{ lines_dy = new_value; }
void GUIMemo::SetEditable(bool new_value)
{ editable = new_value; }
void GUIMemo::AddLine(string line)
{ lines.push_back(line); }

void GUIMemo::Draw()
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

		if(GetLinesFont())
		{
			COLOR4 t_color = GetLinesColor();
			glColor4ub(t_color.r, t_color.g, t_color.b, t_color.a);

			// drawing lines
			int i = 0;
			for(vector<string>::iterator it = lines.begin();
				it != lines.end();
				it++)
			{
				string s = *it;
				int x = GetX() + GetDX();
				int y = GetY() + GetDY() + GetLinesDY() * i;
				PrintText(s.c_str(), GetLinesFont(), x, y);
				i++;
			}
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

	//void OnKeyDown(UCHAR key);
	//void OnKeyUp(UCHAR key);

GUIMemo::GUIMemo()
{
	Init();
	SetLinesFont(NULL);
	SetLinesColor(DEFAULT_TEXT_COLOR);
	Point p = {0, 0};
	SetCaretPos(p);
	SetDY(0);
	SetDX(0);
	SetLinesDY(0);
	SetEditable(false);
}

GUIMemo::GUIMemo(string name)
{
	Init();
	SetLinesFont(NULL);
	SetLinesColor(DEFAULT_TEXT_COLOR);
	Point p = {0, 0};
	SetCaretPos(p);
	SetDY(0);
	SetDX(0);
	SetLinesDY(0);
	SetEditable(false);
	SetName(name);
}

// --------------------------------------------------------------------------