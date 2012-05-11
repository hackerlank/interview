// GID - ���������� ������������� ������� ��������
typedef int GID;

// ����
typedef struct _COLOR4 {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
} COLOR4;

// ���� ��������� �� ������
const COLOR4 DEFAULT_SCREEN_MESSAGE_COLOR = { 255, 255, 255, 255 };

// ���� �������� ��-���������
const COLOR4 DEFAULT_CONTROL_COLOR = { 255, 255, 255, 255 };

// ���� ������� �� �������� ��-���������
const COLOR4 DEFAULT_TEXT_COLOR = { 255, 255, 255, 255 };

// ������������ ����� ���������� ����
const UINT GUI_TEXTBOX_TEXT_LENGTH = 256;

// �������
//	��� ��������� ���������, ������� �������� ������� (������ �� ��� ����
//	� �����). ��������� ��������� ������������ ������ ������ ���
// --------------------------------------------------------------------------
const UINT GUI_SGN_OPEN_GROUP	= 550; // ������� ������ ���������
const UINT GUI_SGN_EXIT			= 650; // ����� (�� ���������)		
const UINT GUI_SGN_SET_VAR		= 750; // ���������� �������� ����. �����.
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
class GUI;
class GUIMessageAllocator;
class GUIScreenMessageAllocator;
class GUIControlAllocator;

class GUIBaseControl;
class GUIVidget;
class GUIContainer;
class GUIPanel;
class GUIIcon;
class GUILabel;
class GUIProgressBar;
class GUIControl;
class GUIButton;
class GUITextBox;
class GUIMemo;
class GUIRadioButton;
class GUICheckbox;
// --------------------------------------------------------------------------

#define GUI_MOUSE_LEFT_CLICK	0x0FF0
#define GUI_MOUSE_RIGHT_CLICK	0x0FF1
#define GUI_MOUSE_MOVE			0x0FF2
#define GUI_MOUSE_ENTER			0x0FF3
#define GUI_MOUSE_LEAVE			0x0FF4
#define GUI_MOUSE_DOWN			0x0FF5
#define GUI_MOUSE_UP			0x0FF6
#define GUI_KEY_DOWN			0x0FF7
#define GUI_KEY_UP				0x0FF8



// ��������� ��������� ��������� GUI
// GUIMessage
// ����������:
//	������������ ��� ��������� �� ����� ����������
// --------------------------------------------------------------------------
typedef struct _GUIScreenMessage {
	// ����� ���������
	char* message;

	// ������� ������� (� ��������) ����� ������������ ���������
	int time_remains;
} GUIScreenMessage;
// --------------------------------------------------------------------------



// ��������� ��������� GUI
// GUIMessage
// ����������:
//	������������ ��� ��������� ��������� OnClick, OnMouseMove � �.�.
// --------------------------------------------------------------------------
typedef struct _GUISignal {
	// ������
	UINT sgn;

	// ��������� ��������� �������
	UINT sgn_param_1;
	UINT sgn_param_2;

} GUISignal;

typedef struct _GUIMessage {
	// ��������� ����������
	bool solved;

	// ��� ������ ���������
	GUIBaseControl* from;

	// ���������
	UINT msg;

	// ������
	GUISignal sgn;

} GUIMessage;
// --------------------------------------------------------------------------


// �������� �����
// GUI
// ����������:
//	������������ ����������
//	���������� GUIMessageAllocator,
//				GUIScreenMessageAllocator,
//				GUIControlAllocator
// --------------------------------------------------------------------------
class GUI
{
private:
	// ��������� �������� ���������
	GUIScreenMessageAllocator* ScreenMessages;

	// ��������� ��������� ���������
	GUIMessageAllocator* Messages;

	// ��������� ���������
	GUIControlAllocator* Controls;

public:
	GUIScreenMessageAllocator* GetScreenMessages();
	GUIMessageAllocator* GetMessages();
	GUIControlAllocator* GetControls();

