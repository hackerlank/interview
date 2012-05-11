// Класс текстовой надписи
// GUILabel (GUIVidget)
// Примечание:
//	
// --------------------------------------------------------------------------



class GUILabel : public GUIControl
{
protected:
	// Надпись
	char* text;

	// Шрифт
	Font* font;

	// Позиция X надписи
	int text_x;

	// Позиция Y надписи
	int text_y;

	// Цвет надписи
	COLOR4 caption_color;

public:
	char* GetText();
	Font* GetFont();
	int GetTextX();
	int GetTextY();
	COLOR4 GetCaptionColor();

	void SetText(char* new_text);
	void SetFont(Font* new_font);
	void SetTextX(int new_text_x);
	void SetTextY(int new_text_y);
	void SetCaptionColor(COLOR4 new_caption_color);

	// Центрировать текст
	void CentrifyText();

	// Конструктор
	GUILabel(const char* name);
};
// --------------------------------------------------------------------------