#include "stdafx.h"

#include "texture.h"

#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"




// GUIVidget Implementation
// --------------------------------------------------------------------------
int GUIVidget::GetX() { return x; }
int GUIVidget::GetY() { return y; }
UINT GUIVidget::GetWidth() { return width; }
UINT GUIVidget::GetHeight() { return height; }
texture_t* GUIVidget::GetTexture() { return texture; }
COLOR4 GUIVidget::GetColor() { return color; }

void GUIVidget::SetX(int new_x) { x = new_x; }
void GUIVidget::SetY(int new_y) { y = new_y; }
void GUIVidget::SetWidth(UINT new_width) { width = new_width; }
void GUIVidget::SetHeight(UINT new_height) { height = new_height; }
void GUIVidget::SetTexture(texture_t* new_texture) { texture = new_texture; }
void GUIVidget::SetColor(COLOR4 new_color) { color = new_color; }

void GUIVidget::Draw()
{
	__super::Draw();
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
	}
}

//void GUIVidget::Process()
//{
//	//__super::Process();
//}

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

}

//GUIVidget::GUIVidget(char* name)
//{
//	Init();
//	SetX(0);
//	SetY(0);
//	SetWidth(0);
//	SetHeight(0);
//	SetColor(DEFAULT_CONTROL_COLOR);
//	SetVisible(false);
//	SetDead(false);
//	SetTexture(NULL);
//	SetParent((GUIBaseControl*)NULL);
//	SetName(name);
//
//}

//GUIVidget::~GUIVidget()
//{
//	SetTexture(NULL);
//	if(GetName())
//		delete GetName();
//	SetName(NULL);
//}
// --------------------------------------------------------------------------


//GUIButton::GUIButton()
//{
//	SetCaption(NULL);
//	SetCaptionFont(NULL);
//	SetCaptionColor(DEFAULT_TEXT_COLOR);
//}
//
//GUIButton::GUIButton(char* name)
//{
//	SetCaption(NULL);
//	SetCaptionFont(NULL);
//	SetCaptionColor(DEFAULT_TEXT_COLOR);
//	SetName(name);
//}