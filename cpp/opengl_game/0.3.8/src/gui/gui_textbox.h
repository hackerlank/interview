// ����� ����������
// GUITextBox (GUIControl)
// ����������:
//	������ ������
// --------------------------------------------------------------------------
class GUITextBox : public GUIControl
{
protected:
	// �����, ��������� � ����
	char* text;

	// ����� ���������� ����
	Font* text_font;

	// ���� ���������� ����
	COLOR4 text_color;

	// ����� ��������� ������
	UINT text_length;

	// ��������� ��������� � ������
	UINT text_caret_pos;

	// ��������� ���������� ����
	int text_x;
	int text_y;

	// ������������ �����
	UINT max_text_length;

	// shift
	bool shift;

public:
	char* GetText();
	Font* GetTextFont();
	COLOR4 GetTextColor();
	UINT GetTextCaretPos();
	int GetTextX();
	int GetTextY();
	bool GetShift();
	UINT GetTextLength();
	UINT GetMaxTextLength();

	void SetText(char* new_text);
	void SetTextFont(Font* new_text_font);
	void SetTextColor(COLOR4 new_text_color);
	void SetTextCaretPos(UINT new_text_caret_pos);
	void SetTextX(int new_text_x);
	void SetTextY(int new_text_y);
	void SetShift(bool new_shift);
	void SetTextLength(UINT new_text_length);
	void SetMaxTextLength(UINT new_max_text_length);

	void Draw();
	void OnKeyDown(UCHAR key, UCHAR ascii);
	void OnKeyUp(UCHAR key);

	// �����������
	GUITextBox();
	GUITextBox(string name);
};
// --------------------------------------------------------------------------