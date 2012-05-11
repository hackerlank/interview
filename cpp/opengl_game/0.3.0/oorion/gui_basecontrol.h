// Базовый класс объектов GUI
// GUIBaseControl
// Примечание:
//	Используется как базовый класс для GUIVidget
// --------------------------------------------------------------------------
class GUIBaseControl
{
protected:
	void Init();

public:
	// Имя контрола
	//char* name;
	string name;

	// Родитель
	GUIBaseControl* parent;

	// Идентификатор
	GID id;

	// Объект имеет фокус
	bool focused;

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

	//char* GetName();
	string GetName();
	GUIBaseControl* GetParent();
	GID GetId();
	GUIMessageAllocator* GetMessages();
	GUIControlAllocator* GetAllocator();
	bool IsDead();
	bool IsVisible();
	bool IsFocused();
	UINT GetGroup();

	//virtual void SetName(char* new_name);
	virtual void SetName(string new_name);
	virtual void SetParent(GUIBaseControl* new_parent);
	virtual void SetParent(char* parent_name);
	virtual void SetId(GID new_id);
	virtual void SetMessages(GUIMessageAllocator* new_messages);
	virtual void SetAllocator(GUIControlAllocator* new_allocator);

	virtual void SetDead(bool new_dead);
	virtual void SetVisible(bool new_visible);
	virtual void SetFocus(bool new_focused);
	virtual void SetGroup(UINT new_group);

	virtual void Draw();
	virtual void Process();

	// Конструктор
	GUIBaseControl();
	//GUIBaseControl(char* name);
	GUIBaseControl(string name);

	// Деструктор
	//virtual ~GUIBaseControl();
	
};
// --------------------------------------------------------------------------