#include "../misc.h"

// ����� ���������� ����
// GUIMemo (GUIControl)
// ����������:
//	������� ���� ��� �����
// --------------------------------------------------------------------------
class GUIMemo : public GUIControl
{
protected:
	// ������
	vector<string> lines;

	// ����� �����
	Font* lines_font;

	// ���� �����
	COLOR4 lines_color;

	// ��������� �������
	Point caret_pos;

	// ������ ������ ������ ������
	int dy;

	// ������ ����� �����
	int dx;

	// ���������� ����� ��������
	int lines_dy;

	// ������������� ��� ���
	bool editable;

public:
	vector<string>* GetLines();
	Font* GetLinesFont();
	COLOR4 GetLinesColor();
	Point GetCaretPos();

	int GetDY();
	int GetDX();
	int GetLinesDY();
	bool IsEditable();

	void SetLinesFont(Font* new_value);
	void SetLinesColor(COLOR4 new_value);
	void SetCaretPos(Point new_value);
	void SetDY(int new_value);
	void SetDX(int new_value);
	void SetLinesDY(int new_value);
	void SetEditable(bool new_value);
	void AddLine(string line);

	void Draw();
	//void OnKeyDown(UCHAR key);
	//void OnKeyUp(UCHAR key);

	// �����������
	GUIMemo();
	GUIMemo(string name);
};
// --------------------------------------------------------------------------