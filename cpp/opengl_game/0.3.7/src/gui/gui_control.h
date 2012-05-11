// Базовый класс объектов интерфейса GUI
// GUIControl (GUIVidget)
// Примечание:
//	Реагирует на события клавиатуры и мыши
// --------------------------------------------------------------------------
class GUIControl : public GUIVidget
{
protected:
	// Количество возможных состояний (текстура)
	UINT pattern_count;

	// Текущее состояние (текстура)
	UINT current_pattern;

	// Доступен или нет
	bool enabled;

	// Заголовок
	char* caption;

	// Шрифт заголовка
	font_t* caption_font;

	// Цвет заголовка
	COLOR4 caption_color;
	COLOR4 selected_caption_color;
	COLOR4 pressed_caption_color;
	COLOR4 current_caption_color;

	// Y-координата заголовка
	int caption_x;

	// X-координата заголовка
	int caption_y;

	// Где в последний раз находился курсор
	int last_cursor_pos;

	// Последнее состояние нажатия
	int last_press_state;

	// Последнее отправленное сообщение
	int last_sent_msg;

	// Данные которые отсылает кнопка при нажатии
	// Затем обрабатывается MessageAllocator'ом
	vector<GUISignal> sgns;

	// Последняя нажатая клавиша
	int last_pressed_key;

	// Последнее обновление нажатий
	int prev_key_update;

	void SetLastCursorPos(int new_last_cursor_pos);
	void SetLastPressState(int new_last_press_state);
	void SetLastSentMsg(int new_last_sent_msg);
	void SetLastPressedKey(UCHAR new_last_pressed_key);

	int GetLastCursorPos();
	int GetLastPressState();
	int GetLastSentMsg();
	UCHAR GetLastPressedKey();

public:
	UINT GetPatternCount();
	UINT GetCurrentPattern();
	bool IsEnabled();
	char* GetCaption();
	font_t* GetCaptionFont();
	COLOR4 GetCaptionColor();
	COLOR4 GetSelectedCaptionColor();
	COLOR4 GetPressedCaptionColor();
	COLOR4 GetCurrentCaptionColor();
	int GetCaptionX();
	int GetCaptionY();


	void SetPatternCount(UINT new_pattern_count);
	void SetCurrentPattern(UINT new_current_pattern);
	void SetEnabled(bool new_enabled);
	void SetCaption(char* new_caption);
	void SetCaptionFont(font_t* new_caption_font);
	void SetCaptionColor(COLOR4 new_caption_color);
	void SetSelectedCaptionColor(COLOR4 new_selected_caption_color);
	void SetPressedCaptionColor(COLOR4 new_pressed_caption_color);
	void SetCurrentCaptionColor(COLOR4 new_current_caption_color);
	void SetCaptionX(int new_caption_x);
	void SetCaptionY(int new_caption_y);
	void AddSgn(UINT sgn, UINT param_1, UINT param_2);

	virtual void SendMessage(GUIMessage msg);
	virtual void OnMouseMove();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnMouseDown();
	virtual void OnMouseUp();
	virtual void OnLeftClick();
	virtual void OnRightClick();
	virtual void OnKeyDown(UCHAR key);
	virtual void OnKeyUp(UCHAR key);


	virtual void Draw();
	virtual void Process();

	// Для логики, рисовать или нет (см. gui_msgalloc)
	bool _draw_border;

	// Конструктор
	GUIControl();
	//GUIControl(char* name);
	GUIControl(string name);
};
// --------------------------------------------------------------------------