

class sprite {
public:
	sprite(GLuint tex_id);
	~sprite();

	int texture_id;
	UINT image_width;
	UINT image_height;
	int posX;
	int posY;

	void DrawAt(int, int);
};

class sprite;
int LoadTexture(texture* dest, char* path);
AUX_RGBImageRec* GetTexture(char *file_name);

//AUX_RGBImageRec *texture1;
//     texture1 = auxDIBImageLoad("Data/NeHe.bmp");
//glGenTextures(1, &texture[0]);
//     glBindTexture(GL_TEXTURE_2D, texture[0]);
