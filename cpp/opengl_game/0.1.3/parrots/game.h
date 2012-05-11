#define GAME_MAINMENU 0
#define GAME_RUNNING 1
#define GAME_PAUSED 20
#define GAME_EXITING 49

#define MAP_TINY 400
#define MAP_SMALL 600
#define MAP_AVERAGE 800
#define MAP_LARGE 1000
#define MAP_HUGE 1200

unsigned int current_state;

enum DIFFICULTY { dfEasy, dfMedium, dfHard, dfImpossible };
enum GALAXYSIZE { gsTiny, gsSmall, gsAverage, gsBig, gsHuge };
enum RACE { rcAlkari, rcBulrathi, rcKlackon, rcSakkra, rcPsilon, rcMrrshan, rcMeklar, rcHuman, rcDarlok, rcSilicoids };

DIFFICULTY diff_pick;
GALAXYSIZE size_pick;
RACE race_pick;
int opponents_pick;

typedef int STARID;

typedef struct tagPLAYER {
	LPSTR name;
	std::vector<STARID> stars;
	int overall_RP;
	int money;
	RACE race;

} PLAYER;

typedef struct tagGAMESPRITE {
	texture_t* tex;
	int patternCount;
	int currentPattern;
	int patternWidth;
	int patternHeight;
} GAMESPRITE;

typedef struct tagSTAR : GAMESPRITE {
	LPSTR name;
	PLAYER* owner;
	STARID id;
} STAR;


typedef struct tagGAME {
	std::vector<PLAYER> players;
	std::vector<STAR> stars;
} GAME;



GAME* StartGame(LPSTR playername, int difficulty_pick, int size_pick, int race_pick, int opponents_pick);

