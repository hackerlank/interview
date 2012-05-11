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

	// ������� ������� (� ������)
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

	// �����������
	GUIControlAllocator(GUIMessageAllocator* msg_alloc);
};
// --------------------------------------------------------------------------


