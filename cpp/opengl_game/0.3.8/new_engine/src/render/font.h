#ifndef __FONT_H_
#define __FONT_H_

#include "renderer.h"

struct Font{
	UINT base;
	char* name;
	BYTE height;
	long weight;
	HFONT font;
	//texture_t* texture;
	Font()
	{
		base = -1;
		name = "";
		height = 0;
		weight = 0;
		font = (HFONT)INVALID_HANDLE_VALUE;
	}
};

bool LoadWindowsFont(const char* in_name, BYTE height, long weight, const char* out_name);
void FreeFont(Font* font);
void FreeFont(const char* font_name);
void FreeFonts(void);
Font* FontByName(const char* font_name);

//Satana ��������� � ���� ����

struct CRecti
{
	int x0, x1, y0, y1;
};


#define CFONT_DEFAULT_DISTANCE	1
#define CFONT_VALIGN_TOP		0x00
#define CFONT_VALIGN_BOTTOM		0x01
#define CFONT_VALIGN_CENTER		0x02
#define CFONT_VALIGN_MASK		0x0f
#define CFONT_HALIGN_LEFT		0x00
#define CFONT_HALIGN_RIGHT		0x10
#define CFONT_HALIGN_CENTER		0x20
#define CFONT_HALIGN_MASK		0xf0


#define CFONT_DEPTH_HIGH		1.0f
#define CFONT_DEPTH_LOW			0.0f
#define CFONT_MAX_STRING_LENGTH	256

class CFont
{
public:
	CFont();
	~CFont();

	// �� CObject
	unsigned int type, id;	// type - ����� ������� �������. id - ����������������� ����� �������. ���� �� ������������. TODO!
	string name;			// name - ��� �������. ������ ���������� � �������� �� ������. � � ��� ������ ������.

	// �� CResource
	bool loaded;		// loaded ������ ���� ������ ���� ��������� ������� ��� ������� ��������, � �� ������ ���������������.
	string filename;	// ������^W ����-�����-������ ���� � �����.

	RGBAf				tClr;					//	����.
	byte				dist;					//	���������� ����� ���������		
	Vector2				p;						//	��������� ������, ��� ������������, ��������� �� ������� ��������� �� ���
	Vector2				wh;						//	������ � ������� � ������� ����-��. ��� ��� ������. x - w, y - h
	int					offset;					//	��������, � �������� ���� �������� � �����, ���� �� ������� �� ���������
	int					s1, s2;					//	������ ������� � ���������� �������� ����������� ������
	bool				isRect;					//	���� �� �������� ����� � �������������
	bool				isSelected;				//	������� �� ����� ������
	bool				doUseGlobalCoordSystem;	//	������������ �� ��� ������ ���������� ������� ��������	
	CPrimitiveRender	Prndr;					//	��������� ����� � ������ ����� �������� � ������.

	bool		LoadFromFile();
	bool		SaveToFile();

	void		Print(const char *text, ...);

	void		SetDepth(float _depth);
	void		PointTo(const Vector2 *_p);
	void		PointBack();
	void		SetAlign(const byte _Halign, const byte _Valign);

	int			GetStringWidth(const char *text);
	int			GetStringWidthEx(int t1, int t2, const char *text);
	int			GetStringHeight(const char *text);
	int			GetStringHeightEx(int t1, int t2, const char *text);

private:
	float				x, y;					//	����������� ���������� ��� ��������� � _Print
	float				depth;					//	������� �� Z
	(const Vector2)		*pp;					//	��������� �� ������ � ������������ ������
	byte				align;					//	����� ������������

	CRecti		bbox[256];		// �������� ���� ������� ��� ������� �������
	byte		width[256];		// ������ ������� �������
	byte		height[256];	// ������ ������� �������
	char		*FontImageName;	// ��� ����� ��������
	GLuint		font;			// Font texture ID
	GLuint		base;			// Base List of 256 glLists for font

	void		_Print(const char *text);
	byte		GetHalign();
	byte		GetValign();
};


#endif // __FONT_H_