// GID - ���������� ������������� ������� ��������
typedef int GID;

// ����
struct COLOR4 {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
};

// ���� ��������� �� ������
const COLOR4 DEFAULT_SCREEN_MESSAGE_COLOR = { 255, 255, 255, 255 };

// ���� �������� ��-���������
const COLOR4 DEFAULT_CONTROL_COLOR = { 255, 255, 255, 255 };

// ���� ������� �� �������� ��-���������
const COLOR4 DEFAULT_TEXT_COLOR = { 255, 255, 255, 255 };

// ������� ���� �������� ��-���������
const COLOR4 DEFAULT_BACKGROUND_COLOR = { 0, 0, 0, 0 };


// ������������ ����� ���������� ����
const UINT GUI_TEXTBOX_TEXT_LENGTH = 256;

// �������
//	��� ��������� ���������, ������� �������� ������� (������ �� ��� ����
//	� �����). ��������� ��������� ������������ ������ ������ ���
// --------------------------------------------------------------------------
const UINT GUI_SGN_OPEN_GROUP	 = 550; // ������� ������ ���������
const UINT GUI_SGN_CLOSE_PROGRAM = 650; // ����� (�� ���������)	
const UINT GUI_SGN_EXIT_GAME	 = 652; // ����� �� ����
//const UINT GUI_SGN_SET_VAR		 = 750; // ���������� �������� ����. �����.
const UINT GUI_SGN_START_GAME	 = 850; // ������ ����
const UINT GUI_SGN_CONTINUE_GAME = 855; // ���������� ����
//const UINT GUI_SGN_LOAD_LEVEL	 = 950; // ��������� �������
// --------------------------------------------------------------------------