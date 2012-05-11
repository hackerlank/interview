#ifndef _SND_H_
#define _SND_H_

#include "bass.h"
#include <map>


//struct SOUND{
//	string filename;
//	HMUSIC mod;
//};
class SND
{
public:
	SND();
	bool InitBass();
	bool LoadSound(string file_name);	
	bool DestroyBass();
	bool PlaySnd(string name, bool looped);
	bool PauseSnd(string name);
	bool StopSnd(string name);
	float GetVolume();
	bool SetVolume(float vol);
	float GetSndVolume(string name);
	bool SetSndVolume(string name, float vol);
private:
	int err;
	bool isBass;
	map<string, HMUSIC> sounds;
};

// Public standalone functions



//extern vector<SOUND*> sounds;
//extern map < string,HMUSIC > sounds;

#endif 
