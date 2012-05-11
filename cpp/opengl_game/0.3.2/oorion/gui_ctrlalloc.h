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

	// Текущий контрол (в фокусе)
	GID focused_control;

public:

	UINT GetControlCount();
	GUIMessageAllocator* GetMessages();
	GUIBaseControl* GetControlByGID(GID id);
	GUIBaseControl* GetControlByName(const char* name);
	vector<GUIBaseControl*>* GetItems();
	GID GetFocusedControl();

	void SetControlCount(UINT new_control_count);
	void SetMessages(GUIMessageAllocator* new_messages);
	void Clear();
	void Add(GUIBaseControl* item);
	void Delete(GUIBaseControl* item);
	void Draw();
	void Process();
	void SetFocusedControl(GID new_focused_control);

	// Конструктор
	GUIControlAllocator(GUIMessageAllocator* msg_alloc);
};
// --------------------------------------------------------------------------


