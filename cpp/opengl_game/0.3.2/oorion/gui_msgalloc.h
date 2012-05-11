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
	GID from;

	// Сообщение
	UINT msg;

	// Его параметр
	int a;

	// Данные
	GUISignal sgn;

} GUIMessage;
// --------------------------------------------------------------------------



// Нулевой сигнал
// --------------------------------------------------------------------------
const GUISignal NO_SIGNAL = { 0, 0, 0 };
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