#include "stdafx.h"

#include "gui_misc.h"
#include "gui.h"


#include "gui_basecontrol.h"

#include "gui_msgalloc.h"

#include "gui_ctrlalloc.h"

#include "misc.h"




// GUIControlAllocator Implementation
// --------------------------------------------------------------------------
GUIControlAllocator::GUIControlAllocator(GUIMessageAllocator* msg_alloc)
{
		control_count = 0;
		controls.resize(0);
		messages = msg_alloc;
		SetFocusedControl(0);
}


void GUIControlAllocator::Add(GUIBaseControl* item)
{
		item->SetMessages(messages);
		controls.push_back(item);
		SetControlCount(GetControlCount() + 1);
		SetFocusedControl(item->GetId());

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
			GUIBaseControl* ctrl = (*it);
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
			GUIBaseControl* ctrl = (*it);
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

GID GUIControlAllocator::GetFocusedControl()
{
	return focused_control;
}

void GUIControlAllocator::SetFocusedControl(GID new_focused_control)
{
	GUIBaseControl* prev = GetControlByGID(GetFocusedControl());
	if(prev)
		prev->SetFocus(false);
	GUIBaseControl* ctrl = GetControlByGID(new_focused_control);
	if(ctrl)
	{
		focused_control = ctrl->GetId();
		ctrl->SetFocus(true);
	}
}


GUIBaseControl* GUIControlAllocator::GetControlByName(const char* name)
{
	if(GetControlCount() > 0)
	{
		if(name)
			for(std::vector<GUIBaseControl*>::iterator it = controls.begin();
				it != controls.end();
				it++)
				if(strcmp((*it)->GetName().c_str(), name) == 0)
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

void GUIControlAllocator::Clear()
{
	for(vector<GUIBaseControl*>::iterator it = controls.begin();
		it != controls.end();
		it++)
	{
		GUIBaseControl* ctrl = (*it);

		ctrl->SetAllocator(NULL);
		ctrl->SetMessages(NULL);
		ctrl->SetName("");
		//delete ctrl;
		ctrl->~GUIBaseControl();
		ctrl = NULL;
	}
	controls.clear();
	SetControlCount(0);
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