	void SetScreenMessages(GUIScreenMessageAllocator* new_screen_messages);
	void SetMessages(GUIMessageAllocator* new_messages);
	void SetControls(GUIControlAllocator* new_controls);
	void Draw();
	void Process();

	void LogOnScreen(char* message);

	GUIMessageAllocator* GetMessages() const { return Messages; }

	// �����������
	GUI();
};
// --------------------------------------------------------------------------



// ��������� ��������� ��������� GUI
// GUIMessageAllocator
// ����������:
//	������������ ���������, ���������� �� ���������
//	���������� GUIMessage ��� �������
// --------------------------------------------------------------------------
class GUIMessageAllocator
{
protected:
	// GUI, ����� ��������� ������ ������, �������� ����� �� �����
	// TODO: friend
	GUI* gui;

	// ���������� ��������� � �������
	UINT message_count;

	// ������ ���������
	vector<GUIMessage> messages;

public:
	UINT GetMessageCount();
	GUI* GetGUI();

	void SetMessageCount(UINT new_message_count);
	void SetGUI(GUI* new_gui);
	void Add(GUIMessage item);
	void Process();
	void Clear();

	// �����������
	GUIMessageAllocator();
};
// --------------------------------------------------------------------------



// ��������� �������� ��������� GUI
// GUIScreenMessageAllocator
// ����������:
//	������������ ������� ��������� ��������� �� �����
//	���������� GUIScreenMessage ��� �������
// --------------------------------------------------------------------------
class GUIScreenMessageAllocator
{
protected:
	// ���������� ��������� � �������
	UINT message_count;

	// ������������ ��� �������� ������� ���������)
	UINT tick1;
	UINT tick2;

	// ������ ���������
	vector<GUIScreenMessage> messages;

public:
	UINT GetMessageCount();

	void SetMessageCount(UINT new_message_count);
	void Add(GUIScreenMessage item);
	void Delete(GUIScreenMessage* item);
	void Draw();
	void Process();
	void Clear();

	// �����������
	GUIScreenMessageAllocator();
};
// --------------------------------------------------------------------------



// ��������� ��������� GUI
// GUIControlAllocator
// ����������:
//	������ �������� � ��������� ���
//	Draw/Process/Add/Delete
// --------------------------------------------------------------------------
class GUIControlAllocator
{
protected:
	// ���������� ���������
	UINT control_count;

	// ���������� ���������
	GUIMessageAllocator* messages;

	// ������ ���������
	vector<GUIBaseControl*> controls;

public:
	UINT GetControlCount();
	GUIMessageAllocator* GetMessages();
	GUIBaseControl* GetControlByGID(GID id);
	GUIBaseControl* GetControlByName(char* name);
	vector<GUIBaseControl*>* GetItems();

	void SetControlCount(UINT new_control_count);
	void SetMessages(GUIMessageAllocator* new_messages);
	void Clear();
	void Add(GUIBaseControl* item);
	void Delete(GUIBaseControl* item);
	void Draw();
	void Process();
	void ViewControl(char* name);

	// �����������
	GUIControlAllocator(GUIMessageAllocator* msg_alloc);
};
// --------------------------------------------------------------------------



/* --------------------------------------------------------------------------

						                    GUIProgressBar
						                     /
						                     |  GUIIcon			GUITextBox
						                     |  /				GUICheckBox
GUIBaseControl	-	GUIVidget	--------	GUIControl		-	GUIButton
						                     |	\				GUIRadioButton
						                     |	GUILabel		GUIMemo
						                     \
						                    GUIContainer - GUIPanel

	------------------------------------------------------------------------ */

// ������� ����� �������� GUI
// GUIBaseControl
// ����������:
//	������������ ��� ������� ����� ��� GUIVidget
// --------------------------------------------------------------------------
class GUIBaseControl
{
public:
	// ��� ��������
	char* name;

	// ��������
	GUIBaseControl* parent;

	// �������������
	GID id;

	// ������ ����� ��� ���
	bool dead;

	// ���������, ���������� ��������� �� ��������
	GUIMessageAllocator* messages;

	// ���������, ������� �������� ������ �������
	GUIControlAllocator* allocator;

