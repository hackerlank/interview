#ifndef __RESOURCE_H_
#define __RESOURCE_H_

enum RES_TYPE { resSprite, resAnimSprite, resCharacter, resNone };

typedef struct _OBJECT_RES {
	char* name;
	RES_TYPE type;
	char* texture;
	int real_x;
	int real_y;
	int real_mirror_x;
	int real_mirror_y;
	int real_width;
	int real_height;
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

	int z;

	int current_pattern;

	int jump_down_pattern;

	int animation_speed;

	// here we'll add new actions
} OBJECT_RES;

const OBJECT_RES NULL_RES = {
	"",
	resNone,
	"",
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};


bool LoadResource(const char* file_name);

OBJECT_RES* ResourceByName(char* name);

#endif