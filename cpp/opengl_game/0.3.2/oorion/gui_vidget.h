// Базовый класс отображаемых объектов GUI
// GUIVidget (GUIBaseControl)
// Примечание:
//	
// --------------------------------------------------------------------------
class GUIVidget : public GUIBaseControl
{
protected:
	// Координата X
	int x;

	// Координата Y
	int y;

	// Ширина контрола
	UINT width;

	// Высота контрола
	UINT height;

	// Текстура контрола
	texture_t* texture;

	// Цвет контрола
	COLOR4 color;

public:
	int GetX();
	int GetY();
	UINT GetWidth();
	UINT GetHeight();
	texture_t* GetTexture();
	COLOR4 GetColor();

	virtual void SetX(int new_x);
	virtual void SetY(int new_y);
	virtual void SetWidth(UINT new_width);
	virtual void SetHeight(UINT new_height);
	virtual void SetTexture(texture_t* new_texture);
	virtual void SetColor(COLOR4 new_color);

	virtual void Draw();

	// Конструктор
	GUIVidget();
	//GUIVidget(char* name);
	GUIVidget(string name);
};
// --------------------------------------------------------------------------