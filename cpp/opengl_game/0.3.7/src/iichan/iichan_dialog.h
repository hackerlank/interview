#ifndef __IICHAN_DIALOG_H_
#define __IICHAN_DIALOG_H_
#include "iichan_spritealloc.h"
#include "iichan_scrtextalloc.h"
#include "../draw/draw.h"
#include "../misc.h"
#include "../config.h"

#define DIALOG_DX 25
#define DIALOG_DY 25
#define DIALOG_TEXT_DY 40

extern config cfg;
extern unsigned int current_time;


struct DIALOG_MESSAGE {
	int id;
	string name;
	string text;
	int time;
	int turn; // 0 - left, 1 - right
	string resource; // for images (avatars)
};

class DIALOG
{
private:
	SPRITE* avatar;
	int current_message_id;
	vector<DIALOG_MESSAGE> messages;
	int id;
	char* name;
	IICHANSpriteAllocator* SpriteAllocator;
	IICHANScreenTextAllocator* ScreenTextAllocator;
	int message_count;
	bool done;

public:
	SPRITE* GetAvatar() const { return avatar; }
	void SetAvatar(SPRITE* new_value) { avatar = new_value; }

	int GetCurrentMessageId() const { return current_message_id; }
	void SetCurrentMessageId(int new_value) { current_message_id = new_value; }

	void SetSpriteAllocator(IICHANSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	IICHANSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	void SetScreenTextAllocator(IICHANScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	IICHANScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	int GetId() const { return id; }
	void SetId(int new_value) { id = new_value; }

	int GetMessageCount() const { return message_count; }
	void SetMessageCount(int new_value) { message_count = new_value; }

	bool GetDone() const { return done; }
	void SetDone(bool new_value) { done = new_value; }

	char* GetName() const { return name; }
	void SetName(char* new_value) { name = new_value; }

	void AddMessage(DIALOG_MESSAGE message)
	{
		static int _count = 0;
		message.id = _count;
		messages.push_back(message);
		_count++;
		SetMessageCount(_count);
	}

	void ClearCurrentMessage()
	{
		if(GetAvatar())
		{
			GetSpriteAllocator()->DeleteSprite(
				GetSpriteAllocator()->GetSprite(GetAvatar()->GetId()));
			SetAvatar(NULL);
		}
	}

	void ShowMessage(int id)
	{
		static const font_t* font = FontByName("default");

		DIALOG_MESSAGE* msg = NULL;
		msg = GetMessageById(id);

		if(msg)
		{
			RESOURCE* res = ResourceByName((char*)msg->resource.c_str());
			if(res)
			{
				ClearCurrentMessage();


				SPRITE* av = new SPRITE();

				av->LoadFromResource(res);
				av->SetX(DIALOG_DX);
				av->SetY(DIALOG_DY);
				av->SetFixed(true);
				av->SetCollidable(false);
				av->SetZ(TOP_Z);
				if(msg->turn % 2 != 0)
				{
					av->SetX(cfg.scr_width - DIALOG_DX - av->GetPatternWidth());
				}
				SetAvatar(av);
				assert(GetSpriteAllocator() != NULL);
				GetSpriteAllocator()->AddSprite(avatar);

				SCREEN_TEXT* text = new SCREEN_TEXT();
				text->time_remaining = msg->time-1;
				text->text = msg->text;
				text->x = cfg.scr_width / 2 - text->text.length()*4;
				text->y = DIALOG_TEXT_DY;
				GetScreenTextAllocator()->AddItem(text);

				SetCurrentMessageId(id);
			}
			else
				WriteToLog("Dialog::ShowMessage(): resource not found");
		}
	}

	DIALOG_MESSAGE* GetMessageById(int id)
	{
		for(vector<DIALOG_MESSAGE>::iterator it = messages.begin();
			it != messages.end();
			it++)
		{
			DIALOG_MESSAGE* msg = &(*it);
			if(msg->id == id)
				return msg;
		}
		return NULL;
	}

	DIALOG()
	{
		id = 0;
		SetAvatar(NULL);
		messages.resize(0);
		SetName(NULL);
		SetSpriteAllocator(NULL);
		SetScreenTextAllocator(NULL);
		SetMessageCount(0);
		SetDone(false);
	}
};

bool LoadDialog(const char* file_name);

DIALOG* DialogByName(char* name);

#endif