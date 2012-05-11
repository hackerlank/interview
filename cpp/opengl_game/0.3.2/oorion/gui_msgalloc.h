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
	GID from;

	// ���������
	UINT msg;

	// ��� ��������
	int a;

	// ������
	GUISignal sgn;

} GUIMessage;
// --------------------------------------------------------------------------



// ������� ������
// --------------------------------------------------------------------------
const GUISignal NO_SIGNAL = { 0, 0, 0 };
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