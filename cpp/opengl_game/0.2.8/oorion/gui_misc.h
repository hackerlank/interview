// GID - ���������� ������������� ������� ��������
typedef int GID;

// ����
typedef struct _COLOR4 {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
} COLOR4;

// ���� ��������� �� ������
const COLOR4 DEFAULT_SCREEN_MESSAGE_COLOR = { 255, 255, 255, 255 };

// ���� �������� ��-���������
const COLOR4 DEFAULT_CONTROL_COLOR = { 255, 255, 255, 255 };

// ���� ������� �� �������� ��-���������
const COLOR4 DEFAULT_TEXT_COLOR = { 255, 255, 255, 255 };

// ������������ ����� ���������� ����
const UINT GUI_TEXTBOX_TEXT_LENGTH = 256;

// �������
//	��� ��������� ���������, ������� �������� ������� (������ �� ��� ����
//	� �����). ��������� ��������� ������������ ������ ������ ���
// --------------------------------------------------------------------------
const UINT GUI_SGN_OPEN_GROUP	= 550; // ������� ������ ���������
const UINT GUI_SGN_EXIT			= 650; // ����� (�� ���������)		
const UINT GUI_SGN_SET_VAR		= 750; // ���������� �������� ����. �����.

	// ��������� �������� GUI_SGN_SET_VAR:
	const UINT GUI_SET_DIFF			= 200; // ���������� ���������
	const UINT GUI_SET_GALAXY_SIZE	= 210; //	������ ���������
	const UINT GUI_SET_RACE			= 220; //	����
	const UINT GUI_SET_OPP_COUNT	= 230; //	���������� ����������
	const UINT GUI_SET_GAME_STATE	= 240; //	��������� ����

const UINT GUI_SGN_START_GAME	= 850; // ������ ����
// --------------------------------------------------------------------------