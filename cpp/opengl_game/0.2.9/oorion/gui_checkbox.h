// Класс чекбокса
// GUIButton (GUIControl)
// Примечание:
//	Имеет два состояния - включен/выключен
// --------------------------------------------------------------------------
class GUICheckBox : public GUIControl
{
protected:
	// состояние
	bool checked;

	// Надпись на чекбоксе
	char* caption;

	// Шрифт надписи
	font_t* caption_font;

	// Цвет надписи
	COLOR4 caption_color;

public:
	bool IsChecked();
	char* GetCaption();
	font_t* GetCaptionFont();
	COLOR4 GetCaptionColor();

	void SetChecked(bool new_checked);
	void SetCaption(char* new_caption);
	void SetCaptionFont(font_t* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);

	// Конструктор
	GUICheckBox(const char* name);
};
// --------------------------------------------------------------------------