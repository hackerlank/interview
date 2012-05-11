typedef int GID;

extern HDC hDC;
extern font_t fonts[256];
extern texture_t textures[256];
extern config cfg;
extern unsigned int current_state;
extern int difficulty_pick;
extern int size_pick;
extern int race_pick;
extern int opponents_pick;

struct GUIMessage
{
public:
	bool solved;
	GID from;
	int a1;
	int a2;
};

class GUIMessageAllocator
{
public:
	int messagesCount;
	std::vector<GUIMessage> messages;

	void Add(GUIMessage item)
	{
		messages.push_back(item);
		messagesCount++;
	}
};


class GUIcontrol
{
private:
	LPSTR parent_name;
	LPSTR next_name;
	GUIcontrol* parent;
	GUIcontrol* next;
	int _childCount;

	bool _mouse_left;
	bool _mouse_right;
	int _mouse_x;
	int _mouse_y;
	int _pos_x;
	int _pos_y;
	bool _close;
	LPSTR _caption;
	LPSTR _text;
	int _caption_dx;
	int _caption_dy;
	bool _caption_center;
	bool _text_center;
	bool _pressed;
	int _pos_dx;
	int _pos_dy;
	

protected:

	font_t* captionFont;
	font_t* textFont;
	int currentPattern;
	int patternCount;
	int patternWidth;
	int patternHeight;
	BYTE color[4];
	BYTE textColor[4];
	BYTE captionColor[4];
	int text_x;
	int text_y;

	int caption_x;
	int caption_y;
	

public:
	std::vector<GUIcontrol*> children;	
	LPSTR name;
	bool IWannaDie;
	bool visible;
	texture_t* tex;
	ACTION action;

	GID gid;
	GUIMessageAllocator* guima;

	GUIcontrol(LPSTR ctrl_name)
	{
		static int __create_count;
		__create_count++;

		gid = __create_count;


		name = NULL;
		parent_name = NULL;
		next_name = NULL;
		guima = NULL;
		children.resize(0);
		name = ctrl_name;
		parent = NULL;
		next = NULL;
		_childCount = 0;
		_mouse_left = false;
		_mouse_right = true;
		_mouse_x = 0;
		_mouse_y = 0;
		_pos_x = 0;
		_pos_y = 0;
		_close = false;
		textColor[0] = 255;
		textColor[1] = 255;
		textColor[2] = 255;
		textColor[3] = 255;
		captionColor[0] = 255;
		captionColor[1] = 255;
		captionColor[2] = 255;
		captionColor[3] = 255;
		tex = NULL;
		captionFont = NULL;
		textFont = NULL;
		_caption = NULL;
		_text = NULL;
		text_x = 5;
		text_y = 5;
		_caption_dx = 0;
		_caption_dy = 0;
		_pos_dx = 0;
		_pos_dy = 0;
		caption_x = 5;
		caption_y = 5;
		_caption_center = true;
		_text_center = true;
		_pressed = false;
		IWannaDie = false;
		visible = false;
		action = actNo;

#ifdef _DEBUG_
		char guid[10];
		_itoa(gid, guid, 10);
		LogToFile("Created GUIcontrol: GID=" + string(guid) + "; name=" + string(name));
#endif


	}

	GID GetGID() { return gid; }
	LPSTR GetName() { return name; }

	void SetAction(ACTION new_action)
	{
		action = new_action;
	}

	ACTION GetAction() { return action; }

	void SetTexture(texture_t* new_tex)
	{
		tex = new_tex;
	}

	texture_t* GetTexture() const { return tex; }

	void SetCaptionFont(font_t* new_font)
	{
		captionFont = new_font;
	}

	font_t* GetCaptionFont() const { return captionFont; }

	void SetTextFont(font_t* new_font)
	{
		textFont = new_font;
	}

	font_t* GetTextFont() const { return textFont; }

	void SetText(LPSTR new_text)
	{
		if(new_text)
			//strcpy(_text, new_text);
			_text = new_text;
	}

	LPSTR GetText() const { return _text; }

