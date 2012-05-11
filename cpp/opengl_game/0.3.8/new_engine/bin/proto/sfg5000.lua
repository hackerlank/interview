--forbidden
name = "sfg5000";

reload_time = 300;
bullets_count = -1;	-- ���� <0, �� �������� ����������


-- �������� ����
bullet_damage = 5;
bullet_vel = 10;

-- �������� ������� ����
texture = "bullets";

image_width = 256;
image_height = 32;
frame_width = 64;
frame_height = 32;
frames_count = 3;

animations = 
{
	{ 
	-- ����, ������� �����
		name = "straight";
		frames_count = 1;
		start_frame = 0;
		anim_speed = 100;
		real_x = 9;
		real_y = 12;
		real_mirror_x = 0;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 5;
	},
	{ 
	-- ����, ������� �� ��������� ����
		name = "diagdown";
		frames_count = 1;
		start_frame = 1;
		anim_speed = 100;
		real_x = 18;
		real_y = 0;
		real_mirror_x = 0;
		real_mirror_y = 15;
		real_width = 29;
		real_height = 30;
	},
	{ 
	-- ����, ������� �� ��������� �����
		name = "diagup";
		frames_count = 1;
		start_frame = 2;
		anim_speed = 100;
		real_x = 18;
		real_y = 0;
		real_mirror_x = 0;
		real_mirror_y = 15;
		real_width = 29;
		real_height = 30;
	}	,
	{}
}