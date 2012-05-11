#include "StdAfx.h"

#include "../misc.h"
#include "font.h"

#define BASE_COUNT 256		// Количество символов

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Font* fonts = NULL;				// Указатель на массив загруженных шрифтов
UINT font_count = 0;			// Количество загруженных шрифтов





extern HDC hDC;

// Загрузка шрифта Windows
// in_name - имя шрифта (например 'Courier New')
// height - высота
// weight (400 - NORMAL, 700 - BOLD, и т. д...)
// out_name - шрифт будет использоваться в игре под таким именем
bool LoadWindowsFont(const char* in_name, BYTE height, long weight, const char* out_name)
{

	if(!in_name || !out_name)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Incorrect in_name or out_name for loading windows font");
		return false;
	}

	Font* font = NULL;
	font = new Font;
	memset(font, '\0', sizeof(Font));

	font->font = CreateFont(-height, 0,	0, 0, weight, 0, 0, 0,
		DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_DONTCARE || DEFAULT_PITCH, in_name);

	if(!font->font)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to create font");
		return false;
	}

	font->height = height;
	font->weight = weight;
	font->name = new char[strlen(out_name) + 1];
	memset(font->name, '\0', strlen(out_name) + 1);
	strcpy(font->name, out_name);

	SelectObject(hDC, font->font);
	font->base = glGenLists(BASE_COUNT);
	wglUseFontBitmaps(hDC, 0, BASE_COUNT, font->base);


	font_count++;

	/*if(fonts == NULL)
		fonts = (Font*)malloc(1 * sizeof(Font));
	else*/
	fonts = (Font*)realloc(fonts, font_count * sizeof(Font));

	memcpy(&fonts[font_count - 1], font, sizeof(Font));
	delete font;

	return true;
}



// Освободить указанный шрифт
void FreeFont(Font* font)
{
	if(!font)
		return;

	Font* old_memory = fonts;
	Font* new_memory = NULL;
	if (font_count > 1)
	{
		new_memory = (Font*)malloc((font_count - 1) * sizeof(Font));
		memset(new_memory, '\0', (font_count - 1) * sizeof(Font));

		UINT j = 0;
		for(UINT i = 0; i < font_count; i++)
		{
			if(&old_memory[i] != font)
			{
				memcpy(&new_memory[j], &old_memory[i], sizeof(Font));
				j++;
			}
		}
	}
	font_count--;

	// free font
	if(font->name)
		delete[] font->name;

	// free object
	DeleteObject(font->font);

	// free gl-list
	glDeleteLists(font->base, BASE_COUNT);

	// now free old memory
	free(old_memory);

	fonts = new_memory;
}



void FreeFont(const char* font_name)
{
	// just find font by name and free it, using FS_FreeFont(font_t*)

	FreeFont(FontByName(font_name));
}



// free all fonts
void FreeFonts(void)
{
	// TODO: Медленно же!!111 Хотя шрифтов мало, так что похуй...
	for(;;)
	{
		if(font_count > 0)
			FreeFont(&fonts[font_count-1]);
		else
			break;
	}
}



// get font by name
Font* FontByName(const char* font_name)
{
	for(UINT i = 0; i < font_count; i++)
	{
		if(strcmp(fonts[i].name, font_name) == 0)
		{
			return &fonts[i];
		}
	}

	return NULL;
}

//-------------------------------------------//
//				Font stuff					 //
//-------------------------------------------//
CFont::CFont()
{
	FontImageName = NULL;
	base = 0;
	dist = CFONT_DEFAULT_DISTANCE;
	pp = &p;
	offset = 0;
	isSelected = isRect = false;
	s1 = s2 =0;
	tClr = RGBAf(1.0f, 1.0f, 1.0f, 1.0f);
	SetDepth(0.0f);
}

CFont::~CFont()
{
	if (base != 0)
		glDeleteLists(base, 256);
	if (FontImageName != NULL)
		delete [] FontImageName;
}


