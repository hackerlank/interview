name = "unylchan";

main_weapon = "sfg5000";
--alt_weapon = "sfg5000";
health = 100;

physic = 1;
phys_solid = 0;
phys_bullet_collidable = 1;
phys_max_x_vel = 5;
phys_max_y_vel = 50;
phys_jump_vel = 15;
phys_walk_acc = 3;

FunctionName = "CreatePlayer";

-- Описание спрайта

texture = "unyl_sprite";
z = 0;

image_width = 1024;
image_height = 1024;
frame_width = 100;
frame_height = 100;
frames_count = 86;

animations = 
{
	{ 
	-- Стоит 
		name = "idle";
		frames_count = 4;
		start_frame = 0;
		anim_speed = 100;
		real_x = 19;
		real_y = 15;
		real_mirror_x = 15;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Идет
		name = "walk";
		frames_count = 10;
		start_frame = 10;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Прыжок
		name = "jump";
		frames_count = 4;
		start_frame = 43;
		anim_speed = 100;
		real_x = 10;
		real_y = 5;
		real_mirror_x = 20;
		real_mirror_y = 5;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- Садится
		name = "sit";
		frames_count = 1;
		start_frame = 24;
		anim_speed = 100;
		real_x = 19;
		real_y = 30;
		real_mirror_x = 16;
		real_mirror_y = 30;
		real_width = 40;
		real_height = 62;
	},
--[[	{ 
	-- Приземление
		name = "land";
		frames_count = 2;
		start_frame = 43;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},--]]--
	{ 
	-- Умирает
		name = "die";
		frames_count = 12;
		start_frame = 30;
		anim_speed = 100;
		real_x = 19;
		real_y = 15;
		real_mirror_x = 15;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Прицеливание стоя
		name = "aim";
		frames_count = 1;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Стрельба из стоячего положения
		name = "shoot";
		frames_count = 2;
		start_frame = 21;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Стрельба сидя
		name = "sitshoot";
		frames_count = 2;
		start_frame = 25;
		anim_speed = 100;
		real_x = 19;
		real_y = 30;
		real_mirror_x = 16;
		real_mirror_y = 30;
		real_width = 40;
		real_height = 62;
	},
	{ 
	-- Прицеливание вверх
		name = "gunaimup";
		frames_count = 1;
		--start_frame = 51;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Поднятие пушки вверх
		name = "gunliftaimup";
		frames_count = 2;
		--start_frame = 50;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Стрельба из поднятой вверх пушки (стоя)
		name = "gunaimupshoot";
		frames_count = 3;
		--start_frame = 51;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Ходьба с поднятой вверх пушкой
		name = "walkgunaimup";
		frames_count = 10;
		--start_frame = 60;
		start_frame = 10;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Прицеливание вниз
		name = "gunaimdown";
		frames_count = 1;
		--start_frame = 81;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Опускание пушки вниз
		name = "gunliftaimdown";
		frames_count = 2;
		--start_frame = 80;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Стрельба из опущенной вниз пушки (стоя)
		name = "gunaimdownshoot";
		frames_count = 2;
		--start_frame = 81;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Ходьба с опущенной вниз пушкой
		name = "walkgunaimdown";
		frames_count = 10;
		--start_frame = 70;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Прицеливание вверх (в прыжке)
		name = "jumpgunaimup";
		frames_count = 1;
		--start_frame = 85;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Поднятие пушки вверх (в прыжке)
		name = "jumpgunliftaimup";
		frames_count = 2;
		--start_frame = 84;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Стрельба из поднятой вверх пушки (в прыжке)
		name = "jumpgunaimupshoot";
		frames_count = 2;
		--start_frame = 85;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Прицеливание вниз (в прыжке)
		name = "jumpgunaimdown";
		frames_count = 1;
		--start_frame = 56;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Опускание пушки вниз (в прыжке)
		name = "jumpgunliftaimdown";
		frames_count = 2;
		--start_frame = 55;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	},
	{ 
	-- Стрельба из опущенной вниз пушки (в прыжке)
		name = "jumpgunaimdownshoot";
		frames_count = 2;
		--start_frame = 56;
		start_frame = 20;
		anim_speed = 100;
		real_x = 8;
		real_y = 15;
		real_mirror_x = 20;
		real_mirror_y = 15;
		real_width = 40;
		real_height = 75;
	}
	
}



