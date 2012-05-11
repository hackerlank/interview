#ifndef __HIGHSCORES_H_
#define __HIGHSCORES_H_

namespace Highscores
{
	void LockScore();
	bool GetScoreLock();
	char* GetAnswer();
	char* GetScoreTable( int mode );
	void Free();
	bool ProtectedVariable( int num, int& new_value, bool modify );
}

bool hsSendScore(const char* nickname, const int score, const int seconds, const int stat, const int mode, const char* char_name);

//bool NicknameExists(const char* nick);

//bool GetPositionByScore(const int score);

//bool GetData();



#endif
