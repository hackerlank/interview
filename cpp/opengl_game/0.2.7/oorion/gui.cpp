#include "stdafx.h"
#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"
#include "misc.h"
#include "game.h"
#include "data.h"
#include "gui.h"
#include "scene.h"



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

GUIScreenMessageAllocator* GUI::GetScreenMessages()
{
	return ScreenMessages;
}

GUIMessageAllocator* GUI::GetMessages()
{
	return Messages;
}
// --------------------------------------------------------------------------



// GUIControlAllocator Implementation
// --------------------------------------------------------------------------
GUIControlAllocator::GUIControlAllocator(GUIMessageAllocator* msg_alloc)
{
		control_count = 0;
		controls.resize(0);
		messages = msg_alloc;
}

void GUIControlAllocator::Add(GUIBaseControl* item)
{
	if(item)
	{
		item->SetMessages(messages);
		controls.push_back(item);
		SetControlCount(GetControlCount() + 1);
	}
}

void GUIControlAllocator::Delete(GUIBaseControl* item)
{
	if(item)
		delete item;
}

void GUIControlAllocator::Draw()
{
	if(GetControlCount() > 0)
	{
		for(vector<GUIBaseControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
		{
			GUIBaseControl* ctrl = *it;
			ctrl->Draw();
		}
	}
}

void GUIControlAllocator::Process()
{
	if(GetControlCount() > 0)
	{
		for(vector<GUIBaseControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
		{
			GUIBaseControl* ctrl = *it;
			if(ctrl)
				ctrl->Process();
		}
		for(UINT i = 0; i < GetControlCount(); i++)
		{
			if(controls[i]->IsDead())
			{
				Delete(controls[i]);
				controls.erase(controls.begin()+i);
				SetControlCount(GetControlCount()-1);
			}
		}
	}
}

GUIBaseControl* GUIControlAllocator::GetControlByGID(GID guid)
{
	if(GetControlCount() > 0)
	{
		for(std::vector<GUIBaseControl*>::iterator it = controls.begin();
			it != controls.end();
			it++)
			if((*it)->GetId() == guid)
				return (*it);
		return NULL;
	}
	else
		return NULL;
}

GUIBaseControl* GUIControlAllocator::GetControlByName(LPSTR name)
{
	if(GetControlCount() > 0)
	{
		if(name)
			for(std::vector<GUIBaseControl*>::iterator it = controls.begin();
				it != controls.end();
				it++)
				if(strcmp((*it)->GetName(), name) == 0)
					return (*it);
			return NULL;
	}
	else
		return NULL;
}

vector<GUIBaseControl*>* GUIControlAllocator::GetItems()
{
	return &controls;
}

void GUIControlAllocator::ViewControl(char* name)
{
		//for(vector<GUIBaseControl*>::iterator it = controls.begin();
		//	it != controls.end();
		//	it++)
		//{
		//	(*it)->SetVisible(false);
		//	if(strcmp(name, (*it)->GetName()) == 0)
		//		(*it)->SetVisible(true);
		//}
}

void GUIControlAllocator::SetControlCount(UINT new_control_count)
{
	control_count = new_control_count;
}

UINT GUIControlAllocator::GetControlCount()
{
	return control_count;
}
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
		
void GUIScreenMessageAllocator::Draw()
{
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
				FontByName(LOG_SCREEN_FONT), 
				cfg.scr_width / 2 - LOG_SCREEN_MARGIN,
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



// GUIMessageAllocator Implementation
// --------------------------------------------------------------------------
GUIMessageAllocator::GUIMessageAllocator()
{
	messages.resize(0);
	message_count = 0;
}

void GUIMessageAllocator::SetGUI(GUI* new_gui)
{
	gui = new_gui;
}

GUI* GUIMessageAllocator::GetGUI()
{
	return gui;
}


void GUIMessageAllocator::Add(GUIMessage item)
{
		messages.push_back(item);
		message_count++;
}

void GUIMessageAllocator::Process()
{
	if(GetMessageCount() > 0)
	{
		for(vector<GUIMessage>::iterator it = messages.begin();
			it != messages.end();
			it++)
		{
			GUIMessage* msg = &(*it);

			GUIControl* ctrl = (GUIControl*)msg->from;

			if(msg->solved)
				continue;

			switch(msg->msg)
			{
			case GUI_MOUSE_LEFT_CLICK:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": left click")+1];
					sprintf(message, "%s: left click\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif

					switch(msg->sgn.sgn)
					{
					case GUI_SGN_OPEN_GROUP:
						{
							vector<GUIBaseControl*>* ctrls =
								gui->GetControls()->GetItems();

							for(vector<GUIBaseControl*>::iterator c =
								ctrls->begin(); c != ctrls->end(); c++)
							{
								GUIBaseControl* slct = *c;

								if(slct->GetGroup() == ctrl->GetGroup())
									slct->SetVisible(false);
								else if(slct->GetGroup() == msg->sgn.sgn_param_1)
									slct->SetVisible(true);
							}
							break;
						}														

					case GUI_SGN_EXIT:
						{
							current_state = GAME_EXITING;
							break;
						}

					default: break;
					}

					break;
				}
			case GUI_MOUSE_RIGHT_CLICK:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": right click")+1];
					sprintf(message, "%s: right click\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_MOVE:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": mouse move")+1];
					sprintf(message, "%s: mouse move\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_DOWN:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": mouse down")+1];
					sprintf(message, "%s: mouse down\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_UP:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": mouse up")+1];
					sprintf(message, "%s: mouse up\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_ENTER:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": mouse enter")+1];
					sprintf(message, "%s: mouse enter\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_LEAVE:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": mouse leave")+1];
					sprintf(message, "%s: mouse leave\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_KEY_DOWN:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": key down")+1];
					sprintf(message, "%s: key down\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_KEY_UP:
				{
#ifdef _DEBUG_GUI
					char* message = new char[strlen(ctrl->GetName())
						+ strlen(": key up")+1];
					sprintf(message, "%s: key up\0", ctrl->GetName());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			default: break;
			}

			msg->solved = true;
		}

		messages.clear();
		SetMessageCount(0);

	}
}

UINT GUIMessageAllocator::GetMessageCount()
{
	return message_count;
}

void GUIMessageAllocator::SetMessageCount(UINT new_message_count)
{
	message_count = new_message_count;
}
// --------------------------------------------------------------------------



// GUIBaseControl Implementation
// --------------------------------------------------------------------------
char* GUIBaseControl::GetName() { return name; }
GUIBaseControl* GUIBaseControl::GetParent() { return parent; }
GID GUIBaseControl::GetId() { return id; }
GUIMessageAllocator* GUIBaseControl::GetMessages() { return messages; }
GUIControlAllocator* GUIBaseControl::GetAllocator() { return allocator; }
bool GUIBaseControl::IsVisible() { return visible; }
UINT GUIBaseControl::GetGroup() { return group; }

void GUIBaseControl::SetName(char *new_name) { name = new_name; }
void GUIBaseControl::SetParent(GUIBaseControl *new_parent)
{ parent = new_parent; }
void GUIBaseControl::SetParent(char* parent_name)
{
	parent = GetAllocator()->GetControlByName(parent_name);
	SetMessages(GetParent()->GetMessages());
}
void GUIBaseControl::SetId(GID new_id) { id = new_id; }
void GUIBaseControl::SetMessages(GUIMessageAllocator* new_messages)
{ messages = new_messages; }
void GUIBaseControl::SetAllocator(GUIControlAllocator* new_allocator)
{ allocator = new_allocator; }
void GUIBaseControl::SetVisible(bool new_visible)
{ visible = new_visible; }
void GUIBaseControl::SetGroup(UINT new_group)
{ group = new_group; }

GUIBaseControl::GUIBaseControl()
{
	static unsigned int __count = 0;
	__count++;

	SetId(__count);
	SetParent((GUIBaseControl*)NULL);
	SetMessages(NULL);
}

GUIBaseControl::GUIBaseControl(char *name)
{
	GUIBaseControl();
	SetName((char*)name);
}

bool GUIBaseControl::IsDead() { return dead; }
void GUIBaseControl::SetDead(bool new_dead) { dead = new_dead; }

void GUIBaseControl::Draw()
{
}

void GUIBaseControl::Process()
{
}
// --------------------------------------------------------------------------



// GUIVidget Implementation
// --------------------------------------------------------------------------
int GUIVidget::GetX() { return x; }
int GUIVidget::GetY() { return y; }
UINT GUIVidget::GetWidth() { return width; }
UINT GUIVidget::GetHeight() { return height; }
texture_t* GUIVidget::GetTexture() { return texture; }
COLOR4 GUIVidget::GetColor() { return color; }
//bool GUIVidget::IsVisible() { return visible; }

void GUIVidget::SetX(int new_x) { x = new_x; }
void GUIVidget::SetY(int new_y) { y = new_y; }
void GUIVidget::SetWidth(UINT new_width) { width = new_width; }
void GUIVidget::SetHeight(UINT new_height) { height = new_height; }
void GUIVidget::SetTexture(texture_t* new_texture) { texture = new_texture; }
void GUIVidget::SetColor(COLOR4 new_color) { color = new_color; }
//void GUIVidget::SetVisible(bool new_visible) { visible = new_visible; }

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

void GUIVidget::Process()
{
	__super::Process();
}

GUIVidget::GUIVidget()
{
	SetColor(DEFAULT_CONTROL_COLOR);
	SetX(0);
	SetY(0);
	SetWidth(0);
	SetHeight(0);
	SetColor(DEFAULT_CONTROL_COLOR);
	SetVisible(false);
}

GUIVidget::GUIVidget(char* name)
{
	GUIVidget();
	SetName(name);
}
// --------------------------------------------------------------------------



// GUIControl Implementation
// --------------------------------------------------------------------------
UINT GUIControl::GetPatternCount() { return pattern_count; }
UINT GUIControl::GetCurrentPattern() { return current_pattern; }
bool GUIControl::IsFocused() { return focused; }
bool GUIControl::IsEnabled() { return enabled; }
int GUIControl::GetLastCursorPos() { return last_cursor_pos; }
int GUIControl::GetLastPressState() { return last_press_state; }
int GUIControl::GetLastSentMsg() { return last_sent_msg; }
char* GUIControl::GetCaption() { return caption; }
font_t* GUIControl::GetCaptionFont() { return caption_font; }
COLOR4 GUIControl::GetCaptionColor() { return caption_color; }
int GUIControl::GetCaptionX() { return caption_x; }
int GUIControl::GetCaptionY() { return caption_y; }
//UINT GUIControl::GetSgn() { return sgn; }
//UINT GUIControl::GetSgnParam1() { return sgn_param_1; }
//UINT GUIControl::GetSgnParam2() { return sgn_param_2; }

void GUIControl::SetPatternCount(UINT new_pattern_count)
{ pattern_count = new_pattern_count; }
void GUIControl::SetCurrentPattern(UINT new_current_pattern)
{ current_pattern = new_current_pattern; }
void GUIControl::SetFocus(bool new_focused)
{ focused = new_focused; }
void GUIControl::SetEnabled(bool new_enabled)
{ enabled = new_enabled; }
void GUIControl::SetLastCursorPos(int new_last_cursor_pos)
{ last_cursor_pos = new_last_cursor_pos; }
void GUIControl::SetLastPressState(int new_last_press_state)
{ last_press_state = new_last_press_state; }
void GUIControl::SetLastSentMsg(int new_last_sent_msg)
{ last_sent_msg = new_last_sent_msg; }
void GUIControl::SetCaption(char *new_caption)
{ caption = new_caption; }
void GUIControl::SetCaptionFont(font_t* new_caption_font)
{ caption_font = new_caption_font; }
void GUIControl::SetCaptionColor(COLOR4 new_caption_color)
{ caption_color = new_caption_color; }
void GUIControl::SetCaptionX(int new_caption_x)
{ caption_x = new_caption_x; }
void GUIControl::SetCaptionY(int new_caption_y)
{ caption_y = new_caption_y; }
//void GUIControl::SetSgn(UINT new_sgn)
//{ sgn = new_sgn; }
//void GUIControl::SetSgnParam1(UINT new_sgn_param_1)
//{ sgn_param_1 = new_sgn_param_1; }
//void GUIControl::SetSgnParam2(UINT new_sgn_param_2)
//{ sgn_param_2 = new_sgn_param_2; }
void GUIControl::AddSgn(UINT sgn, UINT param_1, UINT param_2)
{
	GUISignal d;

	d.sgn = sgn;
	d.sgn_param_1 = param_1;
	d.sgn_param_2 = param_2;

	sgns.push_back(d);
}

void GUIControl::Draw()
{
	//__super::Draw();
	if(IsVisible())
	{
		if(GetTexture())
		{	
			COLOR4 c = GetColor();
			glColor4ub(c.r, c.g, c.b, c.a);

			if(GetCurrentPattern() == 0)
			{
				DrawTexture(GetX(), GetY(),
					0, 0,
					GetWidth(), GetHeight(),
					GetTexture(),
					false, false, 100, 100);
			}
			else
			{
					DrawTexture(GetX(), GetY(),
						GetCurrentPattern() * GetWidth(),
						0,
						GetCurrentPattern() * GetWidth() + GetWidth(),
						GetHeight(),
						GetTexture(),
						false, false, 100, 100);
			}
		}

		if(GetCaption() && GetCaptionFont())
		{
			COLOR4 c = GetCaptionColor();
			glColor4ub(c.r, c.g, c.b, c.a);

			PrintText(GetCaption(),
				GetCaptionFont(),
				GetX() + GetCaptionX(),
				GetY() + GetCaptionY());
		}
	}
}

void GUIControl::Process()
{
	__super::Process();

	if(IsVisible())
	{
		if(PointInBox(mouseX, mouseY, GetX(), GetY(), GetWidth(), GetHeight()))
		{

			int cursor = GetLastCursorPos();
			SetLastCursorPos(1); // mouse is in

			// here
			// 0 - init
			// 1 - mouse was IN control
			// 2 - mouse was OUT control

			if(cursor == 0 || cursor == 2)
			{
				OnMouseEnter();
			}
			else
			{
				OnMouseMove();
			}

			if(mouse[0] || mouse[1])
			{
				if(GetLastPressState() == 0)
				{
					OnMouseDown();

					if(mouse[0])
						SetLastPressState(1);
					else if(mouse[1])
						SetLastPressState(2);
				}
			}
			else
			{
				int press_state = GetLastPressState();
				SetLastPressState(0);

				// here
				// 0 - init
				// 1 - left mouse down
				// 2 - right mouse down
				// 3 - all up

				if(press_state == 1)
				{
					OnLeftClick();
					OnMouseUp();
				}
				else if(press_state == 2)
				{
					OnRightClick();
					OnMouseUp();
				}

			}	
		}
		else
		{
			int cursor = GetLastCursorPos();
			SetLastCursorPos(2); // mouse is out

			if(cursor == 1)
			{
				OnMouseLeave();
			}
		}
	}
}



void GUIControl::SendMessage(GUIMessage msg)
{
	messages->Add(msg);
	SetLastSentMsg(msg.msg);
}

void GUIControl::OnMouseMove()
{
	if(sgns.size() > 0)
	{
		if(GetLastSentMsg() != GUI_MOUSE_MOVE)
		{
			for(vector<GUISignal>::iterator it = sgns.begin();
				it != sgns.end();
				it++)
			{
				GUIMessage msg;

				msg.sgn = *it;
				msg.msg = GUI_MOUSE_MOVE;
				msg.solved = false;
				msg.from = this;

				SendMessage(msg);
			}
		}
	}
}

void GUIControl::OnMouseEnter()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_ENTER;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}

	if(GetPatternCount() > 0)
		SetCurrentPattern(1);
}

void GUIControl::OnMouseLeave()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_LEAVE;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}

	if(GetPatternCount() > 0)
		SetCurrentPattern(0);
}

void GUIControl::OnMouseDown()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_DOWN;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}

	if(GetPatternCount() >= 2)
		SetCurrentPattern(2);
}

void GUIControl::OnMouseUp()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_UP;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}

	if(GetPatternCount() >= 2)
		SetCurrentPattern(1);
}

