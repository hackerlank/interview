#ifndef __CONFIG_H_
#define __CONFIG_H_

typedef struct tagCONFIG {
	int scr_width;
	int scr_height;
	int scr_bpp;

	int window_width;
	int window_height;

	bool fullscreen;
	bool vert_sync;
	bool anti_alias;
	bool debug;
	bool show_fps;
	bool force_start;

	
	int left;
	int right;
	int down;
	int jump;
	int fire;
	int screenshot;

	int bullet_speed;
	int bullet_damage;
	int hero_speed;
	int hero_reload;
	int hero_vuln;

	int enemy_speed;

	float gravity;

} config;

bool LoadConfig(const char* file_name, config* cfg);
void SetConfigField(config* cfg, string field, string value);

#endif