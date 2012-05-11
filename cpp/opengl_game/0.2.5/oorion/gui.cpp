#include "stdafx.h"

#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"
#include "misc.h"

#include "game.h"
#include "data.h"
#include "gui.h"



extern unsigned int current_state;
extern HDC hDC;
extern vector<font_t> fonts;
extern vector<texture_t> textures;
extern config cfg;

extern int mouseX;
extern int mouseY;
extern bool mouse[3];

extern int difficulty_pick;
extern int size_pick;
extern int race_pick;
extern int opponents_pick;

extern const LPSTR names_menu[];
extern const LPSTR names_main[];
extern const LPSTR names_difficulty[];
extern const LPSTR names_size[];
extern const LPSTR names_race[];
extern const LPSTR names_opponents[];


extern GAME* game;
extern GAMEDEFINITION gamedef;

extern int current_time;
extern int previous_time;



/*	***************************************************************************
	***************************************************************************

								GUIControl class

	***************************************************************************
	*************************************************************************** */



GUIControl::GUIControl(LPSTR ctrl_name)
{
		static int __create_count;

		__create_count++;

		_gid					= __create_count;
		_name					= NULL;
		_parent_name			= NULL;
		_next_name				= NULL;
		_messages				= NULL;
		_name					= ctrl_name;
		_parent					= NULL;
		_next					= NULL;
		_child_count			= 0;
		_mouse_left				= false;
		_mouse_right			= false;
		_mouse_x				= 0;
		_mouse_y				= 0;
		_pos_x					= 0;
		_pos_y					= 0;
		_close					= false;
		_text_color[0]			= 255;
		_text_color[1]			= 255;
		_text_color[2]			= 255;
		_text_color[3]			= 255;
		_caption_color[0]		= 255;
		_caption_color[1]		= 255;
		_caption_color[2]		= 255;
		_caption_color[3]		= 255;
		_tex					= NULL;
		_caption_font			= NULL;
		_text_font				= NULL;
		_caption				= NULL;
		_text					= NULL;
		_text_x					= 5;
		_text_y					= 5;
		_caption_dx				= 0;
		_caption_dy				= 0;
		_pos_dx					= 0;
		_pos_dy					= 0;
		_caption_x				= 5;
		_caption_y				= 5;
		_caption_center			= true;
		_text_center			= true;
		_pressed				= false;
		_dead					= false;
		_visible				= false;
		_action					= actNo;
		_data					= -1;
		_current_pattern		= 0;
		_pattern_width			= 0;
		_pattern_height			= 0;
		_pattern_count			= 0;
		_children.resize(0);
}

void GUIControl::SetTextColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
{
		_text_color[0] = clrRed;
		_text_color[1] = clrGreen;
		_text_color[2] = clrBlue;
		_text_color[3] = clrAlpha;
}

void GUIControl::SetCaptionColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
{
		_caption_color[0] = clrRed;
		_caption_color[1] = clrGreen;
		_caption_color[2] = clrBlue;
		_caption_color[3] = clrAlpha;
}

void GUIControl::SetPos(const int x, const int y)
{
		_pos_x = x; _pos_y = y;

		if(GetParent())
		{
			_pos_x += GetParent()->GetPosX();
			_pos_y += GetParent()->GetPosY();
		}
}

void GUIControl::Centrify()
{
		if(!GetParent())
		{
			if((GetPatternWidth() < cfg.scr_width) && (GetPatternHeight() < cfg.scr_height))
				SetPos(cfg.scr_width / 2 - GetPatternWidth() / 2, cfg.scr_height / 2 - GetPatternHeight() / 2);
		}
		else
		{
			if((GetPatternWidth() < GetParent()->GetPatternWidth()) &&
				(GetPatternHeight() < GetParent()->GetPatternHeight()))
					SetPos(GetParent()->GetPatternWidth() / 2 - GetPatternWidth() / 2,
					GetParent()->GetPatternHeight() / 2 - GetPatternHeight() / 2);
		}
}
			

void GUIControl::SetColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
{
		_color[0] = clrRed;
		_color[1] = clrGreen;
		_color[2] = clrBlue;
		_color[3] = clrAlpha;
}

