typedef struct tagCONFIG {
	int scr_width;
	int scr_height;
	int scr_bpp;
	bool fullscreen;
	bool vert_sync;
	bool anti_alias;
	bool debug;

	bool LoadFromFile(char file_name[]);
	bool SaveToFile(char file_name[]);
} config;