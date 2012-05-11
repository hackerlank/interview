#include "stdafx.h"
#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"
#include "misc.h"
#include "gui.h"
#include "game.h"




int difficulty_pick;
int size_pick;
int race_pick;
int opponents_pick;
unsigned int current_state;

extern GUI* gui;



const int default_money = 1500;


int GenerateRace(RACE* dest)
{
			errno_t err;
			unsigned int number;
			int result;

			err = rand_s(&number);

			if(err != 0)
			{
				LogToFile("rand_s() failed");
				return -1;
			}

			result = ((float)number / (float)UINT_MAX * 10.0) + 1;

			*dest = RACE(result);
			return result;
}


GAME* StartGame(LPSTR playername, int difficulty_pick, int size_pick, int race_pick, int opponents_pick)
{
	GAME* game;

	game = new GAME();

	gui->Hide();
	gui->LogOnScreen("Starting game");

#ifdef _DEBUG_
	LogToFile("Starting game");
#endif

	gui->LogOnScreen("Creating players");

	// zero player is human player

	for(int i = 0; i < 5; i++)
	{
		PLAYER* player;
		LPSTR nm = NULL;
		int race = 0;

		player = new PLAYER();

		//sprintf(nm, "Player%d", i);
		player->money = default_money;
		player->overall_RP = 0;
		if(i == 0)
			player->race = RACE(race_pick);
		else
		{
			//retry:
			race = GenerateRace(&player->race);

			//for(std::vector<PLAYER>::iterator it = game->players.begin(); it != game->players.end()-1; it++)
			//	if(it->race == player->race)
			//		goto retry;
		}
		player->stars.resize(0);

		game->players.push_back(*player);
		gui->LogOnScreen(names_race[race]);
	}
}