	// ������� ��� ���
	bool visible;

	// ������, � ������� ����������� ������ ������
	UINT group;

	char* GetName();
	GUIBaseControl* GetParent();
	GID GetId();
	GUIMessageAllocator* GetMessages();
	GUIControlAllocator* GetAllocator();
	bool IsDead();
	bool IsVisible();
	UINT GetGroup();

	virtual void SetName(char* new_name);
	virtual void SetParent(GUIBaseControl* new_parent);
	virtual void SetParent(char* parent_name);
	virtual void SetId(GID new_id);
	virtual void SetMessages(GUIMessageAllocator* new_messages);
	virtual void SetAllocator(GUIControlAllocator* new_allocator);

	virtual void SetDead(bool new_dead);
	virtual void SetVisible(bool new_visible);
	virtual void SetGroup(UINT new_group);

	virtual void Draw();
	virtual void Process();

	// �����������
	GUIBaseControl();
	GUIBaseControl(char* name);
	
};
// --------------------------------------------------------------------------



// ������� ����� ������������ �������� GUI
// GUIVidget (GUIBaseControl)
// ����������:
//	
// --------------------------------------------------------------------------
class GUIVidget : public GUIBaseControl
{
protected:
	// ���������� X
	int x;

	// ���������� Y
	int y;

	// ������ ��������
	UINT width;

	// ������ ��������
	UINT height;

	// �������� ��������
	texture_t* texture;

	// ���� ��������
	COLOR4 color;

public:
	int GetX();
	int GetY();
	UINT GetWidth();
	UINT GetHeight();
	texture_t* GetTexture();
	COLOR4 GetColor();
	//bool IsVisible();

	virtual void SetX(int new_x);
	virtual void SetY(int new_y);
	virtual void SetWidth(UINT new_width);
	virtual void SetHeight(UINT new_height);
	virtual void SetTexture(texture_t* new_texture);
	virtual void SetColor(COLOR4 new_color);
	//virtual void SetVisible(bool new_visible);

	virtual void Draw();
	virtual void Process();

	// �����������
	GUIVidget();
	GUIVidget(char* name);
};
// --------------------------------------------------------------------------



// ������� ����� �������� ���������� GUI
// GUIControl (GUIVidget)
// ����������:
//	��������� �� ������� ���������� � ����
// --------------------------------------------------------------------------
class GUIControl : public GUIVidget
{
protected:
	// ���������� ��������� ��������� (��������)
	UINT pattern_count;

	// ������� ��������� (��������)
	UINT current_pattern;

	// ������ ����� �����
	bool focused;

	// �������� ��� ���
	bool enabled;

	// ���������
	char* caption;

	// ����� ���������
	font_t* caption_font;

	// ���� ���������
	COLOR4 caption_color;

	// Y-���������� ���������
	int caption_x;

	// X-���������� ���������
	int caption_y;

	// ��� � ��������� ��� ��������� ������
	int last_cursor_pos;

	// ��������� ��������� �������
	int last_press_state;

	// ��������� ������������ ���������
	int last_sent_msg;

	// ������ ������� �������� ������ ��� �������
	// ����� �������������� MessageAllocator'��
	vector<GUISignal> sgns;
	//UINT sgn;
	//UINT sgn_param_1;
	//UINT sgn_param_2;

	void SetLastCursorPos(int new_last_cursor_pos);
	void SetLastPressState(int new_last_press_state);
	void SetLastSentMsg(int new_last_sent_msg);

	int GetLastCursorPos();
	int GetLastPressState();
	int GetLastSentMsg();

public:
	UINT GetPatternCount();
	UINT GetCurrentPattern();
	bool IsFocused();
	bool IsEnabled();
	char* GetCaption();
	font_t* GetCaptionFont();
	COLOR4 GetCaptionColor();
	int GetCaptionX();
	int GetCaptionY();
	//UINT GetSgn();
	//UINT GetSgnParam1();
	//UINT GetSgnParam2();

