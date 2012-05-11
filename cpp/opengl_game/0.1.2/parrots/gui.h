typedef int GID;

extern HDC hDC;
extern font_t fonts[256];
extern config cfg;
extern unsigned int current_state;

#define LOG_SCREEN_TIME 3
#define LOG_SCREEN_X 150
#define LOG_SCREEN_Y 20
#define LOG_SCREEN_DX 5
#define LOG_SCREEN_DY 15

#define MAINMENU_ID 0
#define DIFFMENU_ID 1
#define SIZEMENU_ID 2
#define OPPONENTMENU_ID 3
#define RACEMENU_ID 4
#define LOADMENU_ID 5
#define OPTIONSMENU_ID 6
#define CREDITSMENU_ID 7
#define SAVEMENU_ID 8


enum ACTION { actNo, actOpenNext, actOpenPrevious, actClose, actExit };


//class GUIpanel
//{
//private:
//	LPSTR caption;
//	LPSTR text;
//	int pos_x;
//	int pos_y;
//	GUIbase* parent;
//	int childCount;
//protected:
//	font_t* captionFont;
//	font_t* textFont;
//	texture_t* tex;
//	BYTE color[4];
//	BYTE captionColor[4];
//	BYTE textColor[4];
//	int text_x;
//	int text_y;
//	int caption_x;
//	int caption_y;
//	bool caption_center;
//	bool text_center;
//public:
//	std::vector<GUIbase*> children;
//	LPSTR name;
//	bool IWannaDie;
//	bool visible;
//
//	GUIpanel(LPSTR ctrl_name)
//	{
//		children.resize(0);
//		name = ctrl_name;
//		parent = NULL;
//		childCount = 0;
//
//		pos_x = 0;
//		pos_y = 0;
//		textColor[0] = 255;
//		textColor[1] = 255;
//		textColor[2] = 255;
//		textColor[3] = 255;
//		captionColor[0] = 255;
//		captionColor[1] = 255;
//		captionColor[2] = 255;
//		captionColor[3] = 255;
//		texture = NULL;
//		captionFont = NULL;
//		textFont = NULL;
//		caption = NULL;
//		text = NULL;
//		text_x = 5;
//		text_y = 5;
//		caption_x = 5;
//		caption_y = 5;
//		caption_center = true;
//		text_center = true;
//		IWannaDie = false;
//		visible = false;
//	}
//
//	void SetName(LPSTR new_name)
//	{
//		name = new_name;
//	}
//
//	LPSTR GetName() const { return name; }
//
//	void SetTexture(texture_t* new_tex)
//	{
//		tex = new_tex;
//	}
//
//	texture_t* GetTexture() const { return tex; }
//
//	void SetCaptionFont(font_t* new_font)
//	{
//		captionFont = new_font;
//	}
//
//	font_t* GetCaptionFont() const { return captionFont; }
//
//	void SetTextFont(font_t* new_font)
//	{
//		textFont = new_font;
//	}
//
//	font_t* GetTextFont() const { return textFont; }
//
//	void SetText(LPSTR new_text)
//	{
//		if(new_text)
//			//strcpy(_text, new_text);
//			_text = new_text;
//	}
//
//	LPSTR GetText() const { return _text; }
//
//	void SetCaption(LPSTR new_caption)
//	{
//		if(new_caption)
//			//strcpy(_caption, new_caption);
//			_caption = new_caption;
//	}
//
//	LPSTR GetCaption() const { return _caption; }
//
//	void SetTextColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
//	{
//		textColor[0] = clrRed;
//		textColor[1] = clrGreen;
//		textColor[2] = clrBlue;
//		textColor[3] = clrAlpha;
//	}
//
//	void SetCaptionColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
//	{
//		captionColor[0] = clrRed;
//		captionColor[1] = clrGreen;
//		captionColor[2] = clrBlue;
//		captionColor[3] = clrAlpha;
//	}
//
//	void SetTextPos(const int new_x, const int new_y)
//	{
//		text_x = new_x;
//		text_y = new_y;
//	}
//
//	void SetCaptionPos(const int new_x, const int new_y)
//	{
//		caption_x = new_x;
//		caption_y = new_y;
//	}
//
//	int GetPatternWidth() const { return tex->width; }
//	int GetPatternHeight() const { return tex->height; }
//
//	void SetPos(const int x, const int y)
//	{
//		pos_x = x; pos_y = y;
//
//		if(parent)
//		{
//			pos_x += parent->GetPosX();
//			pos_y += parent->GetPosY();
//		}
//	}
//
//	void Centrify()
//	{
//		if(!parent)
//		{
//			if((GetPatternWidth() < cfg.scr_width) && (GetPatternHeight() < cfg.scr_height))
//				SetPos(cfg.scr_width / 2 - GetPatternWidth() / 2, cfg.scr_height / 2 - GetPatternHeight() / 2);
//		}
//		else
//		{
//			if((GetPatternWidth() < GetParent()->GetPatternWidth()) && (GetPatternHeight() < GetParent()->GetPatternHeight()))
//				SetPos(GetParent()->GetPatternWidth() / 2 - GetPatternWidth() / 2, GetParent()->GetPatternHeight() / 2 - GetPatternHeight() / 2);
//		}
//	}
//			
//
//	void SetColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha)
//	{
//		color[0] = clrRed;
//		color[1] = clrGreen;
//		color[2] = clrBlue;
//		color[3] = clrAlpha;
//	}
//
//	int GetPosX() {	return pos_x; }
//	int GetPosY() {	return pos_y; }
//
//	void SetChildCount(int c) { childCount = c; }
//	int GetChildCount() const { return childCount; }
//
//	virtual void Draw()
//	{
//		int tx, ty;
//
//		if(visible)
//		{
//			if(tex)
//			{
//				glColor4ubv(color);
//				tx = _pos_x;
//				ty = _pos_y;
//
//				DrawTexture(tx, ty, 0, 0, GetPatternWidth(), GetPatternHeight(), tex, false, false, 100, 100);
//			}
//
//			if(textFont && _text)
//			{
//				glColor4ubv(textColor);
//
//				int px,py;
//
//				px = pos_x + text_x;
//				py = pos_y + text_y;
//
//				if(text_center)
//				{
//					px += GetPatternWidth() / 4;
//					//py += GetPatternHeight();
//				}
//
//				TextOut(string(caption), captionFont, &hDC, px, py);
//			}
//
//			if(captionFont && caption)
//			{
//				glColor4ubv(captionColor);
//
//				int px,py;
//
//				px = pos_x + caption_x;
//				py = pos_y + caption_y;
//
//				if(caption_center)
//				{
//					px += GetPatternWidth() / 4;
//					//py += GetPatternHeight();
//				}
//
//				TextOut(string(caption), captionFont, &hDC, px, py);
//			}
//		}
//	}
//
//	virtual void DrawAll()
//	{
//		if(visible)
//		{
//			Draw();
//			for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
//			{
//				if((*it))
//				(*it)->DrawAll();
//			}
//		}
//
//	}
//
//	virtual void Process();
//	virtual void ProcessAll();
//
//
//	void SetParent(GUIcontrol* new_parent)
//	{
//		parent = new_parent;
//	}
//
//	GUIcontrol* GetParent()	{ return parent; }
//
//	void SetNext(GUIcontrol* new_next)
//	{
//		next = new_next;
//	}
//
//	GUIcontrol* GetNext() { return next; }
//
//	void SetPrevious(GUIcontrol* new_previous)
//	{
//		previous = new_previous;
//	}
//
//	GUIcontrol* GetPrevious() { return previous; }
//
//	GUIcontrol* FindByName(LPSTR name)
//	{
//		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
//			if(strcmp((*it)->GetName(), name))
//				return *it;
//	}
//
//	void AddChild(GUIcontrol* child)
//	{
//		children.push_back(child);
//		children[GetChildCount()]->parent = this;
//
//		SetChildCount(GetChildCount() + 1);
//		
//	}
//
//	void SetVisible(bool flag) 
//	{
//		visible = flag;
//
//		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
//		{
//			(*it)->SetVisible(flag);
//		}
//	}
//	bool GetVisible() const { return visible; }
//
//	void KillSelf()
//	{
//		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
//		{
//			this->KillChild(*it);
//		}
//		IWannaDie = true;
//				
//	}
//
//	void KillChild(GUIcontrol* child)
//	{
//		int i = -1;
//		bool done = false;
//		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
//		{
//			i++;
//			if( (*it) == child )
//			{
//
//				for(std::vector<GUIcontrol*>::iterator it2 = (*it)->children.begin(); it2 != (*it)->children.end(); it2++)
//				{
//					(*it)->KillChild(*it2);
//				}
//
//				(*it)->children.clear();
//				(*it)->SetChildCount(0);
//				//children.erase(children.begin() + i);				
//				this->children[i] = NULL;
//				
//				delete child;
//				done = true;
//				break;
//			}
//
//			if(done) break;
//		}
//	}
//
//
//	void Close()
//	{
//		if(parent) parent->KillSelf();
//	}
//
//	void ExitProgram()
//	{
//		current_state = 49; // GAME_EXITING
//		//exit(0);
//	}
//
//	void OpenNext()
//	{
//		if(parent)
//			parent->SetVisible(false);
//		else
//			SetVisible(false);
//
//		if(GetPrevious()) previous->SetVisible(false);
//		if(GetNext()) next->SetVisible(true);
//	}
//
//	void OpenPrevious()
//	{
//		if(parent)
//			parent->SetVisible(false);
//		else
//			SetVisible(false);
//
//		if(GetNext()) next->SetVisible(false);
//		if(GetPrevious()) previous->SetVisible(true);
//	}
//
//
//};

