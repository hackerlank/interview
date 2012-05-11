//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Program entry
//
// DESCRIPTION:
//	Here is main() function
//	
//-----------------------------------------------------------------------------

#include "stdafx.h"

int		myargc		=	0;
char*	myargv		=	0;

HINSTANCE hInst		=	NULL;

// for main loop

#include "fs_main.h"



int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{

	myargv = lpCmdLine;
	myargc = 0; // fix it
	hInst = hInstance;

	G_MainLoop();

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif


	return 0;
}