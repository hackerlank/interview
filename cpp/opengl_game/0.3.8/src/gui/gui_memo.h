#include "../misc.h"

// Класс текстового поля
// GUIMemo (GUIControl)
// Примечание:
//	Большое поле для ввода
// --------------------------------------------------------------------------
class GUIMemo : public GUIControl
{
protected:
	// строки
	vector<string> lines;

	// Шрифт строк
	Font* lines_font;

	// Цвет строк
	COLOR4 lines_color;

	// Положение каретки
	Point caret_pos;

	// Отступ первой строки сверху
	int dy;

	// Отступ строк слева
	int dx;

	// Расстояние между строками
	int lines_dy;

	// Редактируемый или нет
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

	// Конструктор
	GUIMemo();
	GUIMemo(string name);
};
// --------------------------------------------------------------------------