void GUIControl::OnLeftClick()
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
			msg.from = this;

			SendMessage(msg);
		}
	}
}

void GUIControl::OnRightClick()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_MOUSE_RIGHT_CLICK;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}
}

void GUIControl::OnKeyUp()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_KEY_UP;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}
}

void GUIControl::OnKeyDown()
{
	if(sgns.size() > 0)
	{

		for(vector<GUISignal>::iterator it = sgns.begin();
			it != sgns.end();
			it++)
		{
			GUIMessage msg;

			msg.sgn = *it;
			msg.msg = GUI_KEY_DOWN;
			msg.solved = false;
			msg.from = this;

			SendMessage(msg);
		}
	}
}

GUIControl::GUIControl()
{
	SetPatternCount(0);
	SetCurrentPattern(0);
	SetFocus(true);
	SetEnabled(true);
	SetDead(false);
	SetLastCursorPos(0);
	SetLastPressState(0);
	SetLastSentMsg(0);
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
	SetCaptionX(0);
	SetCaptionY(0);
	sgns.resize(0);
}

GUIControl::GUIControl(char* name)
{
	GUIControl();
	SetName(name);

}
// --------------------------------------------------------------------------



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
			msg.from = this;

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
				msg.from = this;

				SendMessage(msg);
			}
		}
	}
}

GUIButton::GUIButton()
{
	SetCaption(NULL);
	SetCaptionFont(NULL);
	SetCaptionColor(DEFAULT_TEXT_COLOR);
}

GUIButton::GUIButton(char* name)
{
	GUIButton();
	SetName(name);
}
// --------------------------------------------------------------------------