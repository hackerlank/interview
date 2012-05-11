// ����� ������������
// GUIRadioButton (GUIControl)
// ����������:
//	������ ���� ����������� �� ������ ����� ���� �������
// --------------------------------------------------------------------------
class GUIRadioButton : public GUIControl
{
protected:
	// ���������
	bool checked;

	// ������� �� ������������
	char* caption;

	// ����� �������
	font_t* caption_font;

	// ���� �������
	COLOR4 caption_color;

	// ������ � ������� ����������� �����������
	UINT radio_group;

public:
	bool IsChecked();
	char* GetCaption();
	font_t* GetCaptionFont();
	COLOR4 GetCaptionColor();
	UINT GetRadioGroup();

	void SetChecked(bool new_checked);
	void SetCaption(char* new_caption);
	void SetCaptionFont(font_t* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);
	void SetRadioGroup(UINT new_radio_group);

	// �����������
	GUIRadioButton(const char* name);
};
// --------------------------------------------------------------------------