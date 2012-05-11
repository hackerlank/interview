// Класс радиобаттона
// GUIRadioButton (GUIControl)
// Примечание:
//	Только один радиобаттон из группы может быть включен
// --------------------------------------------------------------------------
class GUIRadioButton : public GUIControl
{
protected:
	// состояние
	bool checked;

	// Надпись на радиобаттоне
	char* caption;

	// Шрифт надписи
	font_t* caption_font;

	// Цвет надписи
	COLOR4 caption_color;

	// Группа к которой принадлежит радиобаттон
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

	// Конструктор
	GUIRadioButton(const char* name);
};
// --------------------------------------------------------------------------