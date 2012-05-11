#ifndef __PROTO_H_
#define __PROTO_H_


enum PROTO_BEH { protoNullBeh, protoPlayer, protoEnemy, protoPowerup, protoSprite };

struct AnimProto
{
	char* anim_name;
	
	UINT frame_count;
	UINT start_frame;
	//UINT frame_x;
	//UINT frame_y;

	UINT anim_speed;

	UINT real_x;
	UINT real_y;
	UINT real_mirror_x;
	UINT real_mirror_y;
	UINT real_width;
	UINT real_height;
	AnimProto()
	{
		frame_count = start_frame = real_x = real_y = real_mirror_y = real_mirror_x = real_width = real_height = 0;
		anim_name = NULL;
	}
};

struct Proto 
{
	//char* name;
	PROTO_BEH behaviour;
	char* texture;
	float z;
	UINT image_width;
	UINT image_height;
	UINT frame_x;
	UINT frame_y;
	UINT frame_widht;
	UINT frame_height;
	UINT frames_count;
	
	vector<AnimProto*> animations;

	float phys_walk_acc;
	float phys_jump_vel;
	float phys_max_x_vel;
	float phys_max_y_vel;
	UINT phys_solid;
	UINT phys_bullet_collidable;
	UINT physic;

	UINT health;
	char* main_weapon;
	char* alt_weapon;

	float bullet_vel;
	UINT bullet_damage;

	UINT reload_time;
	int bullets_count;

	Proto ()
	{
		behaviour = protoNullBeh;
		texture = NULL;

		z = image_height = image_width = frame_x = frame_y = 
			frame_widht = frame_height = frames_count = 
			phys_solid = phys_bullet_collidable = physic = 0;

		phys_walk_acc = phys_jump_vel = phys_max_x_vel = phys_max_y_vel = 0.0f;

		health = 0;
		main_weapon = NULL;
		alt_weapon = NULL;

		bullet_damage = 0;
		bullet_vel = 0;
		reload_time = 0;
		bullets_count = 0;
	}

	~Proto()
	{
		for(vector<AnimProto*>::iterator ait = this->animations.begin(); 
			ait != this->animations.end();
			ait++)
		{
			AnimProto* ap = *ait;
			DELETEARRAY(ap->anim_name);
			DELETESINGLE(ap);
		}
		this->animations.clear();

		DELETEARRAY(this->texture);
		DELETEARRAY(this->main_weapon);
		DELETEARRAY(this->alt_weapon);
	}


};

void AddProto(char* name, Proto* proto);
void RemoveAllProto();
void RemoveProto(char* name);
Proto* GetProto(string proto_name);
//void RemoveProto(map<string, Proto*>::iterator it);
#endif // __PROTO_H_