bool CFont::LoadFromFile()
{
	if (filename == "")
		return false;
	CFile			file;
	Texture	*image;
	if (!file.Open(filename.c_str(), CFILE_READ))
	{
		sLog(DEFAULT_LOG_NAME, "WARNING","Can't Load Font %s: file  couldn't be opened.", name.data()); //TODO: filename wrte too.
		return false;
	}

	file.ReadLine(FontImageName);	
	image = TextureByName(FontImageName);
	font = image->tex;
	file.Read(bbox, sizeof(bbox));
	file.Close();
	base = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, font);
	for(int i=0;i<256;i++)
	{
		glNewList(base+i, GL_COMPILE);
		glBegin(GL_QUADS);		
		glTexCoord2f( (float)bbox[i].x0/image->width, (float)bbox[i].y0/image->height);
		glVertex2i(0, 0);

		glTexCoord2f( (float)bbox[i].x1/image->width, (float)bbox[i].y0/image->height);
		glVertex2i(bbox[i].x1 - bbox[i].x0, 0);

		glTexCoord2f( (float)bbox[i].x1/image->width, (float)bbox[i].y1/image->height);
		glVertex2i(bbox[i].x1 - bbox[i].x0, bbox[i].y1 - bbox[i].y0);

		glTexCoord2f( (float)bbox[i].x0/image->width, (float)bbox[i].y1/image->height);
		glVertex2i(0, bbox[i].y1 - bbox[i].y0);
		glEnd();
		glTranslated(bbox[i].x1 - bbox[i].x0 + dist, 0, 0);
		glEndList();
		width[i] = (bbox[i].x1 - bbox[i].x0);
		height[i] = (bbox[i].y1 - bbox[i].y0);
	}
	return true;
}

bool CFont::SaveToFile()
{
	if (filename == "")
		return false;
	CFile			file;
	file.Open(filename.c_str(), CFILE_WRITE);
	file.Write(FontImageName, (DWORD)strlen(FontImageName));
	file.WriteByte((byte)0x00);
	file.Write(bbox, sizeof(bbox));
	file.Close();
	return true;
}

void CFont::_Print(const char *text)
{
	glPushAttrib(GL_TEXTURE_BIT);
	glPushMatrix();
	glColor4fv(&(tClr.r));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font);
	glTranslatef(pp->x, pp->y, depth);
	glListBase(base-32);
	glCallLists((GLsizei)strlen(text), GL_BYTE, text);
	glPopMatrix();
	glPopAttrib();
}


typedef unsigned			uint;

