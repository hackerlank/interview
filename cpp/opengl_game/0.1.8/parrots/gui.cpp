#include "stdafx.h"

#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"
#include "misc.h"

#include "game.h"
#include "data.h"
#include "gui.h"

#include "memory.h"


extern int mouseX;
extern int mouseY;
extern bool mouse[3];

extern int difficulty_pick;
extern int size_pick;
extern int race_pick;
extern int opponents_pick;


//#define MOUSE_LEFT_CLICKED 0x0FF0
//#define MOUSE_RIGHT_CLICKED 0x0FF1
//#define MOUSE_OVER 0x0FF2
//
//const int menu_length = 5;
//const int main_length = 5;
//const int difficult_length = 4;
//const int size_length = 5;
//const int race_length = 10;
//const int opponents_length = 5;

extern const LPSTR names_menu[];
extern const LPSTR names_main[];
extern const LPSTR names_difficulty[];
extern const LPSTR names_size[];
extern const LPSTR names_race[];
extern const LPSTR names_opponents[];


extern GAME* game;


void GUIcontrol::Process()
{
	if(visible)
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
					//mouse[0] = false;

					
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

						temp_message.from = GetGID();
						temp_message.a1 = MOUSE_LEFT_CLICKED;
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

void GUI::Process()
{
		now_tick = GetTickCount() - prev_tick;

		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if((*it))
			{
				(*it)->Process();
				if((*it)->IWannaDie) DeleteControl(*it); // this totally sux, i expect memory leaks

				if(!(*it)->GetNext())
					if((*it)->GetNextName())
					{
						(*it)->SetNext(ItemByName((*it)->GetNextName()));

						if(!(*it)->GetNext())
						{
							char output[50];
							sprintf(output, "Error: could not set next for %s [NextName=%s]", (*it)->GetName(), (*it)->GetNextName());
							LogToFile(output);
						}
					}
			}
		}

	for(std::vector<GUIMessage>::iterator it = guima.messages.begin(); it != guima.messages.end(); it++)
	{
		if((*it).a1 == MOUSE_LEFT_CLICKED)
		{
			GUIcontrol* ctrl;
			ctrl = ItemByGID((*it).from);
#ifdef _DEBUG_GUI
			string msg;
			msg = ctrl->GetName();
			msg += string(" pressed");		
			LogOnScreen(msg);
#endif
			switch(ctrl->GetAction())
			{
			case 0: break;
			case 1:
				{
					ctrl->OpenNext();
					break;
				}
			case 2:
				{
					// here should be StartGame
					break;
				}
			case 3:
				{
					ctrl->ExitProgram();
					break;
				}
			case 4:
				{
					// here should be difficulty set
					difficulty_pick = ctrl->GetData();
					break;
				}
			case 5:
				{
					// here should be size set
					size_pick = ctrl->GetData();
					break;
				}
			case 6:
				{
					// here should be race set
					race_pick = ctrl->GetData();
					break;
				}
			case 7:
				{
					// here should be opponents set
					opponents_pick = ctrl->GetData();
					break;
				}
			default: break;
			}
		}
		(*it).solved = true;
	}

		for(int i = 0; i < guima.messagesCount; i++)
		{
			if(guima.messages[i].solved)
			{
				guima.messages.erase(guima.messages.begin() + i);
				guima.messagesCount--;
			}
		}

		if(GetTickCount() - prev_tick >= 1000)
		{
			prev_tick = GetTickCount();


			for(std::vector<QMessage>::iterator it = queue.begin(); it != queue.end(); it++)
			{
				it->time_remains--;

			}

			for(int i = 0; i < queueCount; i++)
			{
				if(queue[i].time_remains <= 0)
				{
					queue.erase(queue.begin() + i);
					queueCount--;
				}
			}
		}

}

void GUI::ParseDefinitionElem(struct GUI_DEFINITION_ELEM* guielem)
{
	GUIcontrol* new_ctrl = NULL;

	new_ctrl = new GUIcontrol(guielem->name);

		new_ctrl->SetPos(guielem->x, guielem->y);
		new_ctrl->SetPatternCount(guielem->pattern_count);
		new_ctrl->SetPatternWidth(guielem->pattern_width);
		new_ctrl->SetPatternHeight(guielem->pattern_height);
		new_ctrl->SetCurrentPattern(guielem->pattern_current);
		new_ctrl->SetVisible(guielem->visible);
		new_ctrl->SetTexture(TextureByName(textures, guielem->texture_name));
		new_ctrl->SetColor(guielem->color_red, guielem->color_green, guielem->color_blue, guielem->color_alpha);
		new_ctrl->SetCaption(guielem->caption);
		new_ctrl->SetCaptionFont(FontByName(fonts, guielem->caption_font_name));
		new_ctrl->SetCaptionColor(guielem->caption_color_red, guielem->caption_color_green,
			guielem->caption_color_blue, guielem->caption_color_alpha);
		new_ctrl->SetCaptionPos(guielem->caption_x, guielem->caption_y);
		new_ctrl->SetText(guielem->text);
		new_ctrl->SetTextFont(FontByName(fonts, guielem->text_font_name));
		new_ctrl->SetTextColor(guielem->text_color_red, guielem->text_color_green,
			guielem->text_color_blue, guielem->text_color_alpha);
		new_ctrl->SetTextPos(guielem->text_x, guielem->text_y);
		new_ctrl->SetAction(ACTION(guielem->action));
		new_ctrl->SetNextName(guielem->next);
		new_ctrl->SetParentName(guielem->parent);
		new_ctrl->SetData(guielem->data);

	AddControl(new_ctrl);
};


void GUI::LoadFromDefinition(struct GUIDEFINITION* guidef)
{
	int i = -1;
	for(std::vector<GUI_DEFINITION_ELEM>::iterator it = guidef->elements.begin(); it != guidef->elements.end(); it++)
	{
		i++;
		ParseDefinitionElem(&guidef->elements[i]);
	}
	i = -1;
	for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
	{
		i++;
		
		GUIcontrol* prnt = ItemByName((*it)->GetParentName());

		if(prnt)
		{
			(*it)->SetParent(prnt);
			(*it)->SetPos((*it)->GetParent()->GetPosX() + (*it)->GetPosX(),
				(*it)->GetParent()->GetPosY() + (*it)->GetPosY());
			(*it)->GetParent()->AddChild((*it));
#ifdef _DEBUG_LOAD_GUI
			char output[50];

			sprintf(output, "%s is set as a parent to %s", (*it)->GetParentName(), (*it)->GetName());
			LogToFile(output);
#endif
		}
	}
}