void GUIControl::Draw()
{
		if(GetVisible())
		{
			if(GetTexture())
			{
				int tx, ty;
				glColor4ubv(_color);
				tx = _pos_x + _pos_dx;
				ty = _pos_y + _pos_dy;

				DrawTexture(tx, ty,
					GetCurrentPattern() * GetPatternWidth(),
					GetCurrentPattern() * GetPatternHeight(),
					GetCurrentPattern() * GetPatternWidth() + GetPatternWidth()-1,
					GetCurrentPattern() * GetPatternHeight() + GetPatternHeight()-1,
					_tex, false, false, 100, 100);		
			}

			if(GetTextFont() && GetText())
			{
				glColor4ubv(_text_color);

				int px,py;

				px = _pos_x + _text_x;
				py = _pos_y + _text_y;

				if(_text_center)
				{
					px += GetPatternWidth() / 4;
					//py += GetPatternHeight();
				}

				PrintText(GetText(), GetTextFont()->name, px, py);
			}

			if(GetCaptionFont() && GetCaption())
			{
				glColor4ubv(_caption_color);

				int px,py;

				px = GetPosX() + _caption_x + _caption_dx;
				py = GetPosY() + _caption_y + _caption_dy;

				if(_caption_center)
				{
					px += GetPatternWidth() / 4;
					//py += GetPatternHeight();
				}
				PrintText(GetCaption(), _caption_font->name, px, py);
			}
		}
}

void GUIControl::Process()
{
	if(GetVisible())
	{
		if(GetChildCount() == 0)
		{
			if(PointInBox(mouseX, mouseY, GetPosX(), GetPosY(), GetPatternWidth(), GetPatternHeight()))
			{
				GUIMessage temp_message;
							
				if(GetPatternCount() > 1)
					SetCurrentPattern(1);

				//temp_message.from = GetGID();
				//temp_message.a1 = MOUSE_OVER;
				//temp_message.a2 = 0;

				if(mouse[0])
				{
					_pressed = true;

					
					if(GetPatternCount() > 1)
					{
						SetCurrentPattern(2);
						_pos_dx = 1;
						_pos_dy = 1;
						if(_caption)
						{
							_caption_dx = 1;
							_caption_dy = 1;
						}
					}
				}
				else
				{
					_pos_dx = 0;
					_pos_dy = 0;
					if(_caption)
					{
						_caption_dx = 0;
						_caption_dy = 0;
					}

					if(_pressed)
					{
						_pressed = false;

						//temp_message.from = GetGID();
						temp_message.from = this;
						temp_message.a1 = MOUSE_LEFT_CLICK;
						temp_message.a2 = 0;
						temp_message.solved = false;

						SendMessage(temp_message);

					}

				}

			}

			else
			{
				if(GetCurrentPattern() != 0) SetCurrentPattern(0);
			}
		}
	}
}


void GUIControl::AddChild(GUIControl* child)
{
		child->SetMessages(GetMessages());
		_children.push_back(child);
		SetChildCount(GetChildCount() + 1);	
}

void GUIControl::SetVisible(bool flag) 
{
		_visible = flag;

		for(std::vector<GUIControl*>::iterator it = _children.begin(); it != _children.end(); it++)
		{
			(*it)->SetVisible(flag);
		}
}


void GUIControl::KillSelf()
{
		for(std::vector<GUIControl*>::iterator it = _children.begin(); it != _children.end(); it++)
		{
			this->KillChild(*it);
		}
		SetDead(true);			
}

void GUIControl::KillChild(GUIControl* child)
{
	child->KillSelf();

}

void GUIControl::OpenNext()
{
		if(GetParent())
			GetParent()->SetVisible(false);
		else
			SetVisible(false);

		if(GetNext())
			GetNext()->SetVisible(true);
}

void GUIControl::ExitProgram()
{
	current_state = GAME_EXITING;
}





/*	****************************************************************************
	****************************************************************************

								GUIMessageAllocator class

	****************************************************************************
	**************************************************************************** */


void GUIMessageAllocator::Add(GUIMessage item)
{
		messages.push_back(item);
		_message_count++;
}

void GUIMessageAllocator::Process()
{
		for(vector<GUIMessage>::iterator it = messages.begin();
			it != messages.end();
			it++)
		{
			GUIMessage msg = *it;

			if(msg.a1 == MOUSE_LEFT_CLICK)
			{
				GUIControl* ctrl = msg.from;

#ifdef _DEBUG_GUI
				char* msg = new char[strlen(ctrl->GetName()) + strlen(" pressed")+1];
				sprintf(msg, "%s pressed\0", ctrl->GetName());
				GetGUI()->LogOnScreen(msg);
#endif
				switch(ctrl->GetAction())
				{
				case 0: break;
				case 1: ctrl->OpenNext(); break;
				case 2: /* here should be StartGame */ break;
				case 3: ctrl->ExitProgram(); break;
				default: break;
				}

				switch(ctrl->GetGroup())
				{
				case gNo: break;
				case gDifficulty: difficulty_pick = ctrl->GetData(); break;
				case gRace: race_pick = ctrl->GetData(); break;
				case gSize: size_pick = ctrl->GetData(); break;
				case gOpponents: opponents_pick = ctrl->GetData(); break;
				default: break;
				}
			}
			msg.solved = true;
		}

		for(UINT i = 0; i < GetMessageCount(); i++)
		{
			if(messages[i].solved)
			{
				messages.erase(messages.begin() + i);
				_message_count--;
			}
		}
}





