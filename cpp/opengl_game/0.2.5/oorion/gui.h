typedef int GID;



class GUI;
class GUIMessageAllocator;
class GUIScreenMessageAllocator;
class GUIControlAllocator;



class GUIControl;

typedef struct _GUIScreenMessage {
	char* message;
	int time_remains;
} GUIScreenMessage;

typedef struct _GUIMessage {
	bool solved;
	GUIControl* from;
	int a1;
	int a2;
} GUIMessage;

class GUI
{
private:
	GUIScreenMessageAllocator* ScreenMessages;
	GUIMessageAllocator* Messages;
	GUIControlAllocator* Controls;


public:

	GUI();

	void Draw();

	void Process();

	void LogOnScreen(char* message);

	GUIMessageAllocator* GetMessages() const { return Messages; }

};


class GUIMessageAllocator
{
protected:
	GUI* _gui;
	UINT _message_count;

public:
	
	vector<GUIMessage> messages;

	GUIMessageAllocator() { _message_count = 0; messages.resize(0); }


	void SetGUI(GUI* new_gui) { _gui = new_gui; }
	GUI* GetGUI() const { return _gui; }

	UINT GetMessageCount() const { return _message_count; }

	void Add(GUIMessage item);
	void Process();

};


class GUIControl
{
protected:
	LPSTR					_parent_name;
	LPSTR					_next_name;
	GUIControl*				_parent;
	GUIControl*				_next;
	UINT					_child_count;
	bool					_mouse_left;
	bool					_mouse_right;
	int						_mouse_x;
	int						_mouse_y;
	int						_pos_x;
	int						_pos_y;
	bool					_close;
	LPSTR					_caption;
	LPSTR					_text;
	int						_caption_dx;
	int						_caption_dy;
	bool					_caption_center;
	bool					_text_center;
	bool					_pressed;
	int						_pos_dx;
	int						_pos_dy;
	font_t*					_caption_font;
	font_t*					_text_font;
	UINT					_current_pattern;
	UINT					_pattern_count;
	UINT					_pattern_width;
	UINT					_pattern_height;
	BYTE					_color[4];
	BYTE					_text_color[4];
	BYTE					_caption_color[4];
	int						_text_x;
	int						_text_y;
	int						_caption_x;
	int						_caption_y;
	vector<GUIControl*>		_children;
	LPSTR					_name;
	int						_data;
	bool					_dead;
	bool					_visible;
	texture_t*				_tex;
	ACTION					_action;
	GROUP					_group;
	GID						_gid;
	GUIMessageAllocator*	_messages;

public:
	GUIControl(LPSTR ctrl_name);


	GID GetGID() { return _gid; }
	LPSTR GetName() { return _name; }
	void SetData(int new_data) { _data = new_data; }
	int GetData() { return _data; }
	void SetAction(ACTION new_action)	{		_action = new_action;	}
	ACTION GetAction() { return _action; }
	void SetTexture(texture_t* new_tex)	{		_tex = new_tex;	}
	texture_t* GetTexture() const { return _tex; }
	void SetCaptionFont(font_t* new_font)	{		_caption_font = new_font;	}
	font_t* GetCaptionFont() const { return _caption_font; }
	void SetTextFont(font_t* new_font)	{		_text_font = new_font;	}
	font_t* GetTextFont() const { return _text_font; }
	void SetText(LPSTR new_text)	{		if(new_text)			_text = new_text;	}
	LPSTR GetText() const { return _text; }
	void SetCaption(LPSTR new_caption)	{		if(new_caption)			_caption = new_caption;	}
	LPSTR GetCaption() const { return _caption; }
	void SetTextPos(const int new_x, const int new_y)	{		_text_x = new_x;		_text_y = new_y;	}
	void SetCaptionPos(const int new_x, const int new_y)	{		_caption_x = new_x;		_caption_y = new_y;	}
	void SetPatternCount(const int count)	{		_pattern_count = count;	}
	int GetPatternCount() const { return _pattern_count; }
	void SetCurrentPattern(const int index)	{		_current_pattern = index;	}
	int GetCurrentPattern() const { return _current_pattern; }
	void SetPatternWidth(const int new_width)	{		_pattern_width = new_width;	}
	void SetPatternHeight(const int new_height)	{		_pattern_height = new_height;	}
	int GetPatternWidth() const { return _pattern_width; }
	int GetPatternHeight() const { return _pattern_height; }
	int GetPosX() {	return _pos_x; }
	int GetPosY() {	return _pos_y; }
	bool IsClose() { return _close; }
	void SetChildCount(int c) { _child_count = c; }
	int GetChildCount() const { return _child_count; }
	virtual void Process();
	void SetParent(GUIControl* new_parent)	{		_parent = new_parent;	}
	void SetParentName(LPSTR new_name)	{		_parent_name = new_name;	}
	GUIControl* GetParent()	{ return _parent; }
	LPSTR GetParentName() { return _parent_name; }
	void SetNextName(LPSTR new_name)	{		if(new_name)			_next_name = new_name;	}
	void SetNext(GUIControl* new_next)	{		if(new_next)			_next = new_next;	}
	GUIControl* GetNext() { return _next; }
	LPSTR GetNextName() { return _next_name; }
	void KillSelf();
	void Close()	{		if(_parent) _parent->KillSelf();	}
	void ExitProgram();
	void SendMessage(GUIMessage message)	{		if(_messages) _messages->Add(message);	}
	void SetClose(bool flag) { _close = flag; }
	GROUP GetGroup() const { return _group; }
	void SetGroup(GROUP new_group) { _group = new_group; }
	void SetTextColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha);
	void SetCaptionColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha);
	void SetPos(const int x, const int y);
	void Centrify();
	void SetColor(BYTE clrRed, BYTE clrGreen, BYTE clrBlue, BYTE clrAlpha);
	virtual void Draw();
	void AddChild(GUIControl* child);
	void SetVisible(bool flag);
	void KillChild(GUIControl* child);
	void OpenNext();

	void SetMessages(GUIMessageAllocator* messages) { _messages = messages; }
	GUIMessageAllocator* GetMessages() const { return _messages; }

	void SetDead(bool flag) { _dead = flag; }
	bool GetDead() const { return _dead; }
	bool GetVisible() const { return _visible; }

};


class GUIScreenMessageAllocator
{
protected:
	UINT _message_count;

	UINT _tick1;
	UINT _tick2;

public:

	vector<GUIScreenMessage> messages;

	GUIScreenMessageAllocator()
	{ 
		messages.resize(0);
		_message_count = 0;
		_tick1 = 0;
		_tick2 = 0;
	}

	void Add(GUIScreenMessage item);

	void Delete(GUIScreenMessage* item);

		
	void Draw();

	void Process();


	void SetMessageCount(UINT new_count) { _message_count = new_count; }
	UINT GetMessageCount() const { return _message_count; }

};

class GUIControlAllocator
{
public:
	UINT controlCount;
	GUIMessageAllocator* messages;
	vector<GUIControl*> controls;

	GUIControlAllocator(GUIMessageAllocator* msg_alloc);

	void Add(GUIControl* item);

	void Delete(GUIControl* item);

	void Draw();


	void Process();

	LPSTR NameByGID(GID guid);

	GUIControl* ItemByGID(GID guid);

	GUIControl* ItemByName(LPSTR name);

	void ViewControl(char* name);


};



