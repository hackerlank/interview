// ������� ����� �������� GUI
// GUIBaseControl
// ����������:
//	������������ ��� ������� ����� ��� GUIVidget
// --------------------------------------------------------------------------
class GUIBaseControl
{
protected:
	void Init();

public:
	// ��� ��������
	//char* name;
	string name;

	// ��������
	GUIBaseControl* parent;

	// �������������
	GID id;

	// ������ ����� �����
	bool focused;

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

	// �����������
	GUIBaseControl();
	//GUIBaseControl(char* name);
	GUIBaseControl(string name);

	// ����������
	//virtual ~GUIBaseControl();
	
};
// --------------------------------------------------------------------------