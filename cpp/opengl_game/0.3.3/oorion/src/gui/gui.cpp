#include "stdafx.h"
#include "../draw/texture.h"
#include "../draw/font.h"
#include "../draw/draw.h"
#include "../config.h"
#include "../misc.h"
#include "../gui/gui.h"
#include "../scene.h"

#include "../gui/gui_misc.h"

#include "../gui/gui_ctrlalloc.h"
#include "../gui/gui_msgalloc.h"
#include "../gui/gui_scrmsgalloc.h"



// EXTERNS
// --------------------------------------------------------------------------
extern config cfg;
extern int mouseX;
extern int mouseY;
extern bool mouse[3];
extern unsigned int current_state;
// --------------------------------------------------------------------------


// GUI Implementation
// --------------------------------------------------------------------------
GUI::GUI()
{
		ScreenMessages = new GUIScreenMessageAllocator();
		Messages = new GUIMessageAllocator();
		Controls = new GUIControlAllocator(Messages);
		Messages->SetGUI(this);
}

void GUI::Draw()
{
		Controls->Draw();
		ScreenMessages->Draw();
}

void GUI::Process()
{
	if(Messages)
		Messages->Process();
	if(Controls)
		Controls->Process();
	if(ScreenMessages)
		ScreenMessages->Process();
}

void GUI::LogOnScreen(char* message)
{
		GUIScreenMessage msg;
		msg.time_remains = LOG_SCREEN_TIME;
		msg.message = message;

		ScreenMessages->Add(msg);
}

GUIControlAllocator* GUI::GetControls()
{
	return Controls;
}

void GUI::SetControls(GUIControlAllocator* new_controls)
{
	Controls = new_controls;
}

GUIScreenMessageAllocator* GUI::GetScreenMessages()
{
	return ScreenMessages;
}

void GUI::SetScreenMessages(GUIScreenMessageAllocator* new_screen_messages)
{
	ScreenMessages = new_screen_messages;
}

GUIMessageAllocator* GUI::GetMessages()
{
	return Messages;
}

void GUI::SetMessages(GUIMessageAllocator* new_messages)
{
	Messages = new_messages;
}

void GUI::FreeControls()
{
	if(GetControls())
	{
		GetControls()->Clear();
		delete GetControls();
		SetControls(NULL);
	}
}

void GUI::FreeMessages()
{
	if(GetMessages())
	{
		GetMessages()->Clear();
		delete GetMessages();
		SetMessages(NULL);
	}
}

void GUI::FreeScreenMessages()
{
	if(GetScreenMessages())
	{
		GetScreenMessages()->Clear();
		delete GetScreenMessages();
		SetScreenMessages(NULL);
	}
}
// --------------------------------------------------------------------------