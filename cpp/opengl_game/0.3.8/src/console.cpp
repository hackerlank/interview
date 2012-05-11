#include "stdafx.h"

#include "defines.h"

#include "draw/texture.h"
#include "draw/font.h"

#include "gui/gui.h"
#include "gui/gui_misc.h"
#include "gui/gui_msgalloc.h"
#include "gui/gui_ctrlalloc.h"
#include "gui/gui_basecontrol.h"
#include "gui/gui_vidget.h"
#include "gui/gui_control.h"
#include "gui/gui_button.h"
#include "gui/gui_textbox.h"
#include "gui/gui_memo.h"

#include "osl/osl.h"
//#include "osl/osl_types.h"
//#include "osl/osl_lexems.h"
//#include "osl/osl_parse.h"

#include "iichan/iichan.h"

#include "console.h"

//CONSOLE *console = NULL;
extern GUI* gui;
extern ii* iichan;
extern config cfg;




int console_state = CONSOLE_CLOSED;


void Console::ShowConsole()
{
	if(console_state == CONSOLE_CLOSED)
	{
		if (iichan)
			if (iichan->GetInstanceAllocator())
			{
				iichan->GetInstanceAllocator()->SetAI(false);
				iichan->GetInstanceAllocator()->SetKeyboardEnabled(false);
				iichan->GetInstanceAllocator()->SetPause(true);
			}

		vector<GUIBaseControl*>* ctrls = gui->GetControls()->GetItems();
		for(vector<GUIBaseControl*>::iterator c = ctrls->begin(); c != ctrls->end(); c++)
		{
			GUIBaseControl* slct = *c;
			if (slct->GetGroup() == GUI_CONSOLE_GROUP)
			{
				slct->SetVisible (true);
			}
			else 
			{
				controls[slct->GetId()] = slct->IsVisible();
	//			slct->SetVisible (false);
			}			
		}
		GID prev_gid = gui->GetControls()->GetFocusedControl();
		GUIBaseControl* prev = gui->GetControls()->GetControlByGID(prev_gid);
		if(prev)
			prev->SetFocus(false);
		GUIBaseControl* conTextBox = gui->GetControls()->GetControlByName(CONSOLE_NAME);
		conTextBox->SetFocus(true);

		console_state = CONSOLE_OPENED;
	}
}

void Console::HideConsole()
{
	if(console_state == CONSOLE_OPENED)
	{
		vector<GUIBaseControl*>* ctrls = gui->GetControls()->GetItems();
		for(vector<GUIBaseControl*>::iterator c =
			ctrls->begin(); c != ctrls->end(); c++)
		{
			GUIBaseControl* slct = *c;
			if (slct->GetGroup() == GUI_CONSOLE_GROUP)
				slct->SetVisible (false);
			else
			{
//				slct->SetVisible (controls[slct->GetId()]);
				controls.erase(slct->GetId());
			}

		}
		if (iichan)
			if (iichan->GetInstanceAllocator())
			{
				iichan->GetInstanceAllocator()->SetAI(true);
				iichan->GetInstanceAllocator()->SetKeyboardEnabled(true);
				iichan->GetInstanceAllocator()->SetPause(false);
			}
		console_state = CONSOLE_CLOSED;
	}
}

Console::Console()
{
	console_state = CONSOLE_CLOSED;
	CreateConsole();
}

void Console::CreateConsole()
{
	if (!gui)
	return;

	GUITextBox* ge = new GUITextBox(CONSOLE_NAME);	
	gui->GetControls()->Add(ge);

	ge -> SetWidth (cfg.scr_width - 1); // updated [ender]
	ge -> SetHeight (18);
	ge -> SetX(0); // updated [ender]
	ge -> SetY(100);
	ge -> SetBorder(1);
	ge -> SetBorderType(btAlways);
	ge -> SetVisible(false);
	//ge -> SetText("Конс...");

	char* n = new char[strlen("CreateEnemy(\"slowpoke\", 300, 311, 100); echo(\"superslowpoke!!!\")")+1];
	sprintf(n, "CreateEnemy(\"slowpoke\", 300, 311, 100); echo(\"superslowpoke!!!\")");
	ge->SetText(n);

	ge -> SetCaption("");
	ge -> SetTextFont(FontByName("default"));
	ge -> SetCaptionFont(FontByName("default"));
	ge -> SetGroup(GUI_CONSOLE_GROUP);
	ge -> SetTextX(7);
	ge -> SetTextY(3);
	ge -> SetCaptionX(0);
	ge -> SetCaptionY(-20);
	ge->SetMaxTextLength(66);
	ge->SetTexture(NULL);
//	ge -> SetEnabled();
	
	COLOR4 c;
	c.r = 255;
	c.g = 255;
	c.b = 255;
	c.a = 255;
	
	ge -> SetTextColor(c);

	c.r = 255;
	c.g = 255;
	c.b = 255;
	c.a = 255;
	
	ge -> SetColor(c);

	c.r = 255;
	c.g = 255;
	c.b = 255;
	c.a = 255;
	
	ge -> SetCaptionColor(c);
	ge -> SetCurrentCaptionColor(c);

	c.r = 128;
	c.g = 128;
	c.b = 128;
	c.a = 255;

	ge -> SetBackgroundColor(c);

	// [ender] addition code
	// ----------------------------------------------
	GUIMemo* memo = new GUIMemo(CONSOLE_MEMO_NAME);
	gui->GetControls()->Add(memo);
	memo->SetWidth(cfg.scr_width-1);
	memo->SetHeight(ge->GetY()-2);
	memo->SetX(0);
	memo->SetY(0);
	memo->SetBorder(1);
	memo->SetBorderType(btAlways);
	memo->SetVisible(false);
	memo->SetDY(5);
	memo->SetDX(15);
	memo->SetLinesDY(10);
	memo->SetGroup(GUI_CONSOLE_GROUP);
	memo->SetLinesFont(FontByName("default"));
	memo->SetBackgroundColor(c);
	// -----------------------------------------------

	WriteToLog(DEFAULT_CONS_LOG_NAME, "New console created");
	
}


void Console::Execute()
{
	GUITextBox* conTextBox = (GUITextBox*)gui->GetControls()->GetControlByName(CONSOLE_NAME);
	GUIMemo* conMemo = (GUIMemo*)gui->GetControls()->GetControlByName(CONSOLE_MEMO_NAME); // addition [ender]
	
	char* text = conTextBox->GetText();
	if (strlen(text) == 0)
		return;

	// Выполнение строки
	osl_Load(text, true);	

	// Очищаем TextBox консоли
	char* new_text = new char;
	*new_text = '\0';
	try
	{
		// добавляем эту строку в мемо [ender]
		conMemo->AddLine(text);
		delete[] text;
	}
	catch(...)
	{
		WriteToLog(DEFAULT_CONS_LOG_NAME, "Exception in Console::Execute() on delete[] text");
	}
	conTextBox->SetText(new_text);
	conTextBox->SetTextLength(0);
	return;
}