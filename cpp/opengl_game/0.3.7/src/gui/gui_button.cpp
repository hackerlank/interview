#include "stdafx.h"
#include "../draw/texture.h"
#include "../draw/font.h"

#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"
#include "gui_control.h"
#include "gui_button.h"





// GUIButton Implementation
// --------------------------------------------------------------------------

void GUIButton::OnLeftClick()
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

			SendMessage(msg);
		}
	}
}

void GUIButton::OnMouseMove()
{

	if(GetLastSentMsg() != GUI_MOUSE_MOVE)
	{
		if(sgns.size() > 0)
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

				SendMessage(msg);
			}
		}
	}
}

GUIButton::GUIButton()
{
	Init();
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
}

GUIButton::GUIButton(string name)
{
	Init();
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
	SetName(name);
}

// --------------------------------------------------------------------------