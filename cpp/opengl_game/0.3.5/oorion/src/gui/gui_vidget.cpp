#include "stdafx.h"

#include "../draw/texture.h"
#include "../draw/draw.h"

#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"




// GUIVidget Implementation
// --------------------------------------------------------------------------
int GUIVidget::GetX() const { return x; }
int GUIVidget::GetY() const { return y; }
UINT GUIVidget::GetWidth() const { return width; }
UINT GUIVidget::GetHeight() const { return height; }
texture_t* GUIVidget::GetTexture() const { return texture; }
COLOR4 GUIVidget::GetColor() const { return color; }
bool GUIVidget::GetBorder() const { return border; }
BORDER_TYPE GUIVidget::GetBorderType() const { return border_type; }

void GUIVidget::SetX(int new_x) { x = new_x; }
void GUIVidget::SetY(int new_y) { y = new_y; }
void GUIVidget::SetWidth(UINT new_width) { width = new_width; }
void GUIVidget::SetHeight(UINT new_height) { height = new_height; }
void GUIVidget::SetTexture(texture_t* new_texture) { texture = new_texture; }
void GUIVidget::SetColor(COLOR4 new_color) { color = new_color; }
void GUIVidget::SetBorder(bool new_value) { border = new_value; }
void GUIVidget::SetBorderType(BORDER_TYPE new_value) { border_type = new_value; }

void GUIVidget::Draw()
{
	if(IsVisible())
	{
		if(GetTexture())
		{	
			COLOR4 c = GetColor();
			glColor4ub(c.r, c.g, c.b, c.a);

			DrawTexture(GetX(), GetY(),
				0, 0, GetWidth(), GetHeight(),
				GetTexture(), false, false,
				100, 100);
		}
		if(GetBorder())
		{
			if(GetBorderType() == btAlways)
			{
				Box(GetX()-1, GetY()-1,	GetWidth()+1, GetHeight()+1);
			}
		}
	}
}

GUIVidget::GUIVidget()
{
	Init();
	SetX(0);
	SetY(0);
	SetWidth(0);
	SetHeight(0);
	SetColor(DEFAULT_CONTROL_COLOR);
	SetVisible(false);
	SetDead(false);
	SetTexture(NULL);
	SetParent((GUIBaseControl*)NULL);
	SetBorder(false);
	SetBorderType(btNone);
}

GUIVidget::GUIVidget(string name)
{
	Init();
	SetX(0);
	SetY(0);
	SetWidth(0);
	SetHeight(0);
	SetColor(DEFAULT_CONTROL_COLOR);
	SetVisible(false);
	SetDead(false);
	SetTexture(NULL);
	SetParent((GUIBaseControl*)NULL);
	SetName(name);
	SetBorder(false);
	SetBorderType(btNone);
}