#include "stdafx.h"
#include "effects.h"

BYTE GLOBAL_COLOR_RED		= 0;
BYTE GLOBAL_COLOR_GREEN		= 0;
BYTE GLOBAL_COLOR_BLUE		= 0;
BYTE GLOBAL_COLOR_ALPHA		= 0;

BOOL GLOBAL_QUAKE_ENABLED	= 0;
int GLOBAL_QUAKE_POWER		= 0;


void ApplyQuake()
{
	int cf = rand()%5 + 1;
	glTranslatef((float)cf, (float)cf, 0);
}