	void SetPatternCount(UINT new_pattern_count);
	void SetCurrentPattern(UINT new_current_pattern);
	void SetFocus(bool new_focused);
	void SetEnabled(bool new_enabled);
	void SetCaption(char* new_caption);
	void SetCaptionFont(font_t* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);
	void SetCaptionX(int new_caption_x);
	void SetCaptionY(int new_caption_y);
	//void SetSgn(UINT new_sgn);
	//void SetSgnParam1(UINT new_sgn_param_1);
	//void SetSgnParam2(UINT new_sgn_param_2);
	void AddSgn(UINT sgn, UINT param_1, UINT param_2);

	virtual void SendMessage(GUIMessage msg);
	virtual void OnMouseMove();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnMouseDown();
	virtual void OnMouseUp();
	virtual void OnLeftClick();
	virtual void OnRightClick();
	virtual void OnKeyDown();
	virtual void OnKeyUp();


	virtual void Draw();
	virtual void Process();

	// �����������
	GUIControl();
	GUIControl(char* name);
};
// --------------------------------------------------------------------------
// ����� ��������� �������
// GUILabel (GUIVidget)
// ����������:
//	
// --------------------------------------------------------------------------



class GUILabel : public GUIControl
{
protected:
	// �������
	char* text;

	// �����
	font_t* font;

	// ������� X �������
	int text_x;

	// ������� Y �������
	int text_y;

	// ���� �������
	COLOR4 caption_color;

public:
	char* GetText();
	font_t* GetFont();
	int GetTextX();
	int GetTextY();
	COLOR4 GetCaptionColor();

	void SetText(char* new_text);
	void SetFont(font_t* new_font);
	void SetTextX(int new_text_x);
	void SetTextY(int new_text_y);
	void SetCaptionColor(COLOR4 new_caption_color);

	// ������������ �����
	void CentrifyText();

	// �����������
	GUILabel(const char* name);
};
// --------------------------------------------------------------------------



// ����� ������
// GUIIcon (GUIVidget)
// ����������:
//	
// --------------------------------------------------------------------------
//typedef GUIVidget GUIIcon;
// --------------------------------------------------------------------------



// ����� ������������
// GUIProgressBar (GUIVidget)
// ����������:
//	
// --------------------------------------------------------------------------
class GUIProgressBar : public GUIControl
{
protected:
	// ��� - ���� ������� ������������
	texture_t* step;

	// ���������� ����� ������ (�� ������������)
	UINT step_margin;

	// ������ ���� (�� ������������)
	UINT step_width;

	// ������ ���� (�� ������������)
	UINT step_height;

	// ��������� ���������� X ������� ����
	UINT step_start_x;

	// ��������� ���������� Y ������� ����
	UINT step_start_y;

	// ���������� ����� - "�����" ������������
	UINT step_count;

	// ������� ���
	UINT current_step;

	// ���� ���� (�������� � current_step)
	UINT step_increment;

public:
	texture_t* GetStep();
	UINT GetStepMargin();
	UINT GetStepWidth();
	UINT GetStepHeight();
	UINT GetStepStartX();
	UINT GetStepStartY();
	UINT GetStepCount();
	UINT GetCurrentStep();
	UINT GetStepIncrement();

	void SetStep(texture_t* new_step);
	void SetStepMargin(UINT new_margin);
	void SetStepWidth(UINT new_width);
	void SetStepHeight(UINT new_height);
	void SetStepStartX(UINT new_step_start_x);
	void SetStepStartY(UINT new_step_start_y);
	void SetStepCount(UINT new_step_count);
	void SetCurrentStep(UINT new_current_step);
	void SetStepIncrement(UINT new_step_increment);

	// �����������
	GUIProgressBar(const char* name);
};
// --------------------------------------------------------------------------



// ����� ����������
// GUIContainer (GUIBaseControl)
// ����������:
//	������ ���������, �� ������ ��� ����� ������� ��������.
//	������ ��������� ����� ��������� �������
// --------------------------------------------------------------------------
class GUIContainer : public GUIBaseControl
{
protected:
	// ���� ���������� (����� ������)
	vector<GUIBaseControl*> children;

