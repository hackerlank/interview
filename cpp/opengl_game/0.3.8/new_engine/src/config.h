#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "misc.h"

typedef struct tagCONFIG {
	UINT scr_width;
	UINT scr_height;
	UINT scr_bpp;

	float near_z;
	float far_z;

	UINT window_width;
	UINT window_height;

	BOOL fullscreen;
	BOOL vert_sync;
	//bool anti_alias;
	BOOL debug;
	BOOL show_fps;
	//bool force_start;

	UINT left;
	UINT right;
	UINT down;
	UINT up;
	UINT jump;
	UINT fire;
	UINT sit;
	UINT change_weapon;
	UINT change_player;
	UINT screenshot;
	UINT show_menu;

	UINT bullet_speed;
	UINT bullet_damage;
	UINT hero_vuln;

	float gravity;
	tagCONFIG()
	{
		ZeroMemory(this, sizeof(this));
	}
} config;


__INLINE UINT StringToVKey(string s)
{
	
	if(s == "vk_left")
		return VK_LEFT;
	else if(s == "vk_right")
		return VK_RIGHT;
	else if(s == "vk_down")
		return VK_DOWN;
	else if(s == "vk_up")
		return VK_UP;
	else if(s == "vk_control")
		return VK_CONTROL;
	else if(s == "vk_space")
		return VK_SPACE;
	else if(s == "vk_a")
		return 0x41;
	else if(s == "vk_b")
		return 0x42;
	else if(s == "vk_c")
		return 0x43;
	else if(s == "vk_d")
		return 0x44;
	else if(s == "vk_e")
		return 0x45;
	else if(s == "vk_f")
		return 0x46;
	else if(s == "vk_g")
		return 0x47;
	else if(s == "vk_h")
		return 0x48;
	else if(s == "vk_i")
		return 0x49;
	else if(s == "vk_j")
		return 0x4A;
	else if(s == "vk_k")
		return 0x4B;
	else if(s == "vk_l")
		return 0x4C;
	else if(s == "vk_m")
		return 0x4D;
	else if(s == "vk_n")
		return 0x4E;
	else if(s == "vk_o")
		return 0x4F;
	else if(s == "vk_p")
		return 0x50;
	else if(s == "vk_q")
		return 0x51;
	else if(s == "vk_r")
		return 0x52;
	else if(s == "vk_s")
		return 0x53;
	else if(s == "vk_t")
		return 0x54;
	else if(s == "vk_u")
		return 0x55;
	else if(s == "vk_v")
		return 0x56;
	else if(s == "vk_w")
		return 0x57;
	else if(s == "vk_x")
		return 0x58;
	else if(s == "vk_y")
		return 0x59;
	else if(s == "vk_z")
		return 0x5A;
	else if(s == "vk_f1")
		return VK_F1;
	else if(s == "vk_f2")
		return VK_F2;
	else if(s == "vk_f3")
		return VK_F3;
	else if(s == "vk_f4")
		return VK_F4;
	else if(s == "vk_f5")
		return VK_F5;
	else if(s == "vk_f6")
		return VK_F6;
	else if(s == "vk_f7")
		return VK_F7;
	else if(s == "vk_f8")
		return VK_F8;
	else if(s == "vk_f9")
		return VK_F9;
	else if(s == "vk_f10")
		return VK_F10;
	else if(s == "vk_f11")
		return VK_F11;
	else if(s == "vk_f12")
		return VK_F12;
	else
		return 0x00;
}

