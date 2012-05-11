// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glaux.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "Winmm.lib") // MIDI library
// Windows Header Files:
#include <windows.h>
#include <string>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;


// TODO: reference additional headers your program requires here
