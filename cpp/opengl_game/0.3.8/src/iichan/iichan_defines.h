#ifndef __ii_DEFINES_H_
#define __ii_DEFINES_H_
#define PLAYER_START_X 300
#define PLAYER_START_Y 350

#define KEYDOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//#define PROCESS_DELAY 10
#define MOVE_DELAY 100

#define BORDER_MARGIN cfg.scr_width / 2 // fix it

#define FLASH_DX_RIGHT 55
#define FLASH_DX_LEFT 20
#define FLASH_DY 27
#define FLASH_SIT_DY 15
#define FLASH_MOVE_DY 15
#define FLASH_JUMP_DY 10
#define HUD_DY 25

#define DEFAULT_Z 100
#define PLAYER_Z 5000
#define SHADOW_Z 4000
#define TOP_Z 10000

#define DAMAGE_DX 50


#define SIMPLE_HUD // single icons of HUD
#define HUD_X 10

#ifdef SIMPLE_HUD
	#define HEALTH_X 20
	#define AMMO_X 300
	#define SCORE_X 500
#else
	#define HUD_Q 5
#endif

#define TICK 10
#define JUMP_SPEED 3


#endif