class GUIcontrol
{
private:
	GID id;
	GUIcontrol* parent;
	GUIcontrol* next;
	GUIcontrol* previous;
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
	GUIcontrol(LPSTR ctrl_name)
	{
		children.resize(0);
		name = ctrl_name;
		parent = NULL;
		next = NULL;
		previous = NULL;
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
		caption_x = 5;
		caption_y = 5;
		_caption_center = true;
		_text_center = true;
		_pressed = false;
		IWannaDie = false;
		visible = false;
		action = actNo;


	}

	void SetName(LPSTR new_name)
	{
		name = new_name;
	}

	LPSTR GetName() const { return name; }

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

	GID GetID() const { return id; }	

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
				tx = _pos_x;
				ty = _pos_y;

				DrawTexture(tx, ty, GetCurrentPattern() * GetPatternWidth(), 0, GetCurrentPattern() * GetPatternWidth() + GetPatternWidth(), GetPatternHeight(), tex, false, false, 100, 100);
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

	GUIcontrol* GetParent()	{ return parent; }

	void SetNext(GUIcontrol* new_next)
	{
		next = new_next;
	}

	GUIcontrol* GetNext() { return next; }

	void SetPrevious(GUIcontrol* new_previous)
	{
		previous = new_previous;
	}

	GUIcontrol* GetPrevious() { return previous; }

