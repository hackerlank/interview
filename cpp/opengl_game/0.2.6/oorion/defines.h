#define DEFAULT_FONT "default"
#define LOG_SCREEN_FONT "log_screen"
#define DEBUG_FONT "debug"
#define STAR_FONT "star"

#define GAME_MAINMENU 0
#define GAME_RUNNING 1
#define GAME_PAUSED 20
#define GAME_EXITING 49

#define RACE_COUNT 10
#define STARCOLOR_COUNT 5
#define STARSIZE_COUNT 3
#define STARTYPE_COUNT 4

#define LOG_SCREEN_MARGIN 70
#define WriteToLog LogToFile
#define MAP_DELTA 2
#define STAR_MARGIN 10

#define LOG_SCREEN_TIME 5
#define LOG_SCREEN_X 150
#define LOG_SCREEN_Y 20
#define LOG_SCREEN_DX 0
#define LOG_SCREEN_DY 15

#define DEFAULT_CONFIG_NAME				"\\default.cfg"

#define DEFAULT_TEXTURES_PATH			"\\textures"
#define DEFAULT_FONTS_PATH				"\\fonts"
#define DEFAULT_DATA_PATH				"\\data"
#define DEFAULT_DEFINITION_PATH			"\\def"
#define DEFAULT_SCRIPTS_PATH			"\\scripts"
#define DEFAULT_LOG_PATH				"\\log"

#define DEFAULT_LOG_NAME				"\\oorion.log"
#define DEFAULT_GUI_LOG_NAME			"\\gui.log"
#define DEFAULT_INTERPRET_LOG_NAME		"\\interpret.log"
#define DEFAULT_CONFIG_LOG_NAME			"\\config.log"
#define DEFAULT_TEXTURE_LOG_NAME		"\\texture.log"
#define DEFAULT_MISC_LOG_NAME			"\\misc.log"
#define DEFAULT_OSL_LOG_NAME			"\\osl.log"


#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

#define pi 3.14159265
#define DEGTORAD(a) a * pi/180




//#define _DEBUG_TIMINGS
#define _DEBUG_FPS
//#define _DEBUG_PICKS
#define _DEBUG_GAME
#define _DEBUG_LOAD_GUI
#define _DEBUG_INTERPRET
#define _DEBUG_LOAD_RACES
//#define DRAW_TIME_ELAPSED
//#define _DEBUG_MOUSE

#define SECOND_ELAPSED current_time - previous_time >= 1000
#define GAMENAME "OpenOrion"
#define VERSION "version 0.2.6"
#define PRINT_VERSION
//#define VERSION_X 450
//#define VERSION_Y 70

#define WINDOW_CAPTION_DY 25

#define dc 0

#define STAR_NAMES_COUNT 11

#define GAME_DEBUG_MARGIN_X 150
#define GAME_DEBUG_MARGIX_Y 25

#define MAP_QUADRANT_MULTIPLIER 6
#define MAP_MAX_SIZE 700


#define MAX_FIELD_LENGTH 50
