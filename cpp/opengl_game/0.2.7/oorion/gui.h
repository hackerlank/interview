// GID - Уникальный идентификатор каждого контрола
typedef int GID;

// Цвет
typedef struct _COLOR4 {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
} COLOR4;

// Цвет сообщения на экране
const COLOR4 DEFAULT_SCREEN_MESSAGE_COLOR = { 255, 255, 255, 255 };

// Цвет контрола по-умолчанию
const COLOR4 DEFAULT_CONTROL_COLOR = { 255, 255, 255, 255 };

// Цвет надписи на контроле по-умолчанию
const COLOR4 DEFAULT_TEXT_COLOR = { 255, 255, 255, 255 };

// Максимальная длина текстового поля
const UINT GUI_TEXTBOX_TEXT_LENGTH = 256;

// Сигналы
//	Это служебные сообщения, которые посылает контрол (влияет на всю игру
//	в целом). Остальные сообщения используются только внутри гуя
// --------------------------------------------------------------------------
const UINT GUI_SGN_OPEN_GROUP	= 550; // Открыть группу элементов
const UINT GUI_SGN_EXIT			= 650; // Выход (из программы)		
const UINT GUI_SGN_SET_VAR		= 750; // Установить значение сист. перем.
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



// Структура экранного сообщения GUI
// GUIMessage
// Примечание:
//	Используется для отрисовки на экран информации
// --------------------------------------------------------------------------
typedef struct _GUIScreenMessage {
	// Текст сообщения
	char* message;

	// Сколько времени (в секундах) будет существовать сообщение
	int time_remains;
} GUIScreenMessage;
// --------------------------------------------------------------------------



// Структура сообщения GUI
// GUIMessage
// Примечание:
//	Используется для обработки сообщений OnClick, OnMouseMove и т.д.
// --------------------------------------------------------------------------
typedef struct _GUISignal {
	// Сигнал
	UINT sgn;

	// Параметры основного сигнала
	UINT sgn_param_1;
	UINT sgn_param_2;

} GUISignal;

typedef struct _GUIMessage {
	// Сообщение обработано
	bool solved;

	// Кто послал сообщение
	GUIBaseControl* from;

	// Сообщение
	UINT msg;

	// Данные
	GUISignal sgn;

} GUIMessage;
// --------------------------------------------------------------------------


// Основной класс
// GUI
// Примечание:
//	Контролирует аллокаторы
//	Использует GUIMessageAllocator,
//				GUIScreenMessageAllocator,
//				GUIControlAllocator
// --------------------------------------------------------------------------
class GUI
{
private:
	// Аллокатор экранных сообщений
	GUIScreenMessageAllocator* ScreenMessages;

	// Аллокатор сообщений контролов
	GUIMessageAllocator* Messages;

	// Аллокатор контролов
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

	// Конструктор
	GUI();
};
// --------------------------------------------------------------------------



// Аллокатор сообщений элементов GUI
// GUIMessageAllocator
// Примечание:
//	Обрабатывает сообщения, полученные от контролов
//	Использует GUIMessage как элемент
// --------------------------------------------------------------------------
class GUIMessageAllocator
{
protected:
	// GUI, нужны некоторые методы оттуда, например вывод на экран
	// TODO: friend
	GUI* gui;

	// Количество сообщений в очереди
	UINT message_count;

	// Массив сообщений
	vector<GUIMessage> messages;

public:
	UINT GetMessageCount();
	GUI* GetGUI();

	void SetMessageCount(UINT new_message_count);
	void SetGUI(GUI* new_gui);
	void Add(GUIMessage item);
	void Process();
	void Clear();

	// Конструктор
	GUIMessageAllocator();
};
// --------------------------------------------------------------------------



// Аллокатор экранных сообщений GUI
// GUIScreenMessageAllocator
// Примечание:
//	Контролирует процесс отрисовки сообщений на экран
//	Использует GUIScreenMessage как элемент
// --------------------------------------------------------------------------
class GUIScreenMessageAllocator
{
protected:
	// Количество сообщений в очереди
	UINT message_count;