/*	****************************************************************************
	****************************************************************************

								GUIScreenMessageAllocator class

	****************************************************************************
	**************************************************************************** */



void GUIScreenMessageAllocator::Add(GUIScreenMessage item)
{
		messages.push_back(item);
		_message_count++;
}

void GUIScreenMessageAllocator::Delete(GUIScreenMessage* item)
{
		delete[] item->message;
}
		
void GUIScreenMessageAllocator::Draw()
{
		UINT Count = 0;
		for(vector<GUIScreenMessage>::iterator it = messages.begin();
			it != messages.end();
			it++)
		{
			Count++;
			GUIScreenMessage msg = *it;
			PrintText(msg.message, LOG_SCREEN_FONT, LOG_SCREEN_X, LOG_SCREEN_Y + Count * LOG_SCREEN_DY);
		}
}


void GUIScreenMessageAllocator::Process()
{
	static int startup_time = GetTickCount();

	_tick2 = GetTickCount() - startup_time;

		if(_tick2 - _tick1 >= 1000)
		{
			_tick1 = _tick2;
			for(vector<GUIScreenMessage>::iterator it = messages.begin();
				it != messages.end();
				it++)
			{
				GUIScreenMessage* msg = &(*it);
				msg->time_remains--;

				if(msg->time_remains <= 0)
					Delete(msg);
			}

			UINT i = 0;
			while(i < GetMessageCount())
			{
				if(messages[i].time_remains <= 0)
				{
					messages.erase(messages.begin()+i);
					SetMessageCount(GetMessageCount()-1);
					continue;
				}
				else
					i++;
			}

		}
}








/*	****************************************************************************
	****************************************************************************

								GUIControlAllocator class

	****************************************************************************
	**************************************************************************** */


GUIControlAllocator::GUIControlAllocator(GUIMessageAllocator* msg_alloc)
{
		controlCount = 0;
		controls.resize(0);
		messages = msg_alloc;
}

void GUIControlAllocator::Add(GUIControl* item)
{
		item->SetMessages(messages);
		controls.push_back(item);
		controlCount++;
}

void GUIControlAllocator::Delete(GUIControl* item)
{
		delete item;
}

void GUIControlAllocator:: Draw()
{
		for(vector<GUIControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
		{
			GUIControl* ctrl = *it;
			ctrl->Draw();
		}
}

void GUIControlAllocator::Process()
{
		for(vector<GUIControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
		{
			GUIControl* ctrl = *it;
			ctrl->Process();
		}
		for(UINT i = 0; i < controlCount; i++)
		{
			if(controls[i]->GetDead())
			{
				Delete(controls[i]);
				controls.erase(controls.begin()+i);
				controlCount--;
			}
		}
}

LPSTR GUIControlAllocator::NameByGID(GID guid)
{
		for(vector<GUIControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
			if((*it)->GetGID() == guid)
				return (*it)->GetName();
		return NULL;
}

GUIControl* GUIControlAllocator::ItemByGID(GID guid)
{
		for(std::vector<GUIControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
			if((*it)->GetGID() == guid)
				return (*it);
		return NULL;
}

GUIControl* GUIControlAllocator::ItemByName(LPSTR name)
{
		if(name)
			for(std::vector<GUIControl*>::iterator it = controls.begin();
				it != controls.end();
				it++)
				if(strcmp((*it)->GetName(), name) == 0)
					return (*it);
			return NULL;
}

void GUIControlAllocator::ViewControl(char* name)
{
		for(vector<GUIControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
		{
			(*it)->SetVisible(false);
			if(strcmp(name, (*it)->GetName()) == 0)
				(*it)->SetVisible(true);
		}
}	






/*	****************************************************************************
	****************************************************************************

								GUI class

	****************************************************************************
	**************************************************************************** */









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
		Messages->Process();
		Controls->Process();
		ScreenMessages->Process();
}

void GUI::LogOnScreen(char* message)
{
		GUIScreenMessage msg;
		msg.time_remains = LOG_SCREEN_TIME;
		msg.message = message;

		ScreenMessages->Add(msg);
}