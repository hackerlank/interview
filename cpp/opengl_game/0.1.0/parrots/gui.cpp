#include "stdafx.h"
#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"

#include "gui.h"
#include "misc.h"
#include "memory.h"

extern int mouseX;
extern int mouseY;
extern bool mouse[3];

extern texture_t textures[256];
extern font_t fonts[256];
extern config cfg;

void GUIcontrol::CloseAction()
{
	//if(parent) parent->KillChild(this);
	if(parent) parent->KillSelf();
	
}

void GUIcontrol::Process()
{
	if(visible)
	{
		if(PointInBox(mouseX, mouseY, GetPosX(), GetPosY(), GetPatternWidth(), GetPatternHeight()))
		{
						
			if(GetPatternCount() > 1)
				SetCurrentPattern(1);

			if(mouse[0])
			{
				_pressed = true;
				
				if(GetPatternCount() > 1)
					SetCurrentPattern(2);

				if(_caption)
				{
					_caption_dx = 1;
					_caption_dy = 1;
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
					if(IsClose()) CloseAction();
				}

			}
		}
		else
		{
			if(GetCurrentPattern() != 0) SetCurrentPattern(0);
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
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if((*it))
			{
				(*it)->ProcessAll();
				if((*it)->IWannaDie) DeleteControl(*it); // this totally sux, i expect memory leaks
				// DO NOT WANT MEMORY LEAKS
			}
		}
}





