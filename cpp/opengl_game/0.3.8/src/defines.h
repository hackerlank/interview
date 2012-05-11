#ifndef __DEFINES_H_
#define __DEFINES_H_

#define DEFAULT_FONT "default"
#define LOG_SCREEN_FONT "default"
#define DEBUG_FONT "default"

#define GAME_MAINMENU 0
#define GAME_RUNNING 1
#define GAME_GAMEMENU 2
#define GAME_PAUSED 20
#define GAME_EXITING 49


// -- MENU ID's
#define MENU_MAIN 0
#define MENU_CHARACTER 1
#define MENU_GAME 2
#define MENU_2BCONTINUED 3
#define MENU_FAILURE 4
// --------------------

#define LOG_SCREEN_MARGIN 70
#define WriteToLog LogToFile

#define LOG_SCREEN_TIME 5
#define LOG_SCREEN_X 150
#define LOG_SCREEN_Y 20
#define LOG_SCREEN_DX 0
#define LOG_SCREEN_DY 15

#define KEY_FIRST_DELAY 110
#define KEY_DELAY 110

#define DEFAULT_CONFIG_PATH				"\\config"
#define DEFAULT_CONFIG_NAME				"\\default.cfg"

#define DEFAULT_TEXTURES_PATH			"\\textures"
#define DEFAULT_FONTS_PATH				"\\fonts"
#define DEFAULT_DATA_PATH				"\\data"
#define DEFAULT_OBJECTS_PATH			"\\objects"
#define DEFAULT_SCRIPTS_PATH			"\\scripts"
#define DEFAULT_LOG_PATH				"\\log"
#define DEFAULT_LEVELS_PATH				"\\levels"
#define DEFAULT_DIALOGS_PATH			"\\dialogs"

#define DEFAULT_LOG_NAME				"\\iichan.log"
#define DEFAULT_GUI_LOG_NAME			"\\gui.log"
#define DEFAULT_OSL_LOG_NAME			"\\osl.log"
#define DEFAULT_CONS_LOG_NAME			"\\console.log"



#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

#define pi 3.14159265
#define DEGTORAD(a) a * pi/180

//#define USE_FONT_OUTLINES
#define USE_FONT_BITMAPS

//#define USE_PNG

#ifdef USE_PNG
#define USE_ZLIB
#endif




//#define _DEBUG_TIMINGS
#define _DEBUG_FPS
//#define _DEBUG_PICKS
#define _DEBUG_GAME
//#define _DEBUG_OSL
//#define _DEBUG_INTERPRET
//#define _DEBUG_LOAD_RACES
//#define DRAW_TIME_ELAPSED
//#define _DEBUG_MOUSE
//#define _DEBUG_GUI
//#define _DEBUG_SYS_VARS
//#define _DEBUG_KEYBOARD

#define SECOND_ELAPSED current_time - _previous_time >= 1000
#define GAMENAME "iichantra"
#define VERSION "iichantra XS build"
#define ENGINE "v. 0.3.8"
#define PRINT_VERSION
//#define VERSION_X 450
//#define VERSION_Y 70


#define GAME_DEBUG_MARGIN_X 150
#define GAME_DEBUG_MARGIX_Y 25


#define MAX_FIELD_LENGTH 50


#endif