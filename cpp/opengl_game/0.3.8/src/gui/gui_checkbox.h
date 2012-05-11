// ����� ��������
// GUIButton (GUIControl)
// ����������:
//	����� ��� ��������� - �������/��������
// --------------------------------------------------------------------------
class GUICheckBox : public GUIControl
{
protected:
	// ���������
	bool checked;

	// ������� �� ��������
	char* caption;

	// ����� �������
	Font* caption_font;

	// ���� �������
	COLOR4 caption_color;

public:
	bool IsChecked();
	char* GetCaption();
	Font* GetCaptionFont();
	COLOR4 GetCaptionColor();

	void SetChecked(bool new_checked);
	void SetCaption(char* new_caption);
	void SetCaptionFont(Font* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);

	// �����������
	GUICheckBox(const char* name);
};
// --------------------------------------------------------------------------