void CFont::Print( const char *text, ... )
{
	if (text == NULL)
		return;
	if ((uint)offset >= strlen(text) || offset < 0)
		return;

	int swidth = 0, sheight = 0, selx = 0;
	if (isSelected)
	{
		if (s1 > s2+1)
		{
			swap(s1, s2);
			s1++;
			s2--;
		}

		swidth = GetStringWidth(text + offset);
		sheight = GetStringHeight(text + offset);
		selx = p.x + GetStringWidthEx(0, s1-1-offset, text+offset);


		if (!(s1 > s2 || s1 < 0 || (uint)s2 >= strlen(text)))
		{
			int selw = GetStringWidthEx(max(s1, offset), s2, text);
			CPrimitiveRender pr;
			pr.psClr = &RGBAf(10, 50, 200, 150);
			pr.depth = depth;
			pr.grRectS(Vector2(selx, p.y), Vector2(selx + selw, p.y + sheight));
		}


	}

	int xpos, ypos;
	if (isRect)
	{

		byte tempV = align & CFONT_VALIGN_MASK, tempH = align & CFONT_HALIGN_MASK;

		if (tempV == CFONT_VALIGN_TOP)
		{
			ypos = p.y + wh.y - sheight;
		}
		if (tempV == CFONT_VALIGN_BOTTOM)
		{
			ypos = p.y;
		}
		if (tempV == CFONT_VALIGN_CENTER)
		{
			ypos = p.y + wh.y/2 - sheight/2;
		}

		if (tempH == CFONT_HALIGN_LEFT)
		{
			xpos = p.x;
		}
		if (tempH == CFONT_HALIGN_RIGHT)
		{
			xpos = p.x + wh.x - swidth;
		}

		if (tempH == CFONT_HALIGN_CENTER)
		{
			xpos = p.x + wh.x/2 - swidth/2;
		}

		glPushAttrib(GL_SCISSOR_TEST);
		glEnable(GL_SCISSOR_TEST);
		glScissor(p.x, p.y, wh.x, wh.y);


		CPrimitiveRender pr;
		pr.depth = depth;
		pr.lClr = RGBAf(0.4f, 0.5f, 0.7f, 0.9f);
		pr.sClr = RGBAf(0.5f, 0.7f, 0.4f, 0.9f);
		pr.pClr = RGBAf(0.7f, 0.5f, 0.4f, 0.9f);

		pr.grRectS(Vector2(selx - 1, ypos), Vector2(selx - 1 + 1, ypos + min(sheight, GetStringHeight("!\0")))); //  glRGBAub(10, 10, 10, 200)
		pr.grRectS(Vector2(selx - 3, ypos), Vector2(selx - 3 + 5, ypos + 1)); // glRGBAub(10, 10, 10, 200)
		//gSolidRectEx(selx - 2, ypos, 5, 1, depth, &glRGBAub(10, 10, 10, 200));
		pr.grRectS(Vector2(selx - 3, ypos + min(sheight, GetStringHeight("!\0"))), Vector2(selx - 3 + 5,ypos + min(sheight, GetStringHeight("!\0"))+ 1)); // glRGBAub(10, 10, 10, 200)

	}


	static char	*temp;//[CFONT_MAX_STRING_LENGTH];
	temp = new char [strlen(text)+CFONT_MAX_STRING_LENGTH];
	va_list	ap;
	va_start(ap, text);
	vsprintf(temp, text, ap);
	va_end(ap);
	_Print(temp);
	delete [] temp;

	if (isRect)
	{
		glDisable(GL_SCISSOR_TEST);
		glPopAttrib();
	}
}


int	CFont::GetStringWidth(const char *text)
{
	if (text == NULL)
		return -1;
	int r = 0, l = (int)strlen(text);
	for (int i=0;i<l;i++)
		r += width[text[i]-32] + dist;
	return r;
}

int CFont::GetStringWidthEx(int t1, int t2, const char *text)
{
	if (text == NULL)
		return -1;
	if (t1 > t2 || t2 < 0)
		return -1;
	if ((uint)t2 >= strlen(text))
		return -1;
	int r = 0;
	for (uint i = t1; i <= t2; i++)
		r += width[text[i]-32]+dist;
	return r;
}

int	CFont::GetStringHeight(const char *text)
{
	if (text == NULL)
		return -1;
	int r = 0, l = (uint)strlen(text);
	for (int i=0;i<l;i++)
		r = Max(height[text[i]-32], r);
	return r;
}

int CFont::GetStringHeightEx( int t1, int t2, const char *text )
{
	if (text == NULL)
		return -1;
	if (t1 > t2 || t2 < 0)
		return -1;
	if ((uint)t2 >= strlen(text))
		return -1;
	int r = 0, l = (uint)strlen(text);
	for (uint i=0; i<l; i++)
		r = Max(height[text[i]-32], r);
	return r;
}

void CFont::SetDepth( float _depth )
{
	depth = _depth;
	depth = clampf(depth, CFONT_DEPTH_LOW, CFONT_DEPTH_HIGH);
}

void CFont::PointTo(const Vector2 *_p)
{
	if (!_p)
		return;
	pp = _p;
}

void CFont::PointBack()
{
	pp = &p;
}

void CFont::SetAlign( const byte _Halign, const byte _Valign )
{
	align = _Valign || _Halign;
}

byte CFont::GetHalign()
{
	return align & CFONT_HALIGN_MASK;
}

byte CFont::GetValign()
{
	return align & CFONT_VALIGN_MASK;
}
