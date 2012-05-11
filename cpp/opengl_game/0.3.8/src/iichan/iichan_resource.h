#ifndef __RESOURCE_H_
#define __RESOURCE_H_

//enum RES_TYPE { resNone, resSprite, resAnimSprite, resCharacter};
enum RES_BEH { resNullBeh, resPlayer, resEnemy, resPowerup, resSprite, resAnimSprite };
struct RESOURCE {
	//char* name;
	string name;
	//RES_TYPE type;
	RES_BEH behaviour;

	int enemy_type;
	bool enemy_squashable;
	bool enemy_attack_collide;

	int reload_speed;
	int attack_range;
	int damage;

	string texture;
	string shadow_texture;

	int real_x;
	int real_y;
	int real_mirror_x;
	int real_mirror_y;
	int real_width;
	int real_height;
	
	int real_shoot_x;
	int real_shoot_y;
	int real_walk_x;
	int real_walk_y;
	int real_stand_x;
	int real_stand_y;
	int real_sit_x;
	int real_sit_y;
	int real_jump_x;
	int real_jump_y;

	int real_shoot_mirror_x;
	int real_shoot_mirror_y;
	int real_walk_mirror_x;
	int real_walk_mirror_y;
	int real_stand_mirror_x;
	int real_stand_mirror_y;
	int real_sit_mirror_x;
	int real_sit_mirror_y;
	int real_jump_mirror_x;
	int real_jump_mirror_y;

	int real_shoot_width;
	int real_shoot_height;
	int real_walk_width;
	int real_walk_height;
	int real_stand_width;
	int real_stand_height;
	int real_sit_width;
	int real_sit_height;
	int real_jump_width;
	int real_jump_height;
	

	int pattern_width;
	int pattern_height;
	int real_pattern_count;
	int pattern_count;
	int start_pattern;

	int pattern_x;
	int pattern_y;

	int image_width;
	int image_height;

	int shoot_pattern;
	int shoot_count;
	int shoot_speed;
	int shoot_attack_pattern;
	string bullet_type;
	string shoot_sound;

	int walk_pattern;
	int walk_count;
	int walk_speed;

	int jump_pattern;
	int jump_count;
	int jump_speed;

	int idle_pattern;
	int idle_count;
	int idle_speed;

	int land_pattern;
	int land_count;
	int land_speed;

	int stand_pattern;
	int stand_count;
	int stand_speed;

	int sit_pattern;
	int sit_count;
	int sit_speed;

	int sit_shoot_pattern;
	int sit_shoot_count;
	int sit_shoot_speed;

	int damage_pattern;
	int damage_count;
	int damage_speed;

	int die_pattern;
	int die_count;
	int die_speed;

	int shadow_x;
	int shadow_y;
	int shadow_mirror_x;
	int shadow_mirror_y;

	int flash_jump_x;
	int flash_jump_y;
	int flash_sit_x;
	int flash_sit_y;
	int flash_walk_x;
	int flash_walk_y;
	int flash_stand_x;
	int flash_stand_y;

	int flash_jump_mirror_x;
	int flash_jump_mirror_y;
	int flash_walk_mirror_x;
	int flash_walk_mirror_y;
	int flash_sit_mirror_x;
	int flash_sit_mirror_y;
	int flash_stand_mirror_x;
	int flash_stand_mirror_y;

	float speed_x;				// устаревшее
	float speed_y;				// устаревшее
	
	float phys_walk_vel;
	float phys_jump_speed;
	float phys_max_x_speed;
	float phys_max_y_speed;
	int phys_collidable;

	int bullet_collidable;

	float blend;

	int z;

	int current_pattern;

	int jump_down_pattern;

	int animation_speed;

};

bool LoadResource(const char* file_name);

RESOURCE* ResourceByName(const char* name);

#endif
