#include "stdafx.h"

#include "texture.h"
#include "font.h"
#include "draw.h"
#include "config.h"
#include "misc.h"
#include "gui.h"
#include "math.h"

#include "game.h"


int difficulty_pick;
int size_pick;
int race_pick;
int opponents_pick;
unsigned int current_state;

extern GUI* gui;
extern font_t fonts[256];
extern texture_t textures[256];
extern HDC hDC;



//#define GAME_DEBUG_X 500
//#define GAME_DEBUG_Y 


void STAR::Draw()
{

		__super::Draw();
		if(owner)
			glColor4ubv(owner->color);
		else
			glColor4ub(128, 128, 128, 255);

		TextOut(string(name), STAR_FONT, &hDC, x, y - 15);
}





int GenerateRace(PLAYER* dest)
{
	int result = Random(RACE_COUNT);
	dest->race = RACE(result);
	dest->_race = result;
	return result;
}

int GAME::GenerateStar(STAR* dest)
{
	static int count;

	count++;

	errno_t err;
	unsigned int pick;


	err = rand_s(&pick);

	if(err != 0)
	{
		LogToFile("rand_s() failed");
		return -1;
	}

	dest->starcolor = STARCOLOR(Random(STARCOLOR_COUNT));
	dest->starsize = STARSIZE(Random(STARSIZE_COUNT));
	dest->startype = STARTYPE(Random(STARTYPE_COUNT));
	dest->name = star_names[Random(STAR_NAMES_COUNT)];
	dest->id = count-1;
	dest->owner = NULL;
	dest->tex = &textures[FindByName(textures, "star")];
	dest->SetCurrentPattern(0);
	dest->SetPatternWidth(dest->tex->width);
	dest->SetPatternHeight(dest->tex->height);
}


void GAME::AddPlayer(PLAYER new_player)
{
	playerCount++;
	new_player.stars.resize(0);
	players.push_back(new_player);
}

void GAME::AddStar(STAR new_star)
{
	starCount++;
	stars.push_back(new_star);
}


void GAME::WriteDebug()
{
	LogToFile("Difficulty=" + string(names_difficulty[difficulty_pick]));
	LogToFile("Galaxy size=" + string(names_size[size_pick]));
	LogToFile("Race=" + string(names_race[race_pick]));
	LogToFile("Opponents count=" + string(names_opponents[opponents_pick]));
}

void GAME::DebugToScreen()
{
	int i = 0;
	char output[50];

	TextOut("Players: ", DEBUG_FONT, &hDC, cfg.scr_width - GAME_DEBUG_MARGIN_X, GAME_DEBUG_MARGIX_Y);

	if(GetPlayerCount() > 0)
		for(std::vector<PLAYER>::iterator it = players.begin(); it != players.end(); it++)
		{
			i++;
			sprintf(output, "%s (%s)", it->name, names_race[it->race]);
			TextOut(output, DEBUG_FONT, &hDC, cfg.scr_width - GAME_DEBUG_MARGIN_X, GAME_DEBUG_MARGIX_Y + i*15);

		}
}

void GAME::InitPlayers(int race_pick, int opponents_pick)
{
	playerCount = 0;
	// zero player is human player

	for(int i = 0; i < opponents_pick+2; i++)
	{
		PLAYER* player;
		char name[20];
		int race = 0;

		player = new PLAYER();

		sprintf(name, "Player%d", i);
		strcpy(player->name, name);
		player->money = default_money;
		player->overall_RP = 0;
		if(i == 0)
		{
			race = race_pick;
			player->race = RACE(race_pick);
		}
		else
		{
			retry:
			race = GenerateRace(player);

			for(std::vector<PLAYER>::iterator it = players.begin(); it != players.end(); it++)
				if(it->race == player->race)
					goto retry;
		}

		AddPlayer(*player);
		gui->LogOnScreen(string(names_race[race]) + " arrived");
#ifdef _DEBUG_
		LogToFile(string("Created player: name=") + string(name) + "; race=" + string(names_race[race]));
#endif
	}
}

void GAME::InitStars(int size_pick)
{
	int stars_to_add = map_size[size_pick];

	double root = sqrt((double)stars_to_add);

	int stars_per_row = floor(root);

	int stars_remaining = stars_to_add;

	int row_count = -1;

	while(stars_remaining > 0)
	{
		int row_length = Random(stars_per_row - 1, stars_per_row + 1);
		row_count++;


		for(int i = 0; i < row_length; i++)
		{
			STAR* star = new STAR();
			GenerateStar(star);

			int quadrantWidth = MAP_MAX_SIZE / row_length;

			star->x = Random(i * quadrantWidth + STAR_MARGIN, i * quadrantWidth + quadrantWidth - STAR_MARGIN);
			star->y = Random(row_count * quadrantWidth + STAR_MARGIN, row_count * quadrantWidth + quadrantWidth - STAR_MARGIN);
			AddStar(*star);
			stars_remaining--;

			char output[50];

			sprintf(output, "Star %s created at %d, %d", star->name, star->x, star->y);

#ifdef _DEBUG_GAME
			gui->LogOnScreen(output);
#endif
#ifdef _DEBUG_
			LogToFile(output);
#endif
		}
	}





}










GAME::GAME(LPSTR playername, int difficulty_pick, int size_pick, int race_pick, int opponents_pick)
{
	gui->Hide();
	gui->LogOnScreen("Starting game");
	gui->ViewMenu(5);

#ifdef _DEBUG_
	WriteDebug();
#endif

	SetSizeX(size_pick * 400 + 400);
	SetSizeY(size_pick * 400 + 400);


	gui->LogOnScreen("Creating races");
	InitPlayers(race_pick, opponents_pick);

	gui->LogOnScreen("Creating galaxy");
	InitStars(size_pick);

}



void GAME::Draw()
{
	for(std::vector<STAR>::iterator it = stars.begin(); it != stars.end(); it++)
		it->Draw();
}
		