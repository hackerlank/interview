typedef struct tagCONFIG {
	unsigned int scr_width;
	unsigned int scr_height;
	unsigned int scr_bpp;
	bool fullscreen;
	bool vert_sync;
	bool anti_alias;
	bool debug;

	//bool LoadFromFile(char file_name[]);
	//bool SaveToFile(char file_name[]);
} config;

bool LoadConfig(char* file_name, config* cfg);
void SetConfigField(config* cfg, string field, string value);