	// используется для подсчета времени сообщений)
	UINT tick1;
	UINT tick2;

	// Массив сообщений
	vector<GUIScreenMessage> messages;

public:
	UINT GetMessageCount();

	void SetMessageCount(UINT new_message_count);
	void Add(GUIScreenMessage item);
	void Delete(GUIScreenMessage* item);
	void Draw();
	void Process();
	void Clear();

	// Конструктор
	GUIScreenMessageAllocator();
};
// --------------------------------------------------------------------------



// Аллокатор контролов GUI
// GUIControlAllocator
// Примечание:
//	Держит контролы и управляет ими
//	Draw/Process/Add/Delete
// --------------------------------------------------------------------------
class GUIControlAllocator
{
protected:
	// Количество контролов
	UINT control_count;

	// Обработчик сообщений
	GUIMessageAllocator* messages;

	// Массив контролов
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

	// Конструктор
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

// Базовый класс объектов GUI
// GUIBaseControl
// Примечание:
//	Используется как базовый класс для GUIVidget
// --------------------------------------------------------------------------
class GUIBaseControl
{
public:
	// Имя контрола
	char* name;

	// Родитель
	GUIBaseControl* parent;

	// Идентификатор
	GID id;

	// Объект мертв или нет
	bool dead;

	// Аллокатор, получающий сообщения от контрола
	GUIMessageAllocator* messages;

	// Аллокатор, который содержит данный контрол
	GUIControlAllocator* allocator;

	// Видимый или нет
	bool visible;

	// Группа, к которой принадлежит данный объект
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

	// Конструктор
	GUIBaseControl();
	GUIBaseControl(char* name);
	
};
// --------------------------------------------------------------------------



// Базовый класс отображаемых объектов GUI
// GUIVidget (GUIBaseControl)
// Примечание:
//	
// --------------------------------------------------------------------------
class GUIVidget : public GUIBaseControl
{
protected:
	// Координата X
	int x;

	// Координата Y
	int y;

	// Ширина контрола
	UINT width;

	// Высота контрола
	UINT height;

	// Текстура контрола
	texture_t* texture;

	// Цвет контрола
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

	// Конструктор
	GUIVidget();
	GUIVidget(char* name);
};
// --------------------------------------------------------------------------



// Базовый класс объектов интерфейса GUI
// GUIControl (GUIVidget)
// Примечание:
//	Реагирует на события клавиатуры и мыши
// --------------------------------------------------------------------------
class GUIControl : public GUIVidget
{
protected:
	// Количество возможных состояний (текстура)
	UINT pattern_count;

	// Текущее состояние (текстура)
	UINT current_pattern;

	// Объект имеет фокус
	bool focused;

	// Доступен или нет
	bool enabled;

	// Заголовок
	char* caption;

	// Шрифт заголовка
	font_t* caption_font;

	// Цвет заголовка
	COLOR4 caption_color;

	// Y-координата заголовка
	int caption_x;

	// X-координата заголовка
	int caption_y;

	// Где в последний раз находился курсор
	int last_cursor_pos;

	// Последнее состояние нажатия
	int last_press_state;

	// Последнее отправленное сообщение
	int last_sent_msg;

	// Данные которые отсылает кнопка при нажатии
	// Затем обрабатывается MessageAllocator'ом
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

	// Конструктор
	GUIControl();
	GUIControl(char* name);
};
// --------------------------------------------------------------------------
// Класс текстовой надписи
// GUILabel (GUIVidget)
// Примечание:
//	
// --------------------------------------------------------------------------



class GUILabel : public GUIControl
{
protected:
	// Надпись
	char* text;

	// Шрифт
	font_t* font;

	// Позиция X надписи
	int text_x;

	// Позиция Y надписи
	int text_y;

	// Цвет надписи
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

	// Центрировать текст
	void CentrifyText();

