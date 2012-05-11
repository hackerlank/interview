//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Miscellaneous
//
// DESCRIPTION:
//	Misc. stuff, tools, logging etc
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "fs_data.h"

#include "m_tools.h"


//?

void M_Echo(int stream, const char* message, ...)
{
	if(stream && S_LOG)
	{
		// opening file for write and append (text)

		static uint __counter = 0;
		
		FILE* f = NULL;
		char* mode = "a\0";

		if(__counter == 0)
			mode = "w\0";

		f = fopen(LOGFILE, mode);

		if(f)
		{

			va_list argptr;

			va_start(argptr, message);
			vfprintf(f, message, argptr);
			fprintf(f, "\n");
			va_end(argptr);

			fclose(f);

		}

		__counter++;
	}

	if(stream && S_SCREEN)
	{
		// TODO: textout to screen

	}

	if(stream && S_CONSOLE)
	{
		// TODO: console

	}

	if(stream && S_STDERR)
	{

		// writing to stderr

		va_list argptr;

		va_start(argptr, message);
		vfprintf(stderr, message, argptr);
		fprintf(stderr, "\n");
		va_end(argptr);

		fflush(stderr);

	}
}



// gets extension
// e.g. from 'textures/sky.bmp' will return 'bmp'

char* M_GetExtFromPath(const char* path)
{
	if(!path)
		return NULL;

	char* p = (char*)path + strlen(path);

	for(;;)
	{
		p--;

		if(*p == '.')
			return ++p;

		if(p == path)
			return NULL;

	}
}



// gets file name without extension
// e.g. from 'textures/sky.bmp' will return 'sky'
// NOTE: dont forget to free memory after usage

char* M_GetNameFromPath(const char* path)
{
	if(!path)
		return NULL;

	char* p = (char*)path + strlen(path);

	uint l = 0;
	
	bool f = false;

	for(;;)
	{
		p--;

		if(f)
			l++;

		if(*p == '.')
			f = true;

		if(*p == '\\' || *p == '/' || p == path)
		{
			if(f)
			{
				char* name = new char[l+1];
				memset(name, '\0', l+1);
				memcpy(name, p + 1, l - 1);
				return name;
			}
			else
				return NULL;
		}
	}
}
