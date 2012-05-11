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