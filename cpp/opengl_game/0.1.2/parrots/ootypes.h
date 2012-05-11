typedef struct tagTEXTURE {
		GLuint image;
		char* image_name;
	} texture;
typedef struct tagFONT {
		GLuint data;
		char* name;
		UCHAR height;
	} font;

typedef struct tagOSTLINE {
		char* line;
		UINT x;
		UINT y;
		UCHAR red;
		UCHAR green;
		UCHAR blue;
		UCHAR alpha;
		char* font_name;
	} ostline;




	