	void SetCaption(LPSTR new_caption)
	{
		if(new_caption)
			//strcpy(_caption, new_caption);
			_caption = new_caption;
	}

	LPSTR GetCaption() const { return _caption; }

	void SetTextColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
	{
		textColor[0] = clrRed;
		textColor[1] = clrGreen;
		textColor[2] = clrBlue;
		textColor[3] = clrAlpha;
	}

	void SetCaptionColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
	{
		captionColor[0] = clrRed;
		captionColor[1] = clrGreen;
		captionColor[2] = clrBlue;
		captionColor[3] = clrAlpha;
	}

	void SetTextPos(const int new_x, const int new_y)
	{
		text_x = new_x;
		text_y = new_y;
	}

	void SetCaptionPos(const int new_x, const int new_y)
	{
		caption_x = new_x;
		caption_y = new_y;
	}


	void SetPatternCount(const int count)
	{
		patternCount = count;
	}
	int GetPatternCount() const { return patternCount; }
	void SetCurrentPattern(const int index)
	{
		currentPattern = index;
	}
	int GetCurrentPattern() const { return currentPattern; }

	void SetPatternWidth(const int new_width)
	{
		patternWidth = new_width;
	}

	void SetPatternHeight(const int new_height)
	{
		patternHeight = new_height;
	}

	int GetPatternWidth() const { return patternWidth; }
	int GetPatternHeight() const { return patternHeight; }

	void SetPos(const int x, const int y)
	{
		_pos_x = x; _pos_y = y;

		if(parent)
		{
			_pos_x += parent->GetPosX();
			_pos_y += parent->GetPosY();
		}
	}

	void Centrify()
	{
		if(!parent)
		{
			if((GetPatternWidth() < cfg.scr_width) && (GetPatternHeight() < cfg.scr_height))
				SetPos(cfg.scr_width / 2 - GetPatternWidth() / 2, cfg.scr_height / 2 - GetPatternHeight() / 2);
		}
		else
		{
			if((GetPatternWidth() < GetParent()->GetPatternWidth()) && (GetPatternHeight() < GetParent()->GetPatternHeight()))
				SetPos(GetParent()->GetPatternWidth() / 2 - GetPatternWidth() / 2, GetParent()->GetPatternHeight() / 2 - GetPatternHeight() / 2);
		}
	}
			

	void SetColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
	{
		color[0] = clrRed;
		color[1] = clrGreen;
		color[2] = clrBlue;
		color[3] = clrAlpha;
	}

	int GetPosX() {	return _pos_x; }
	int GetPosY() {	return _pos_y; }

	void SetClose(int flag)
	{
		if(flag == 1)
			_close = true;
		else
			_close = false;
	}

	bool IsClose() { return _close; }

	void SetChildCount(int c) { _childCount = c; }
	int GetChildCount() const { return _childCount; }

	virtual void Draw()
	{
		int tx, ty;

		if(visible)
		{
			if(tex)
			{
				glColor4ubv(color);
				tx = _pos_x + _pos_dx;
				ty = _pos_y + _pos_dy;

				DrawTexture(tx, ty,
					GetCurrentPattern() * GetPatternWidth(),
					GetCurrentPattern() * GetPatternHeight(),
					GetCurrentPattern() * GetPatternWidth() + GetPatternWidth()-1,
					GetCurrentPattern() * GetPatternHeight() + GetPatternHeight()-1,
					tex, false, false, 100, 100);
				

			}

			if(textFont && _text)
			{
				glColor4ubv(textColor);

				int px,py;

				px = _pos_x + text_x;
				py = _pos_y + text_y;

				if(_text_center)
				{
					px += GetPatternWidth() / 4;
					//py += GetPatternHeight();
				}

				TextOut(string(_caption), captionFont, &hDC, px, py);
			}

			if(captionFont && _caption)
			{
				glColor4ubv(captionColor);

				int px,py;

				px = _pos_x + caption_x + _caption_dx;
				py = _pos_y + caption_y + _caption_dy;

				if(_caption_center)
				{
					px += GetPatternWidth() / 4;
					//py += GetPatternHeight();
				}

				TextOut(string(_caption), captionFont, &hDC, px, py);
			}
		}
	}

