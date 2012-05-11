// Класс кнопки
// GUIButton (GUIControl)
// Примечание:
//	Просто кнопка
// --------------------------------------------------------------------------
class GUIButton : public GUIControl
{
public:
	void OnLeftClick();
	void OnMouseMove();

	// Конструктор
	GUIButton();
	GUIButton(string name);
};
// --------------------------------------------------------------------------