	GUIcontrol* FindByName(LPSTR name)
	{
		for(std::vector<GUIcontrol*>::iterator it = children.begin(); it != children.end(); it++)
			if(strcmp((*it)->GetName(), name))
				return *it;
	}

	void AddChild(GUIcontrol* child)
	{
		children.push_back(child);
		children[GetChildCount()]->parent = this;

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
		current_state = 49; // GAME_EXITING
		//exit(0);
	}

	void OpenNext()
	{
		if(parent)
			parent->SetVisible(false);
		else
			SetVisible(false);

		if(GetPrevious()) previous->SetVisible(false);
		if(GetNext()) next->SetVisible(true);
	}

	void OpenPrevious()
	{
		if(parent)
			parent->SetVisible(false);
		else
			SetVisible(false);

		if(GetNext()) next->SetVisible(false);
		if(GetPrevious()) previous->SetVisible(true);
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

	int queueCount;

	std::vector<GUIcontrol*> controls;

	GUIcontrol* mainmenu; // done
	GUIcontrol* difficultymenu; // done
	GUIcontrol* sizemenu; // done
	GUIcontrol* opponentsmenu; // done
	GUIcontrol* racemenu;
	GUIcontrol* loadmenu;
	GUIcontrol* optionsmenu;
	GUIcontrol* creditsmenu;
	GUIcontrol* savemenu;

	GUIcontrol* loader;

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
		int px = LOG_SCREEN_X;
		int py = LOG_SCREEN_Y;
		for(std::vector<QMessage>::iterator it = queue.begin(); it != queue.end(); it++)
		{
			px += LOG_SCREEN_DX;
			py += LOG_SCREEN_DY;
			TextOut(it->message, &fonts[0], &hDC, px, py);
		}
	}

	void Process();

	void AddControl(GUIcontrol* ctrl)
	{
		SetControlCount(GetControlCount() + 1);

		controls.push_back(ctrl);
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

	void ShowMainMenu();

	GUIcontrol* FindByName(LPSTR name)
	{
		for(std::vector<GUIcontrol*>::iterator it = controls.begin(); it != controls.end(); it++)
			if(strcmp((*it)->GetName(), name))
				return *it;
	}

	void ViewMenu(int id)
	{
		if(mainmenu) mainmenu->SetVisible(false);
		if(difficultymenu) difficultymenu->SetVisible(false);
		if(sizemenu) sizemenu->SetVisible(false);
		if(opponentsmenu) opponentsmenu->SetVisible(false);
		if(racemenu) racemenu->SetVisible(false);
		if(loadmenu) loadmenu->SetVisible(false);
		if(optionsmenu) optionsmenu->SetVisible(false);
		if(creditsmenu) creditsmenu->SetVisible(false);
		if(savemenu) savemenu->SetVisible(false);

		switch(id)
		{
		case 0:
			{
				if(mainmenu) mainmenu->SetVisible(true);
				break;
			}
		case 1:
			{
				if(difficultymenu) difficultymenu->SetVisible(true);
				break; 
			}
		case 2:
			{
				if(sizemenu) sizemenu->SetVisible(true);
				break;
			}
		case 3:
			{
				if(opponentsmenu) opponentsmenu->SetVisible(true);
				break;
			}
		case 4:
			{
				if(racemenu) racemenu->SetVisible(true);
				break;
			}
		case 5:
			{
				if(loadmenu) loadmenu->SetVisible(true);
				break;
			}
		case 6:
			{
				if(optionsmenu) optionsmenu->SetVisible(true);
				break;
			}
		case 7:
			{
				if(creditsmenu) creditsmenu->SetVisible(true);
				break;
			}
		case 8:
			{
				if(savemenu) savemenu->SetVisible(true);
				break;
			}
		}

	}


};
