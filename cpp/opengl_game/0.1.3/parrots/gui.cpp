#include "stdafx.h"
#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"
#include "misc.h"

#include "gui.h"

#include "memory.h"

extern int mouseX;
extern int mouseY;
extern bool mouse[3];

extern texture_t textures[256];
extern font_t fonts[256];
extern config cfg;

#define MOUSE_LEFT_CLICKED 0x0FF0
#define MOUSE_RIGHT_CLICKED 0x0FF1
#define MOUSE_OVER 0x0FF2

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

						if(_caption)
						{
							_caption_dx = 1;
							_caption_dy = 1;
						}
					}
				}
				else
				{
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

						switch(action)
						{
							case actNo: break;
							case actClose:
								{
									Close();
									break;
								}
							case actOpenNext:
								{
									OpenNext();
									break;
								}
							case actOpenPrevious:
								{
									OpenPrevious();
									break;
								}
							case actExit:
								{
									ExitProgram();
									break;
								}
							default: break;
						}

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

void GUIcontrol::ProcessAll()
{
	for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
	{
		if((*it))
		(*it)->ProcessAll();
	}

	Process();
}

void GUI::Process()
{
		now_tick = GetTickCount() - prev_tick;

		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if((*it))
			{
				(*it)->ProcessAll();
				if((*it)->IWannaDie) DeleteControl(*it); // this totally sux, i expect memory leaks
				// DO NOT WANT MEMORY LEAKS
			}
		}

		for(std::vector<GUIMessage>::iterator it = guima.messages.begin(); it != guima.messages.end(); it++)
		{
			string msg;
			//char guid[10];

			//itoa((*it).from, guid, 10);

			msg = FindByGID((*it).from);
			msg += string(" pressed");
			
			//msg = string(guid) + string(" pressed");

			LogOnScreen(msg);
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





