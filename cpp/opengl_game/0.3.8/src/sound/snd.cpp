#include "StdAfx.h"

#include "bass.h"
#include "snd.h"

#include "../misc.h"

#pragma comment (lib, "bass.lib")
#pragma warning (disable : 4800) // disable annoying 'BOOL to int conversion' warnings


SND::SND()
{
	isBass = InitBass();
}

SND::~SND()
{
	DestroyBass();
}

bool SND::LoadSound(string file_name)
{
	if(isBass)
	{
		DWORD d;
		string name = NameFromPath(file_name);
		char path_app[MAX_PATH];
		char ch[MAX_PATH];
		GetCurrentDirectory(255, path_app);
		sprintf(ch, "%s\\%s", path_app, file_name.data());
		d = BASS_MusicLoad(FALSE,ch,0,0,BASS_MUSIC_RAMPS,0);
		if (!d)
			d = BASS_StreamCreateFile(FALSE, ch, 0, 0, 0);

		if (d) 
			sounds[name] = d;
		else 
			return false;

		return true;	
	}
	return false;
}


bool SND::InitBass()
{
	 return BASS_Init(-1, 44100, 0, 0, NULL);
}

bool SND::DestroyBass()
{	
	if (isBass)
	{
		BASS_Free(); 
		isBass = false;
		return true;
	}
	return false;
}

bool SND::PlaySnd(string name, bool looped)
{
	if(isBass)
		return BASS_ChannelPlay(sounds[name],looped);
	return false;
}

bool SND::StopSnd(string name)
{
	if(isBass)
		return BASS_ChannelStop(sounds[name]);
	return false;
}

bool SND::PauseSnd(string name)
{
	if(isBass)
		return BASS_ChannelPause(sounds[name]);
	return false;
}

float SND::GetVolume()
{
	if (isBass)
		return BASS_GetVolume();
	return -1.0;
}

bool SND::SetVolume(float vol)
{
	if (isBass)
		if (vol >= 0 && vol <= 1)
			return BASS_SetVolume(vol);
	return false;
}

float SND::GetSndVolume(string name)
{
	if (isBass)
	{
		float* vol = NULL;
		if (BASS_ChannelGetAttribute(sounds[name], BASS_ATTRIB_MUSIC_VOL_CHAN, vol))
			return *vol;
	}
	return -1.0;
}

bool SND::SetSndVolume(string name, float vol)
{
	if(isBass)
		if(vol >= 0 && vol <= 1)
			return BASS_ChannelSetAttribute(sounds[name], BASS_ATTRIB_MUSIC_VOL_CHAN, vol);
	return false;
}

bool SND::PlayBackMusic(string name)
{
	StopBackMusic();
	if (PlaySnd(name, true))
	{
		SetCurrentBackMusic(name);
		return true;
	}
	return false;
}

bool SND::PauseBackMusic()
{
	return PauseSnd(currBackMusic);
}

bool SND::StopBackMusic()
{
	bool b = StopSnd(currBackMusic);
	SetCurrentBackMusic(string(""));
	return b;
}

void SND::SetCurrentBackMusic(string newMusic)
{
	currBackMusic = newMusic;
}

string SND::GetCurrentBackMusic() const
{
	return currBackMusic;
}

bool SND::StopAll()
{
	if (isBass)
	{
		for(map<string, HMUSIC>::iterator it = sounds.begin();
			it != sounds.end();
			it++)
		{
			BASS_ChannelStop(it->second);
		}
		return true;
	}
	return false;
}