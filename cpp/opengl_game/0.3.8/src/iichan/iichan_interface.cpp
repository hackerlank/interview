//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Other
//
// DESCRIPTION:
// Взаимодействие с другими компонентами программы
//-----------------------------------------------------------------------------


#include "stdafx.h"

#include "iichan_interface.h"


// TODO: по идее надо использовать misc.h/LogToFile для иерархичности
// но по сути написал все заново

void m_log(int stream, const char* message, ...)
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