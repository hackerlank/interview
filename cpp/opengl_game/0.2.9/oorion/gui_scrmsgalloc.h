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