	// ���������� �����
	UINT child_count;

public:
	UINT GetChildCount();
	GUIBaseControl* GetControlByName(char* name);
	GUIBaseControl* GetControlByGID(GID id);
	
	void AddChild(GUIBaseControl* child);
	void DeleteChildByName(char* name);
	void SetChildCount(UINT new_child_count);
	void Clear();
	void Show();
	void Hide();

	// �����������
	GUIContainer(const char* name);
};
// --------------------------------------------------------------------------



// ����� ������
// GUIContainer (GUIVidget)
// ����������:
//	������ ��� ����������� ���������
//	��� �� ������
// --------------------------------------------------------------------------
//typedef GUIContainer GUIPanel;
// --------------------------------------------------------------------------



// ����� ������
// GUIButton (GUIControl)
// ����������:
//	������ ������
// --------------------------------------------------------------------------
class GUIButton : public GUIControl
{
public:
	void OnLeftClick();
	void OnMouseMove();

	// �����������
	GUIButton();
	GUIButton(char* name);
};
// --------------------------------------------------------------------------



// ����� ��������
// GUIButton (GUIControl)
// ����������:
//	����� ��� ��������� - �������/��������
// --------------------------------------------------------------------------
class GUICheckBox : public GUIControl
{
protected:
	// ���������
	bool checked;

	// ������� �� ��������
	char* caption;

	// ����� �������
	font_t* caption_font;

	// ���� �������
	COLOR4 caption_color;

public:
	bool IsChecked();
	char* GetCaption();
	font_t* GetCaptionFont();
	COLOR4 GetCaptionColor();

	void SetChecked(bool new_checked);
	void SetCaption(char* new_caption);
	void SetCaptionFont(font_t* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);

	// �����������
	GUICheckBox(const char* name);
};
// --------------------------------------------------------------------------



// ����� ������������
// GUIRadioButton (GUIControl)
// ����������:
//	������ ���� ����������� �� ������ ����� ���� �������
// --------------------------------------------------------------------------
class GUIRadioButton : public GUIControl
{
protected:
	// ���������
	bool checked;

	// ������� �� ������������
	char* caption;

	// ����� �������
	font_t* caption_font;

	// ���� �������
	COLOR4 caption_color;

	// ������ � ������� ����������� �����������
	UINT radio_group;

public:
	bool IsChecked();
	char* GetCaption();
	font_t* GetCaptionFont();
	COLOR4 GetCaptionColor();
	UINT GetRadioGroup();

	void SetChecked(bool new_checked);
	void SetCaption(char* new_caption);
	void SetCaptionFont(font_t* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);
	void SetRadioGroup(UINT new_radio_group);

	// �����������
	GUIRadioButton(const char* name);
};
// --------------------------------------------------------------------------



// ����� ����������
// GUITextBox (GUIControl)
// ����������:
//	������ ������
// --------------------------------------------------------------------------
class GUITextBox : public GUIControl
{
protected:
	// �����, ��������� � ����
	char text[GUI_TEXTBOX_TEXT_LENGTH];

	// ����� ���������� ����
	font_t* text_font;

	// ���� ���������� ����
	COLOR4 text_color;

	// ����� ��������� ������
	UINT text_length;

	// ��������� ��������� � ������
	UINT text_caret_pos;

public:
	char* GetText();
	font_t* GetTextFont();
	//COLOR4 GetTextColor();
	UINT GetTextLength();
	UINT GetTextCaretPos();

	void SetText(char* new_text);
	void SetTextFont(font_t* new_font);
	//void SetTextColor(BYTE new_text_color[4]);
	void SetTextLength(UINT new_text_length);
	void SetTextCaretPos(UINT new_text_caret_pos);

	// �����������
	GUITextBox(const char* name);
};
// --------------------------------------------------------------------------



// ����� ���������� ����
// GUIMemo (GUIControl)
// ����������:
//	������� ���� ��� �����
// --------------------------------------------------------------------------
//typedef GUITextBox GUIMemo;
// --------------------------------------------------------------------------