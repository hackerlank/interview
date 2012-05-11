#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

// Disable deprecations and warnings for unsafe functions
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE


// Memory leak detection (in _DEBUG only)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>

#include <vector>

#include <assert.h>

using namespace std;