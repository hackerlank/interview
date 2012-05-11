#include "stdafx.h"
#include "gui_misc.h"
#include "gui.h"
#include "gui_msgalloc.h"
#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "../misc.h"


// GUIBaseControl Implementation
// --------------------------------------------------------------------------
string GUIBaseControl::GetName() { return name; }
GUIBaseControl* GUIBaseControl::GetParent() { return parent; }
GID GUIBaseControl::GetId() { return id; }
GUIMessageAllocator* GUIBaseControl::GetMessages() { return messages; }
GUIControlAllocator* GUIBaseControl::GetAllocator() { return allocator; }
bool GUIBaseControl::IsVisible() { return visible; }
bool GUIBaseControl::IsFocused() { return focused; }
UINT GUIBaseControl::GetGroup() { return group; }

void GUIBaseControl::SetName(string new_name)
{
	name = new_name;
}

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
void GUIBaseControl::SetFocus(bool new_focused)
{ focused = new_focused; }
void GUIBaseControl::SetGroup(UINT new_group)
{ group = new_group; }

void GUIBaseControl::Init()
{
	static unsigned int __count = 0;
	__count++;
	SetId(__count);
	parent = NULL;
	messages = NULL;
	name = "";
}

GUIBaseControl::GUIBaseControl()
{
	Init();
}

GUIBaseControl::GUIBaseControl(string name)
{
	Init();
	SetName(name);
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