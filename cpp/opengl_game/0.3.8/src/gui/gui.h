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

	void FreeScreenMessages();
	void FreeMessages();
	void FreeControls();

	void SetScreenMessages(GUIScreenMessageAllocator* new_screen_messages);
	void SetMessages(GUIMessageAllocator* new_messages);
	void SetControls(GUIControlAllocator* new_controls);
	void Draw();
	void Process();

	//void LogOnScreen(char* message);
	void LogOnScreen(string message);

	// врапперы, для использования iiInstanceAllocator'ом
	void wrapperShowGroup(int index);
	void wrapperHideGroup(int index);

	// Конструктор
	GUI();

	// Деструктор
	// Освобождает аллокаторы сообщений и пр.
	~GUI();
};
// --------------------------------------------------------------------------