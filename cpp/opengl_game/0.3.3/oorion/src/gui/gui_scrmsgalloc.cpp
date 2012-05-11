#include "stdafx.h"

#include "../config.h"
#include "../draw/font.h"
#include "../draw/draw.h"

#include "gui_misc.h"

#include "gui_scrmsgalloc.h"

// EXTERNS
// --------------------------------------------------------------------------
config cfg;
// --------------------------------------------------------------------------

// GUIScreenMessageAllocator Implementation
// --------------------------------------------------------------------------
GUIScreenMessageAllocator::GUIScreenMessageAllocator()
{
	messages.resize(0);
	message_count = 0;
}

void GUIScreenMessageAllocator::Add(GUIScreenMessage item)
{
		messages.push_back(item);
		message_count++;
}

void GUIScreenMessageAllocator::Delete(GUIScreenMessage* item)
{
	if(item)
		if(item->message)
			delete[] item->message;
}

void GUIScreenMessageAllocator::Clear()
{
	messages.clear();
	SetMessageCount(0);
}
		
void GUIScreenMessageAllocator::Draw()
{
	static const font_t* font = FontByName(LOG_SCREEN_FONT);
	static const int left = cfg.scr_width / 2 - LOG_SCREEN_MARGIN;
	if(GetMessageCount() > 0)
	{
		UINT Count = 0;
		for(vector<GUIScreenMessage>::iterator it = messages.begin();
			it != messages.end();
			it++)
		{
			Count++;
			GUIScreenMessage msg = *it;
			COLOR4 c = DEFAULT_SCREEN_MESSAGE_COLOR;
			glColor4ub(c.r, c.g, c.b, c.a);
			PrintText(msg.message,
				font, 
				left,
				LOG_SCREEN_Y + Count * LOG_SCREEN_DY);
		}
	}
}

void GUIScreenMessageAllocator::Process()
{
	if(GetMessageCount() > 0)
	{
		static int startup_time = GetTickCount();

		tick2 = GetTickCount() - startup_time;

		if(tick2 - tick1 >= 1000)
		{
			tick1 = tick2;
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
}

void GUIScreenMessageAllocator::SetMessageCount(UINT new_message_count)
{
	message_count = new_message_count;
}

UINT GUIScreenMessageAllocator::GetMessageCount()
{
	return message_count;
}
// --------------------------------------------------------------------------