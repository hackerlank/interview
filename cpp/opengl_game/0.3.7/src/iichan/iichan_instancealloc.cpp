#include "stdafx.h"
#include "iichan_instancealloc.h"

extern unsigned int current_time;

void IICHANInstanceAllocator::StartDialog(DIALOG* dialog)
{
	if(!dialog)
		return;

	dialog->SetSpriteAllocator(GetSpriteAllocator());
	dialog->SetScreenTextAllocator(GetScreenTextAllocator());
	// showing first message
	dialog->ShowMessage(0);
	SetCurrentDialog(dialog);
	_prev_message_time = current_time;

	// disabling keyboard, AI and hiding hud
	SetHUDEnabled(false);
	SetAI(false);
	SetPause(true);
	SetKeyboardEnabled(false);
}

// Some useful macro's to make more readable code
// --------------------------------------------------------------------------
#define DIALOG_EXIST GetCurrentDialog()
#define DIALOG_NOT_FINISHED !GetCurrentDialog()->GetDone()
#define NOT_LAST_MESSAGE GetCurrentDialog()->GetCurrentMessageId() < \
	GetCurrentDialog()->GetMessageCount() - 1
#define MESSAGE_EXPIRED now - _prev_message_time >= GetCurrentDialog()->GetMessageById( \
					GetCurrentDialog()->GetCurrentMessageId())->time * 1000
#define SHOW_NEXT_MESSAGE GetCurrentDialog()->ShowMessage( \
	GetCurrentDialog()->GetCurrentMessageId() + 1)
// --------------------------------------------------------------------------

void IICHANInstanceAllocator::Process()
{
	if(DIALOG_EXIST)
	{
		if(DIALOG_NOT_FINISHED)
		{
			int now = current_time;
			if(NOT_LAST_MESSAGE)
			{
				if(MESSAGE_EXPIRED)
				{
					SHOW_NEXT_MESSAGE;
					_prev_message_time = now;
				}
			}
			else
			{
				if(MESSAGE_EXPIRED)
				{
					GetCurrentDialog()->ClearCurrentMessage();
					GetCurrentDialog()->SetDone(true);

					// enabling keyboard, AI and hud
					SetHUDEnabled(true);
					SetAI(true);
					SetPause(false);
					SetKeyboardEnabled(true);
				}
			}
		}
	}
}
