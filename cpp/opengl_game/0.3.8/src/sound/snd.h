#ifndef _SND_H_
#define _SND_H_

#include "bass.h"

class SND
{
public:
	SND();
	~SND();
	bool InitBass();
	bool DestroyBass();
	bool LoadSound(string file_name);	
	bool PlaySnd(string name, bool looped);
	bool PauseSnd(string name);
	bool StopSnd(string name);

	bool StopAll();
	
	float GetVolume();
	bool SetVolume(float vol);
	float GetSndVolume(string name);
	bool SetSndVolume(string name, float vol);

	bool PlayBackMusic(string name);
	bool PauseBackMusic();
	bool StopBackMusic();
	
	string GetCurrentBackMusic() const;
private:
	void SetCurrentBackMusic(string newMusic);

	string currBackMusic;
	int err;
	bool isBass;
	map<string, DWORD> sounds;
};


#endif 