	virtual void DrawAll()
	{
		if(visible)
		{
			Draw();
			for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
			{
				if((*it))
				(*it)->DrawAll();
			}
		}

	}

	virtual void Process();
	virtual void ProcessAll();


	void SetParent(GUIcontrol* new_parent)
	{
		parent = new_parent;
	}

	void SetParentName(LPSTR new_name)
	{
		parent_name = new_name;
		//strcpy(parent_name, new_name);
	}

	GUIcontrol* GetParent()	{ return parent; }
	LPSTR GetParentName() { return parent_name; }

	void SetNextName(LPSTR new_name)
	{
		next_name = new_name;
		//if(new_name)
			//strcpy(next_name, new_name);
	}

	void SetNext(GUIcontrol* new_next)
	{
		next = new_next;
	}

	GUIcontrol* GetNext() { return next; }
	LPSTR GetNextName() { return next_name; }

	void AddChild(GUIcontrol* child)
	{
		child->guima = guima;
		children.push_back(child);
		//children[GetChildCount()]->parent = this;

		SetChildCount(GetChildCount() + 1);
		
	}

	void SetVisible(bool flag) 
	{
		visible = flag;

		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
		{
			(*it)->SetVisible(flag);
		}
	}
	bool GetVisible() const { return visible; }

	void KillSelf()
	{
		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
		{
			this->KillChild(*it);
		}
		IWannaDie = true;
				
	}

	void KillChild(GUIcontrol* child)
	{
		int i = -1;
		bool done = false;
		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
		{
			i++;
			if( (*it) == child )
			{

				for(std::vector<GUIcontrol*>::iterator it2 = (*it)->children.begin(); it2 != (*it)->children.end(); it2++)
				{
					(*it)->KillChild(*it2);
				}

				(*it)->children.clear();
				(*it)->SetChildCount(0);
				//children.erase(children.begin() + i);				
				this->children[i] = NULL;
				
				delete child;
				done = true;
				break;
			}

			if(done) break;
		}
	}


	void Close()
	{
		if(parent) parent->KillSelf();
	}

	void ExitProgram()
	{
		current_state = GAME_EXITING;
	}

	void OpenNext()
	{
		if(parent)
			parent->SetVisible(false);
		else
			SetVisible(false);

		if(GetNext())
			next->SetVisible(true);
	}

	void SendMessage(GUIMessage message)
	{
		if(guima) guima->Add(message);
	}

};

typedef struct tagQMessage {
	string message;
	int time_remains;
} QMessage;

class GUI
{
private:
	int controlCount;
	std::vector<QMessage> queue;

	int now_tick;
	int prev_tick;
public:

	GUIMessageAllocator guima;
	int queueCount;
	std::vector<GUIcontrol*> controls;

	GUIcontrol* mainmenu; // done
	GUIcontrol* difficultymenu; // done
	GUIcontrol* sizemenu; // done
	GUIcontrol* opponentsmenu; // done
	GUIcontrol* racemenu; // done
	GUIcontrol* loadmenu;
	GUIcontrol* optionsmenu;
	GUIcontrol* creditsmenu;

	GUIcontrol* gamemenu;


	void SetControlCount(int c) { controlCount = c; }
	int GetControlCount() const { return controlCount; }

	GUI()
	{

		SetControlCount(0);
		controls.resize(0);
		queue.resize(0);
		now_tick = 0;
		prev_tick = 0;
		queueCount = 0;

		guima.messagesCount = 0;
		guima.messages.resize(0);


#ifdef _DEBUG_
		LogToFile("GUI created");
#endif
	}

	void DrawAll()
	{

		// drawing controls
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if((*it))
				(*it)->DrawAll();
		}

