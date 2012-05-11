enum BORDER_TYPE { btAlways, btOnMouseMove, btNone };


// ������� ����� ������������ �������� GUI
// GUIVidget (GUIBaseControl)
// ����������:
//	
// --------------------------------------------------------------------------
class GUIVidget : public GUIBaseControl
{
protected:
	// ���������� X
	int x;

	// ���������� Y
	int y;

	// ������ ��������
	UINT width;

	// ������ ��������
	UINT height;

	// �������� ��������
	Texture* texture;

	// ���� ��������
	COLOR4 color;

	bool border;

	BORDER_TYPE border_type;

public:
	int GetX() const;
	int GetY() const;
	UINT GetWidth() const;
	UINT GetHeight() const;
	Texture* GetTexture() const;
	COLOR4 GetColor() const;
	bool GetBorder() const;
	BORDER_TYPE GetBorderType() const;

	virtual void SetX(int new_x);
	virtual void SetY(int new_y);
	virtual void SetWidth(UINT new_width);
	virtual void SetHeight(UINT new_height);
	virtual void SetTexture(Texture* new_texture);
	virtual void SetColor(COLOR4 new_color);
	virtual void SetBorder(bool new_value);
	virtual void SetBorderType(BORDER_TYPE new_value);

	virtual void Draw();

	// �����������
	GUIVidget();
	//GUIVidget(char* name);
	GUIVidget(string name);
};
// --------------------------------------------------------------------------