__INLINE BOOL GetVKeyByNum(UINT num, char* vk)
{
	if (vk)
	{
		switch(num)
		{
		case VK_LBUTTON:	strcpy(vk, "lmb"); 			break;
		case VK_RBUTTON:	strcpy(vk, "rmb");			break;

		case VK_BACK:		strcpy(vk, "backspace");	break;
		case VK_TAB:		strcpy(vk, "tab"); 			break;
		case VK_RETURN:		strcpy(vk, "enter"); 		break;
		case VK_SHIFT:		strcpy(vk, "shift"); 		break;
		case VK_CONTROL:	strcpy(vk, "ctrl"); 		break;
		case VK_MENU:		strcpy(vk, "alt"); 			break;
		case VK_PAUSE:		strcpy(vk, "pause");		break;
		case VK_CAPITAL:	strcpy(vk, "capslock"); 	break;
		case VK_ESCAPE:		strcpy(vk, "esc"); 			break;
		case VK_SPACE:		strcpy(vk, "space"); 		break;
		case VK_PRIOR:		strcpy(vk, "pgup"); 		break;
		case VK_NEXT:		strcpy(vk, "pgdown"); 		break;
		case VK_END:		strcpy(vk, "end"); 			break;
		case VK_HOME:		strcpy(vk, "home"); 		break;

		case VK_LEFT:		strcpy(vk, "left"); 		break;
		case VK_UP:			strcpy(vk, "up"); 			break;
		case VK_RIGHT:		strcpy(vk, "right"); 		break;
		case VK_DOWN:		strcpy(vk, "down"); 		break;

		case VK_SNAPSHOT:	strcpy(vk, "printscreen");	break;
		case VK_INSERT:		strcpy(vk, "insert"); 		break;
		case VK_DELETE:		strcpy(vk, "delete");		break;		
			// VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
			// 0x40 : unassigned
			// VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
		case 0x30: 			strcpy(vk, "0");  			break;
		case 0x31: 			strcpy(vk, "1");  			break;
		case 0x32: 			strcpy(vk, "2");  			break;
		case 0x33: 			strcpy(vk, "3");  			break;
		case 0x34: 			strcpy(vk, "4");  			break;
		case 0x35: 			strcpy(vk, "5");  			break;
		case 0x36: 			strcpy(vk, "6");  			break;
		case 0x37: 			strcpy(vk, "7");  			break;
		case 0x38: 			strcpy(vk, "8");  			break;
		case 0x39: 			strcpy(vk, "9");  			break;

		case 0x41: 			strcpy(vk, "a");  			break;
		case 0x42: 			strcpy(vk, "b");  			break;
		case 0x43: 			strcpy(vk, "c");  			break;
		case 0x44: 			strcpy(vk, "d");  			break;
		case 0x45: 			strcpy(vk, "e");  			break;
		case 0x46: 			strcpy(vk, "f");  			break;
		case 0x47: 			strcpy(vk, "g");  			break;
		case 0x48: 			strcpy(vk, "h");  			break;
		case 0x49: 			strcpy(vk, "i");  			break;
		case 0x4a: 			strcpy(vk, "j");  			break;
		case 0x4b: 			strcpy(vk, "k");  			break;
		case 0x4c: 			strcpy(vk, "l");  			break;
		case 0x4d: 			strcpy(vk, "m");  			break;
		case 0x4e: 			strcpy(vk, "n");  			break;
		case 0x4f: 			strcpy(vk, "o");  			break;
		case 0x50:			strcpy(vk, "p");  			break;
		case 0x51:			strcpy(vk, "q");  			break;
		case 0x52:			strcpy(vk, "r");  			break;
		case 0x53:			strcpy(vk, "s");  			break;
		case 0x54: 			strcpy(vk, "t");  			break;
		case 0x55: 			strcpy(vk, "u");  			break;
		case 0x56: 			strcpy(vk, "v");  			break;
		case 0x57: 			strcpy(vk, "w");  			break;
		case 0x58: 			strcpy(vk, "x");  			break;
		case 0x59:			strcpy(vk, "y");  			break;
		case 0x5a:			strcpy(vk, "z");  			break;

		case VK_NUMPAD0:	strcpy(vk, "num0");  		break;
		case VK_NUMPAD1:	strcpy(vk, "num1");  		break;
		case VK_NUMPAD2:	strcpy(vk, "num2");  		break;
		case VK_NUMPAD3:	strcpy(vk, "num3");  		break;
		case VK_NUMPAD4:	strcpy(vk, "num4");  		break;
		case VK_NUMPAD5:	strcpy(vk, "num5");  		break;
		case VK_NUMPAD6:	strcpy(vk, "num6"); 		break;
		case VK_NUMPAD7:	strcpy(vk, "num7"); 		break;
		case VK_NUMPAD8:	strcpy(vk, "num8"); 		break;
		case VK_NUMPAD9:	strcpy(vk, "num9");			break;
		case VK_MULTIPLY:	strcpy(vk, "multiply");		break;
		case VK_ADD:		strcpy(vk, "add");			break;
		case VK_SEPARATOR:	strcpy(vk, "separator");	break;
		case VK_SUBTRACT:	strcpy(vk, "substract");	break;
		case VK_DECIMAL:	strcpy(vk, "decimal");		break;
		case VK_DIVIDE:		strcpy(vk, "divide");		break;

		case VK_F1:			strcpy(vk, "f1");			break;
		case VK_F2:			strcpy(vk, "f2"); 			break;
		case VK_F3:			strcpy(vk, "f3"); 			break;
		case VK_F4:			strcpy(vk, "f4"); 			break;
		case VK_F5:			strcpy(vk, "f5");			break;
		case VK_F6:			strcpy(vk, "f6"); 			break;
		case VK_F7:			strcpy(vk, "f7"); 			break;
		case VK_F8:			strcpy(vk, "f8"); 			break;
		case VK_F9:			strcpy(vk, "f9"); 			break;
		case VK_F10:		strcpy(vk, "f10"); 			break;
		case VK_F11:		strcpy(vk, "f11"); 			break;
		case VK_F12:		strcpy(vk, "f12"); 			break;

		default:			strcpy(vk, ""); return FALSE; break;

		}
		return TRUE;
	}
	return FALSE;
}

#endif