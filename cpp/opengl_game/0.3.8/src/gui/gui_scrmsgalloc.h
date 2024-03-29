// ��������� ��������� ��������� GUI
// GUIMessage
// ����������:
//	������������ ��� ��������� �� ����� ����������
// --------------------------------------------------------------------------
typedef struct _GUIScreenMessage {
	// ����� ���������
	//char* message;
	string message;

	// ������� ������� (� ��������) ����� ������������ ���������
	int time_remains;

	int id;

} GUIScreenMessage;
// --------------------------------------------------------------------------



// ��������� �������� ��������� GUI
// GUIScreenMessageAllocator
// ����������:
//	������������ ������� ��������� ��������� �� �����
//	���������� GUIScreenMessage ��� �������
// --------------------------------------------------------------------------
class GUIScreenMessageAllocator
{
private:
	UINT _startup_time;

protected:
	// ���������� ��������� � �������
	UINT message_count;

	// ������������ ��� �������� ������� ���������)
	UINT tick1;
	UINT tick2;

	// ������ ���������
	vector<GUIScreenMessage> messages;

public:
	UINT GetMessageCount();

	void SetMessageCount(UINT new_message_count);
	void Add(GUIScreenMessage item);
	//void Delete(GUIScreenMessage* item);
	void Draw();
	void Process();
	void Clear();

	// �����������
	GUIScreenMessageAllocator();
};
// --------------------------------------------------------------------------