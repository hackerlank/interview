#ifndef __GAME_H_
#define __GAME_H_

namespace game
{
	void InitGame();
	void FreeGame();

	void UpdateGame();
	void DrawGame();
	void HandleKeyboard();

	enum GameStates
	{
		GAME_NONE,
		GAME_INITIALIZING,
		GAME_RUNNNIG,
		GAME_DESTROYING,
		GAME_PAUSED
	};

	#define GAMETICK 10
}


#endif // __GAME_H_