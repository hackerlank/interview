#include "StdAfx.h"
#include "main.h"

#include "misc.h"
#include "config.h"
#include "scene.h"

extern char path_app[MAX_PATH];	

HINSTANCE hInst = NULL;


// ����� �����. 
// ������ ����������� ����� ��������. 
// ���� �������� � ��������� ����� � ������ ���������� � ����������� CDummy.
// ������� ������.
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	//_CrtSetBreakAlloc(1245);
	//InitPaths();
	//SetCurrentDirectory(path_app);
	//CheckPaths();
	//CleanupLogs();
	//LogPaths();
	CheckDefaultConfig();

	srand((unsigned)time(NULL));
	
	hInst = hInstance;	
	scene::InitScene();	
	scene::MainLoop();
	scene::FreeScene();

	EndupLogs();


	return 0;
}