		// drawing on-screen text
		int px = cfg.scr_width / 2; //LOG_SCREEN_X;
		int py = LOG_SCREEN_Y;
		for(std::vector<QMessage>::iterator it = queue.begin(); it != queue.end(); it++)
		{
			px += LOG_SCREEN_DX;
			py += LOG_SCREEN_DY;
			glColor4ub(255, 255, 255, 255);
			TextOut(it->message, LOG_SCREEN_FONT, &hDC, px, py);
		}
	}

	void Process();

	void AddControl(GUIcontrol* ctrl)
	{
		SetControlCount(GetControlCount() + 1);
		ctrl->guima = &guima;
		controls.push_back(ctrl);

	}

	LPSTR NameByGID(GID guid)
	{
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if((*it)->GetGID() == guid)
				return (*it)->GetName();
			for(std::vector<GUIcontrol*>::iterator it2 = (*it)->children.begin(); it2 != (*it)->children.end(); it2++)
			{
				if((*it2)->GetGID() == guid)
					return (*it2)->GetName();
			}
		}
		return NULL;
	}

	GUIcontrol* ItemByGID(GID guid)
	{
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if((*it)->GetGID() == guid)
				return (*it);
			for(std::vector<GUIcontrol*>::iterator it2 = (*it)->children.begin(); it2 != (*it)->children.end(); it2++)
			{
				if((*it2)->GetGID() == guid)
					return (*it2);
			}
		}
		return NULL;
	}

	GUIcontrol* ItemByName(LPSTR name)
	{
		if(name)

			for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
			{
				if(strcmp((*it)->GetName(), name) == 0)
					return (*it);
				for(std::vector<GUIcontrol*>::iterator it2 = (*it)->children.begin(); it2 != (*it)->children.end(); it2++)
				{
					if(strcmp((*it2)->GetName(), name) == 0)
						return (*it2);
				}
			}
			return NULL;

	}


	void DeleteControl(GUIcontrol* ctrl)
	{

		int i = -1;
		bool done = false;
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			i++;

			if((*it) == ctrl)
			{
				// its AWFUL, DO NOT WANT
				// so i should improve memory management
				//controls.erase(controls.begin() + i);
				controls[i] = NULL;
				
				delete ctrl;
				done = true;
				break;
			}
			if(done) break;
		}
		SetControlCount(GetControlCount() - 1);
	}

	void LogOnScreen(string message)
	{
		QMessage tmp;
		tmp.time_remains = LOG_SCREEN_TIME;
		tmp.message = message;
		queue.push_back(tmp);
		queueCount++;
	}

	void ViewControl(char* name)
	{
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			(*it)->SetVisible(false);
		}
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
		{
			if(strcmp(name, (*it)->name) == 0)
			{
				(*it)->SetVisible(true);
			}
		}
	}



	void ViewMenu(int id)
	{
		mainmenu->SetVisible(false);
		difficultymenu->SetVisible(false);
		sizemenu->SetVisible(false);
		racemenu->SetVisible(false);
		opponentsmenu->SetVisible(false);
		gamemenu->SetVisible(false);
		switch(id)
		{
		case 0:
			{
				mainmenu->SetVisible(true);
				break;
			}
		case 1:
			{
				difficultymenu->SetVisible(true);
				break;
			}
		case 2:
			{
				sizemenu->SetVisible(true);
				break;
			}
		case 3:
			{
				racemenu->SetVisible(true);
				break;
			}
		case 4:
			{
				opponentsmenu->SetVisible(true);
				break;
			}
		case 5:
			{
				gamemenu->SetVisible(true);
				break;
			}
		}
	}

	void Hide()
	{
		mainmenu->SetVisible(false);
		difficultymenu->SetVisible(false);
		sizemenu->SetVisible(false);
		racemenu->SetVisible(false);
		opponentsmenu->SetVisible(false);
	}

	void LoadFromDefinition(struct GUIDEFINITION* guidef);
	void ParseDefinitionElem(struct GUI_DEFINITION_ELEM* guielem);

	//void ShowMessage()
	//{
	//	GUIcontrol* msg;

	//	msg = new GUIcontrol("Message");


				

};



















