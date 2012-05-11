name = "sohchan";

main_weapon = "sfg9000";
alt_weapon = "sfg5000";
health = 100;

physic = 1;
phys_solid = 0;
phys_bullet_collidable = 1;
phys_max_x_vel = 5;
phys_max_y_vel = 50;
phys_jump_vel = 15;
phys_walk_acc = 3;

FunctionName = "CreatePlayer";

-- �������� �������

texture = "sohchan";
z = 0;

image_width = 1024;
image_height = 2048;
frame_width = 128;
frame_height = 128;
frames_count = 86;

animations = 
{
	{ 
	-- ����� 
		name = "idle";
		frames_count = 4;
		start_frame = 0;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 42;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- ����
		name = "walk";
		frames_count = 10;
		start_frame = 10;
		anim_speed = 100;
		real_x = 25;
		real_y = 30;
		real_mirror_x = 63;
		real_mirror_y = 30;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- ������
		name = "jump";
		frames_count = 3;
		start_frame = 45;
		anim_speed = 100;
		real_x = 45;
		real_y = 30;
		real_mirror_x = 43;
		real_mirror_y = 30;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- �������
		name = "sit";
		frames_count = 1;
		start_frame = 24;
		anim_speed = 100;
		real_x = 45;
		real_y = 49;
		real_mirror_x = 43;
		real_mirror_y = 49;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- �����������
		name = "land";
		frames_count = 2;
		start_frame = 43;
		anim_speed = 100;
		real_x = 45;
		real_y = 49;
		real_mirror_x = 43;
		real_mirror_y = 49;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- �������
		name = "die";
		frames_count = 13;
		start_frame = 30;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- ������������ ����
		name = "aim";
		frames_count = 1;
		start_frame = 21;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- �������� �� �������� ���������
		name = "shoot";
		frames_count = 2;
		start_frame = 21;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- �������� ����
		name = "sitshoot";
		frames_count = 2;
		start_frame = 25;
		anim_speed = 100;
		real_x = 45;
		real_y = 49;
		real_mirror_x = 43;
		real_mirror_y = 49;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- ������������ �����
		name = "gunaimup";
		frames_count = 1;
		start_frame = 51;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- �������� ����� �����
		name = "gunliftaimup";
		frames_count = 2;
		start_frame = 50;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- �������� �� �������� ����� ����� (����)
		name = "gunaimupshoot";
		frames_count = 3;
		start_frame = 51;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- ������ � �������� ����� ������
		name = "walkgunaimup";
		frames_count = 10;
		start_frame = 60;
		anim_speed = 100;
		real_x = 37;
		real_y = 30;
		real_mirror_x = 46;
		real_mirror_y = 30;
		real_width = 45;
		real_height = 75;
	},
	{ 
	-- ������������ ����
		name = "gunaimdown";
		frames_count = 1;
		start_frame = 81;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- ��������� ����� ����
		name = "gunliftaimdown";
		frames_count = 2;
		start_frame = 80;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- �������� �� ��������� ���� ����� (����)
		name = "gunaimdownshoot";
		frames_count = 2;
		start_frame = 81;
		anim_speed = 100;
		real_x = 45;
		real_y = 36;
		real_mirror_x = 43;
		real_mirror_y = 36;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- ������ � ��������� ���� ������
		name = "walkgunaimdown";
		frames_count = 10;
		start_frame = 70;
		anim_speed = 100;
		real_x = 40;
		real_y = 30;
		real_mirror_x = 43;
		real_mirror_y = 30;
		real_width = 45;
		real_height = 75;
	},
	{ 
	-- ������������ ����� (� ������)
		name = "jumpgunaimup";
		frames_count = 1;
		start_frame = 85;
		anim_speed = 100;
		real_x = 41;
		real_y = 14;
		real_mirror_x = 47;
		real_mirror_y = 14;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- �������� ����� ����� (� ������)
		name = "jumpgunliftaimup";
		frames_count = 2;
		start_frame = 84;
		anim_speed = 100;
		real_x = 41;
		real_y = 14;
		real_mirror_x = 47;
		real_mirror_y = 14;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- �������� �� �������� ����� ����� (� ������)
		name = "jumpgunaimupshoot";
		frames_count = 2;
		start_frame = 85;
		anim_speed = 100;
		real_x = 41;
		real_y = 14;
		real_mirror_x = 47;
		real_mirror_y = 14;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- ������������ ���� (� ������)
		name = "jumpgunaimdown";
		frames_count = 1;
		start_frame = 56;
		anim_speed = 100;
		real_x = 41;
		real_y = 14;
		real_mirror_x = 47;
		real_mirror_y = 14;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- ��������� ����� ���� (� ������)
		name = "jumpgunliftaimdown";
		frames_count = 2;
		start_frame = 55;
		anim_speed = 100;
		real_x = 41;
		real_y = 14;
		real_mirror_x = 47;
		real_mirror_y = 14;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- �������� �� ��������� ���� ����� (� ������)
		name = "jumpgunaimdownshoot";
		frames_count = 2;
		start_frame = 56;
		anim_speed = 100;
		real_x = 41;
		real_y = 14;
		real_mirror_x = 47;
		real_mirror_y = 14;
		real_width = 40;
		real_height = 62;
	}
	
}



