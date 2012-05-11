#define OORION_EXPORTS
#define DEFAULT_FONT &fonts[0]
#define LOG_SCREEN_FONT &fonts[1]
#define DEBUG_FONT &fonts[2]
#define STAR_FONT &fonts[3]

#define GAME_MAINMENU 0
#define GAME_RUNNING 1
#define GAME_PAUSED 20
#define GAME_EXITING 49

#define MAP_TINY 5
#define MAP_SMALL 10
#define MAP_AVERAGE 15
#define MAP_LARGE 20
#define MAP_HUGE 25

#define RACE_COUNT 10
#define STARCOLOR_COUNT 5
#define STARSIZE_COUNT 3
#define STARTYPE_COUNT 4

enum DIFFICULTY { dfEasy, dfMedium, dfHard, dfImpossible };
enum GALAXYSIZE { gsTiny, gsSmall, gsAverage, gsBig, gsHuge };
enum RACE { rcAlkari, rcBulrathi, rcKlackon, rcMeklar, rcPsilon, rcMrrshan, rcDarlok, rcHuman, rcSakkra, rcSilicoid };
enum STARCOLOR { scYellow, scRed, scBlue, scWhite, scGreen };
enum STARSIZE { ssSmall, ssMedium, ssBig };
enum STARTYPE { stTerrain, stUnavailable, stInferno, stOrion };

const LPSTR names_menu[] = { "MainMenu", "DifficultyMenu", "SizeMenu", "RaceMenu", "OpponentsMenu" };
const LPSTR names_main[] = { "Start", "Load", "Options", "Credits", "Exit" };
const LPSTR names_difficulty[] = { "Easy", "Medium", "Hard", "Impossible" };
const LPSTR names_size[] = { "Tiny", "Small", "Average", "Big", "Huge" };
const LPSTR names_race[] = { "Alkari", "Bulrathi", "Klackon", "Meklar", "Psilon", "Mrrshan" , "Darlok", "Human", "Sakkra", "Silicoid" };
const LPSTR names_opponents[] = { "One opponent", "Two opponents", "Three opponents", "Four opponents", "Five opponents" };

typedef int STARID;

#define LOG_SCREEN_TIME 5
//#define LOG_SCREEN_X 150
#define LOG_SCREEN_Y 20
#define LOG_SCREEN_DX 0
#define LOG_SCREEN_DY 15


enum ACTION { actNo, actOpenNext, actClose, actStart, actExit };

#define DEFAULT_LOG_NAME "debug.log"
#define DEFAULT_CONFIG_NAME "default.cfg"
#define DEFAULT_TEXTURES_PATH "\\textures"
#define DEFAULT_FONTS_PATH "\\fonts"

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

//#define _DEBUG_TIMINGS
#define _COUNT_ONLY_FPS
//#define _DEBUG_GUI
#define _DEBUG_PICKS
#define _DEBUG_GAME

#define SECOND_ELAPSED current_time - previous_time >= 1000
#define GAMENAME "OpenOrion"
#define VERSION "version 0.1.5"
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