	// Конструктор
	GUILabel(const char* name);
};
// --------------------------------------------------------------------------



// Класс иконки
// GUIIcon (GUIVidget)
// Примечание:
//	
// --------------------------------------------------------------------------
//typedef GUIVidget GUIIcon;
// --------------------------------------------------------------------------



// Класс прогрессбара
// GUIProgressBar (GUIVidget)
// Примечание:
//	
// --------------------------------------------------------------------------
class GUIProgressBar : public GUIControl
{
protected:
	// Шаг - один элемент прогрессбара
	texture_t* step;

	// Расстояние между шагами (на прогрессбаре)
	UINT step_margin;

	// Ширина шага (на прогрессбаре)
	UINT step_width;

	// Высота шага (на прогрессбаре)
	UINT step_height;

	// Стартовая координата X первого шага
	UINT step_start_x;

	// Стартовая координата Y первого шага
	UINT step_start_y;

	// Количество шагов - "длина" прогрессбара
	UINT step_count;

	// Текущий шаг
	UINT current_step;

	// Сила шага (прибавка к current_step)
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

	// Конструктор
	GUIProgressBar(const char* name);
};
// --------------------------------------------------------------------------



// Класс контейнера
// GUIContainer (GUIBaseControl)
// Примечание:
//	Просто контейнер, на основе его будет сделана панелька.
//	Только контейнер может содержать чайлдов
// --------------------------------------------------------------------------
class GUIContainer : public GUIBaseControl
{
protected:
	// Дети контейнера (любой объект)
	vector<GUIBaseControl*> children;

	// Количество детей
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

	// Конструктор
	GUIContainer(const char* name);
};
// --------------------------------------------------------------------------



// Класс панели
// GUIContainer (GUIVidget)
// Примечание:
//	Панель это оформленный контейнер
//	Еще не сделал
// --------------------------------------------------------------------------
//typedef GUIContainer GUIPanel;
// --------------------------------------------------------------------------



// Класс кнопки
// GUIButton (GUIControl)
// Примечание:
//	Просто кнопка
// --------------------------------------------------------------------------
class GUIButton : public GUIControl
{
public:
	void OnLeftClick();
	void OnMouseMove();

	// Конструктор
	GUIButton();
	GUIButton(char* name);
};
// --------------------------------------------------------------------------



// Класс чекбокса
// GUIButton (GUIControl)
// Примечание:
//	Имеет два состояния - включен/выключен
// --------------------------------------------------------------------------
class GUICheckBox : public GUIControl
{
protected:
	// состояние
	bool checked;

	// Надпись на чекбоксе
	char* caption;

	// Шрифт надписи
	font_t* caption_font;

	// Цвет надписи
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

	// Конструктор
	GUICheckBox(const char* name);
};
// --------------------------------------------------------------------------



// Класс радиобаттона
// GUIRadioButton (GUIControl)
// Примечание:
//	Только один радиобаттон из группы может быть включен
// --------------------------------------------------------------------------
class GUIRadioButton : public GUIControl
{
protected:
	// состояние
	bool checked;

	// Надпись на радиобаттоне
	char* caption;

	// Шрифт надписи
	font_t* caption_font;

	// Цвет надписи
	COLOR4 caption_color;

	// Группа к которой принадлежит радиобаттон
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

	// Конструктор
	GUIRadioButton(const char* name);
};
// --------------------------------------------------------------------------



// Класс текстбокса
// GUITextBox (GUIControl)
// Примечание:
//	Строка текста
// --------------------------------------------------------------------------
class GUITextBox : public GUIControl
{
protected:
	// Текст, введенный в поле
	char text[GUI_TEXTBOX_TEXT_LENGTH];

	// Шрифт текстового поля
	font_t* text_font;

	// Цвет текстового поля
	COLOR4 text_color;

	// Длина введенной строки
	UINT text_length;

	// Положение указателя в строке
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

	// Конструктор
	GUITextBox(const char* name);
};
// --------------------------------------------------------------------------



// Класс текстового поля
// GUIMemo (GUIControl)
// Примечание:
//	Большое поле для ввода
// --------------------------------------------------------------------------
//typedef GUITextBox GUIMemo;
// --------------------------------------------------------------------------