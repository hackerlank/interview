#define OORION_EXPORTS
#define DEFAULT_FONT &fonts[0]
#define LOG_SCREEN_FONT &fonts[1]
#define DEBUG_FONT &fonts[2]
#define STAR_FONT &fonts[3]

#define GAME_MAINMENU 0
#define GAME_RUNNING 1
#define GAME_PAUSED 20
#define GAME_EXITING 49



#define RACE_COUNT 10
#define STARCOLOR_COUNT 5
#define STARSIZE_COUNT 3
#define STARTYPE_COUNT 4

#define WriteToLog LogToFile

const int map_size[] = { 8, 12, 16, 20, 25 };
#define MAP_DELTA 2

#define STAR_MARGIN 10

typedef int STARID;

#define LOG_SCREEN_TIME 5
//#define LOG_SCREEN_X 150
#define LOG_SCREEN_Y 20
#define LOG_SCREEN_DX 0
#define LOG_SCREEN_DY 15


enum ACTION { actNo, actOpenNext, actStart, actExit };
enum GROUP { gNo, gDifficulty, gRace, gSize, gOpponents };

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




#define pi 3.14159265
#define DEGTORAD(a) a * pi/180

#define MOUSE_LEFT_CLICKED 0x0FF0
#define MOUSE_RIGHT_CLICKED 0x0FF1
#define MOUSE_OVER 0x0FF2

const int menu_length = 5;
const int main_length = 5;
const int difficult_length = 4;
const int size_length = 5;
const int race_length = 10;
const int opponents_length = 5;

#define _DEBUG_TIMINGS
//#define _COUNT_ONLY_FPS
#define _DEBUG_GUI
#define _DEBUG_PICKS
#define _DEBUG_GAME
#define _DEBUG_LOAD_GUI
#define _DEBUG_INTERPRET
#define _DEBUG_LOAD_RACES

#define SECOND_ELAPSED current_time - previous_time >= 1000
#define GAMENAME "OpenOrion"
#define VERSION "version 0.2.2"
#define PRINT_VERSION
//#define VERSION_X 450
//#define VERSION_Y 70

#define WINDOW_CAPTION_DY 25

#define dc 0

const int default_money = 1500;
const LPSTR star_names[] = { "Alpha Centauri", "Vega", "Sol", "Antares", "Pollux", "Mitsar", "Sirius",
"Arctur", "Arrakis", "Kanopus", "Castra" };

#define STAR_NAMES_COUNT 11

#define GAME_DEBUG_MARGIN_X 150
#define GAME_DEBUG_MARGIX_Y 25

#define MAP_QUADRANT_MULTIPLIER 6
#define MAP_MAX_SIZE 700


#define MAX_FIELD_LENGTH 50
