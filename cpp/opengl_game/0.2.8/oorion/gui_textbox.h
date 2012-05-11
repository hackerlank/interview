// ����� ����������
// GUITextBox (GUIControl)
// ����������:
//	������ ������
// --------------------------------------------------------------------------
class GUITextBox : public GUIControl
{
protected:
	// �����, ��������� � ����
	//char text[GUI_TEXTBOX_TEXT_LENGTH];
	char* text;
	//string text;

	// ����� ���������� ����
	font_t* text_font;

	// ���� ���������� ����
	COLOR4 text_color;

	// ����� ��������� ������
	UINT text_length;

	// ��������� ��������� � ������
	UINT text_caret_pos;

	// ��������� ���������� ����
	int text_x;
	int text_y;

	// shift
	bool shift;

public:
	char* GetText();
	//string GetText();
	font_t* GetTextFont();
	COLOR4 GetTextColor();
	UINT GetTextCaretPos();
	int GetTextX();
	int GetTextY();
	bool GetShift();

	void SetText(char* new_text);
	//void SetText(string new_text);
	void SetTextFont(font_t* new_text_font);
	void SetTextColor(COLOR4 new_text_color);
	void SetTextCaretPos(UINT new_text_caret_pos);
	void SetTextX(int new_text_x);
	void SetTextY(int new_text_y);
	void SetShift(bool new_shift);

	void Draw();
	void OnKeyDown(UCHAR key);
	void OnKeyUp(UCHAR key);

	// �����������
	GUITextBox();
	GUITextBox(string name);
};
// --------------------------------------------------------------------------