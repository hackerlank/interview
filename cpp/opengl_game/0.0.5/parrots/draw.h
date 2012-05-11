


void PutPixel(UINT Xcoord, UINT Ycoord, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void TextOut(string text, font_t* font, HDC* hDC, UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void Circle(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius, float width, bool antialias);
//void CircleAnim(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius, float width, bool antialias);
void Box(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void Box2(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
		  UCHAR red2, UCHAR green2, UCHAR blue2, UCHAR alpha2);
void FilledBox(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void FilledBox2(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
				UCHAR red2, UCHAR green2, UCHAR blue2, UCHAR alpha2);
//void WUCircle(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius);

void TextOutD(string text, font_t* font, HDC* hDC, UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
			  float speed);

