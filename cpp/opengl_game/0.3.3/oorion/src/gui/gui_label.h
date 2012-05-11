// ����� ��������� �������
// GUILabel (GUIVidget)
// ����������:
//	
// --------------------------------------------------------------------------



class GUILabel : public GUIControl
{
protected:
	// �������
	char* text;

	// �����
	font_t* font;

	// ������� X �������
	int text_x;

	// ������� Y �������
	int text_y;

	// ���� �������
	COLOR4 caption_color;

public:
	char* GetText();
	font_t* GetFont();
	int GetTextX();
	int GetTextY();
	COLOR4 GetCaptionColor();

	void SetText(char* new_text);
	void SetFont(font_t* new_font);
	void SetTextX(int new_text_x);
	void SetTextY(int new_text_y);
	void SetCaptionColor(COLOR4 new_caption_color);

	// ������������ �����
	void CentrifyText();

	// �����������
	GUILabel(const char* name);
};
// --------------------------------------------------------------------------