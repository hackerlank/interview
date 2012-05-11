#include "stdafx.h"

#include "gui_misc.h"
#include "gui.h"

#include "texture.h"
#include "font.h"
#include "gui_scrmsgalloc.h"

#include "gui_msgalloc.h"

#include "gui_basecontrol.h"
#include "gui_vidget.h"
#include "gui_control.h"



#include "gui_ctrlalloc.h"

#include "misc.h"

#include "iichan.h"



// EXTERNS
// --------------------------------------------------------------------------
extern unsigned int current_state;
//extern unsigned int difficulty_pick;
//extern unsigned int size_pick;
//extern unsigned int race_pick;
//extern unsigned int opponents_pick;
//extern char* player_name;
//extern char* home_star_name;
extern IICHAN* iichan;

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

void GUIMessageAllocator::Clear()
{
	messages.clear();
	SetMessageCount(0);
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

			GUIControl* ctrl = (GUIControl*)gui->GetControls()->GetControlByGID(
				msg->from);

			if(msg->solved)
				continue;

			switch(msg->msg)
			{
			case GUI_MOUSE_LEFT_CLICK:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": left click")+1];
					sprintf(message, "%s: left click\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif

					switch(msg->sgn.sgn)
					{
					case GUI_SGN_OPEN_GROUP:
						{
							//vector<GUIBaseControl*>* ctrls =
							//	gui->GetControls()->GetItems();
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
					//case GUI_SGN_SET_VAR:
					//	{
					//		switch(msg->sgn.sgn_param_1)
					//		{
					//		//case GUI_SET_DIFF:
					//		//	{
					//		//		difficulty_pick = msg->sgn.sgn_param_2;
					//		//		break;
					//		//	}
					//		//case GUI_SET_GALAXY_SIZE:
					//		//	{
					//		//		size_pick = msg->sgn.sgn_param_2;
					//		//		break;
					//		//	}
					//		//case GUI_SET_RACE:
					//		//	{
					//		//		race_pick = msg->sgn.sgn_param_2;
					//		//		break;
					//		//	}
					//		//case GUI_SET_OPP_COUNT:
					//		//	{
					//		//		opponents_pick = msg->sgn.sgn_param_2;
					//		//		break;
					//		//	}
					//		default:
					//			{
					//				LogToFile(DEFAULT_GUI_LOG_NAME, "Unknown signal parameter");
					//				break;
					//			}
					//		}
					//		break;
					//	}

					case GUI_SGN_EXIT:
						{
							current_state = GAME_EXITING;
							break;
						}
					case GUI_SGN_START_GAME:
						{
							char* message = new char[strlen("Game started")+1];
							sprintf(message, "%s\0", "Game started");
							GetGUI()->LogOnScreen(message);
							current_state = GAME_RUNNING;
							vector<GUIBaseControl*>* items = GetGUI()->GetControls()->GetItems();
							for(vector<GUIBaseControl*>::iterator it = items->begin();
								it != items->end();
								it++)
							{
								(*it)->SetVisible(false);
							}
							iichan = new IICHAN();
							iichan->Init();
							break;
						}
					//case GUI_SGN_LOAD_LEVEL:
					//	{
					//		iichan->LoadLevel(msg->sgn.sgn_param_1);
					//		break;
					//	}


					default:
						{
							LogToFile(DEFAULT_GUI_LOG_NAME, "Unknown signal");
							break;
						}
					}

					break;
				}
			case GUI_MOUSE_RIGHT_CLICK:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": right click")+1];
					sprintf(message, "%s: right click\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_MOVE:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": mouse move")+1];
					sprintf(message, "%s: mouse move\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_DOWN:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": mouse down")+1];
					sprintf(message, "%s: mouse down\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					GetGUI()->GetControls()->SetFocusedControl(ctrl->GetId());
					break;
				}
			case GUI_MOUSE_UP:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": mouse up")+1];
					sprintf(message, "%s: mouse up\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_ENTER:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": mouse enter")+1];
					sprintf(message, "%s: mouse enter\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_MOUSE_LEAVE:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": mouse leave")+1];
					sprintf(message, "%s: mouse leave\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_KEY_DOWN:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": key down")+1];
					sprintf(message, "%s: key down\0", ctrl->GetName().c_str());
					GetGUI()->LogOnScreen(message);
#endif
					break;
				}
			case GUI_KEY_UP:
				{
#ifdef _DEBUG_GUI
					char* message = new char[ctrl->GetName().length()
						+ strlen(": key up")+1];
					sprintf(message, "%s: key up\0", ctrl->GetName().c_str());
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