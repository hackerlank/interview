typedef struct tagCONFIG {
	unsigned int scr_width;
	unsigned int scr_height;
	unsigned int scr_bpp;
	bool fullscreen;
	bool vert_sync;
	bool anti_alias;
	bool debug;
	bool force_start;

	
	int left;
	int right;
	int down;
	int jump;
	int fire;

	int bullet_speed;
	int bullet_life;
	int bullet_damage;
	int hero_speed;
	int hero_jump_height;
	int hero_reload;

	int enemy_speed;

} config;



bool LoadConfig(char* file_name, config* cfg);
void SetConfigField(config* cfg, string field, string value);