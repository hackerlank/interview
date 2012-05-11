// oorion.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "oorion.h"
#include "bmp.h"













extern bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp)
{
	AUX_RGBImageRec bmp;
	long size;
	char file_name[100];

	static int counter = 0;
	static int startup_time;
	static int last_time;
	int now_time = 0;

	//if(counter == 0)
	//{
	//	startup_time = GetTickCount();
	//	last_time = GetTickCount() - startup_time;
	//}
	//else
	//{
		//now_time = GetTickCount() - startup_time;
	//}

	//if(now_time - last_time >= 50 || counter == 1)
	//{
		counter++;
		//LogToFile("debug.log", "Saving screenshot: " + string(file_name));
		//last_time = GetTickCount() - startup_time;

		// saving
		sprintf(file_name, "screenshot%d.bmp", counter++);

		bmp.sizeX = scr_width;
		bmp.sizeY = scr_height;

		size = bmp.sizeX * bmp.sizeY * int(scr_bpp / 8);

		bmp.data = new GLubyte[size];

		glReadPixels(0, 0, bmp.sizeX, bmp.sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.data);

		if(!bmpWrite(&bmp, file_name) == 0)
		{
			delete[] bmp.data;
			return false;
		}
		else
		{
			delete[] bmp.data;
			return true;
		}

	 //}

	return false;
}

extern bool SearchFiles(LPCSTR lpszFileName, bool bInnerFolders, LPSEARCHFUNC lpSearchFunc)
{
    LPTSTR part;
    char tmp[MAX_PATH]; // временный массив
    char name[MAX_PATH];
	size_t ext_len;
	string ext;

    HANDLE hSearch = NULL;
    WIN32_FIND_DATA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

    // сначала поиск внутри вложенных папках ...
    if(bInnerFolders)
    {
        if(GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return false;
        strcpy(name, part);
        strcpy(part, "*.*");

        // если папки существуют, то делаем поиск
        wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
        do
        {
            // в каждой папке есть две папки с именами "." и ".."
            // и эти папки мы не трогаем

            // пропускаем папки "." и ".."
            if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))            
            continue;
        
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // если мы нашли папку
            {
                char next[MAX_PATH];
                if(GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return false;
                strcpy(part, wfd.cFileName);
                strcat(next, "\\");
                strcat(next, name);

                SearchFiles(next, true, lpSearchFunc);
            }
        }
        while (FindNextFile(hSearch, &wfd)); // ищем следующий файл

        FindClose (hSearch); // заканчиваем поиск
    }

    if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE) 
        return true; // в противном случае выходим
    do
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // если мы нашли файл
    {
        char file[MAX_PATH];
        if(GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return false;
        strcpy(part, wfd.cFileName);
		ext_len = strlen(file);
		ext = string(file).substr(string(file).length()-3,3);

#ifdef _DEBUG
		//LogToFile("debug.log", "File is " + string(file));
		//LogToFile("debug.log", "File extension is " + ext);
#endif
        //if(ext == "bmp") OnFoundBMP(file);
    }
    while (FindNextFile(hSearch, &wfd)); // ищем следующий файл
    FindClose (hSearch); // заканчиваем поиск

    return true;
}





