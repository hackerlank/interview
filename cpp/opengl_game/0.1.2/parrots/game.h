#define GAME_MAINMENU 0
#define GAME_LOADMENU 15
#define GAME_SAVEMENU 16

#define GAME_RUNNING 1
#define GAME_PAUSED 20
#define GAME_EXITING 49

unsigned int current_state;


enum RACE { rcAlkari, rcBulrathi, rcPsilon, rcHuman, rcDarlok, rcMeklar, rcKlackon, rcMrrshan, rcSakkra, rcSilicoids };
enum GALAXYSIZE { gsTiny, gsSmall, gsAverage, gsBig, gsHuge };
enum DIFFICULTY { dfEasy, dfMedium, dfHard, dfImpossible };
string races[] = { "Alkari", "Bulrathi", "Psilon", "Human", "Darlok", "Meklar", "Klackon", "Mrrshan", "Sakkra", "Silicoids" };


void StartGame(GALAXYSIZE size, DIFFICULTY diff, int opponents, RACE player_choice);
			
