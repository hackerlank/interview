#include "stdafx.h"
#include "misc.h"
#include <direct.h>


CHAR path_log[MAX_PATH];
CHAR path_config[MAX_PATH];
CHAR path_app[MAX_PATH];
CHAR path_textures[MAX_PATH];
CHAR path_fonts[MAX_PATH];
CHAR path_protos[MAX_PATH];
CHAR path_scripts[MAX_PATH];
CHAR path_levels[MAX_PATH];
//char path_dialogs[MAX_PATH];




BOOL DirectoryExists(const char* path)
{
	return (GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}

// Возвращает имя файла без расширения,
// т.е. для строку "textures/sky.bmp" вернет "sky".
// Выделяет в памяти новую строку, не забывать удалить.
char* GetNameFromPath(const char* path)
{
	if(!path)
		return NULL;

	char* p = (char*)path + strlen(path);

	UINT l = 0;

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

// Возращает расширение файла (ищет первую точку от конца).
// Не формирует новую строку, а возвращает указатель на подстроку в исходной.
char* ExtFromPath(const char* path)
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


bool FileExists(const char* file_name)
{
	return (GetFileAttributes(file_name) != INVALID_FILE_ATTRIBUTES);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Служебные функции программы

// Очищает логи
void CleanupLogs()
{
	static char* cleanup_msg = "Log started";

	//_LogToFileTrunc(DEFAULT_LOG_NAME, cleanup_msg);
	//_LogToFileTrunc(DEFAULT_GUI_LOG_NAME, cleanup_msg);
	//_LogToFileTrunc(DEFAULT_SCRIPT_LOG_NAME, cleanup_msg);
	CreateLogFile(DEFAULT_LOG_NAME);
	CreateLogFile(DEFAULT_SCRIPT_LOG_NAME);
}

// Вставляет в логи строку, указывающую на завершение логирования
void EndupLogs()
{
	static char* endup_msg = "Finished logging";

	//_LogToFile(DEFAULT_LOG_NAME, endup_msg);
	//_LogToFile(DEFAULT_GUI_LOG_NAME, endup_msg);
	//_LogToFile(DEFAULT_SCRIPT_LOG_NAME, endup_msg);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "%s", endup_msg);
	sLog(DEFAULT_SCRIPT_LOG_NAME, LOG_INFO_EV, "%s", endup_msg);
}

// Удаляет имя файла из строки, содержащей полное имя файла
void DeleteFileNameFromEndOfPathToFile(char *src)
{
	UINT i = strlen(src)-1;
	while(src[i] != '/' && src[i] != '\\' && src[i] != ':')
		i--;
	src[i+1] = 0;
}


// Задает пути
void InitPaths(void)
{
	// application
	GetModuleFileName(GetModuleHandle(0), path_app, MAX_PATH);
	DeleteFileNameFromEndOfPathToFile(path_app);
	SetCurrentDirectory(path_app);

	GetCurrentDirectory(MAX_PATH, path_app);

	// log
	strcpy(path_log, path_app);
	strcat(path_log, DEFAULT_LOG_PATH);

	// config
	strcpy(path_config, path_app);
	strcat(path_config, DEFAULT_CONFIG_PATH);

	// textures
	strcpy(path_textures, path_app);
	strcat(path_textures, DEFAULT_TEXTURES_PATH);

	// fonts
	strcpy(path_fonts, path_app);
	strcat(path_fonts, DEFAULT_FONTS_PATH);

	// protos
	strcpy(path_protos, path_app);
	strcat(path_protos, DEFAULT_PROTO_PATH);

	// scripts
	strcpy(path_scripts, path_app);
	strcat(path_scripts, DEFAULT_SCRIPTS_PATH);

	// levels
	strcpy(path_levels, path_app);
	strcat(path_levels, DEFAULT_LEVELS_PATH);
}


// Процедура проверяет существует ли указанные пути.
// Если их нет, создает их.
void CheckPaths()
{
	if(!DirectoryExists(path_log))
	{
		_mkdir(path_log);
	}
	if(!DirectoryExists(path_config))
	{
		_mkdir(path_config);
	}
	if(!DirectoryExists(path_textures))
	{
		_mkdir(path_textures);
	}
	if(!DirectoryExists(path_fonts))
	{
		_mkdir(path_fonts);
	}
	if(!DirectoryExists(path_protos))
	{
		_mkdir(path_protos);
	}
	if(!DirectoryExists(path_scripts))
	{
		_mkdir(path_scripts);
	}
	if(!DirectoryExists(path_levels))
	{
		_mkdir(path_levels);
	}
}


// Занесение в лог путей
void LogPaths(void)
{
#ifdef DISABLE_LOG
	return;
#endif

	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Game path:\t\t%s", path_app);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Log path:\t\t%s", path_log);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Config path:\t\t%s", path_config);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Textures path:\t%s", path_textures);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Fonts path:\t\t%s", path_fonts);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Prototypes path:\t%s", path_protos);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "Scripts path:\t\t%s", path_scripts);
}


// Процедура проверяет существует ли конфиг DEFAULT_CONFIG_NAME в директории path_config.
// Если конфига нет, создает его, указывая статические данные.
void CheckDefaultConfig()
{
	char path_to_cfg[MAX_PATH];

	sprintf(path_to_cfg, "%s%s\0", path_config, DEFAULT_CONFIG_NAME);
	if(!FileExists(path_to_cfg))
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "%s does not exist. Recreating...", path_to_cfg);
		ofstream f;
		f.open(path_to_cfg, ios::out);

		if(f.is_open())
		{
			f << "config = \n";
			f << "{";
			f << "\twindow_width = 640;\n";
			f << "\twindow_height = 480;\n";
			f << "\tscr_width = 640;\n";
			f << "\tscr_height = 480;\n";
			f << "\tnear_z = -1;\n";
			f << "\tfar_z = 1;\n";
			f << "\tbpp = 16;\n";
			f << "\tfullscreen = 0;\n";
			f << "\tvert_sync = 0;\n";
			f << "\tdebug = 0;\n";
			f << "\tshow_fps = 1;\n\n";
			f << "\t-- controls\n\n";
			f << "\tup = keys.up;\n";
			f << "\tdown = keys.down;\n";
			f << "\tleft = keys.left;\n";
			f << "\tright = keys.right;\n";
			f << "\tjump = keys.z;\n";
			f << "\tsit = keys.x;\n";
			f << "\tfire = keys.ctrl;\n";
			f << "\tchange_weapon = keys.a;\n";
			f << "\tchange_player = keys.s;\n";
			f << "\tscreenshot = keys.f11;\n";
			f << "\tshow_menu = keys.esc;\n";
			f << "}";
			f.close();
			sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "%s succesfully recreated", path_to_cfg);
		}
		else
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to recreate %s", path_to_cfg);
	}


}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Satanlog
bool LogEnabled = true;
char LogFile[2048];				// Буфер для полного имени файла лога
CDummy InitLog = CDummy();

CDummy::CDummy(void)
{
	// DEBUG_STOP_ON_ALLOC_NUM определяется в misc.h
	// Там ее искать легче :)
#ifdef DEBUG_STOP_ON_ALLOC_NUM
	_CrtSetBreakAlloc(DEBUG_STOP_ON_ALLOC_NUM);
#endif // DEBUG_STOP_ON_ALLOC_NUM

	InitPaths();
	SetCurrentDirectory(path_app);
	CheckPaths();
	CleanupLogs();
	LogPaths();
}

CDummy::~CDummy(void)
{
#ifdef _DEBUG 
	_CrtDumpMemoryLeaks(); 
#endif
}

// Создает новый файл лога (или очищает старый)
// fname - имя файла вида "\\file_name.log"
void CreateLogFile(char *fname)
{
#ifdef DISABLE_LOG
	return;
#endif

	if (!LogEnabled)
		return;

	FILE *hf = NULL;

	memset(LogFile, '\0', strlen(path_log) + strlen(fname) + 1);
	sprintf(LogFile, "%s%s\0", path_log, fname);

	hf = fopen(LogFile, "w");
	if (!hf)
	{
		char buf[2048];
		sprintf(buf, "Failed to open log-file: %s", LogFile);
		MessageBox(NULL, buf, NULL, NULL);
		return;
	}

	__time64_t long_time;
	_time64( &long_time ); 
	struct tm *newtime;
	newtime = localtime(&long_time);
	char buff[256];
	memset(buff,0,256);
	strcpy(buff, asctime(newtime));
	for (UINT i=0; i<strlen(buff); i++)
	{
		if (buff[i]<=13)
			buff[i] = 0;
	}

	fprintf(hf, "[%s] [%s] Log file \"%s\" created\n", buff, LOG_INFO_EV, fname);

	fclose(hf);
}


// Запись в лог
// fname - имя файла вида "\\file_name.log"
// Event - описание записи в логе
// Format - строка формата сообщения (подобно printf)
void sLog(const char* fname, const char* Event, const char* Format,...)
{
#ifdef DISABLE_LOG
	return;
#endif

	if (!LogEnabled)
		return;


	memset(LogFile, '\0', strlen(path_log) + strlen(fname) + 1);
	sprintf(LogFile, "%s%s\0", path_log, fname);

	
	FILE *hf = NULL;
	hf = fopen(LogFile, "a");
	if (!hf)
	{
		char buf[2048];
		sprintf(buf, "Failed to open log-file: %s", LogFile);
		MessageBox(NULL, buf, NULL, NULL);
		return;
	}
	

	__time64_t long_time;
	_time64( &long_time ); 
	struct tm *newtime;
	newtime = localtime(&long_time);
	char buff[256];
	memset(buff,0,256);
	strcpy(buff,asctime(newtime));
	for (UINT i=0; i<strlen(buff); i++)
	{
		if (buff[i]<=13)
			buff[i] = 0;
	}
	fprintf(hf,"[%s] [%s] ", buff, Event);

	va_list ap;
	va_start(ap, Format);
	vfprintf(hf, Format, ap);
	va_end(ap);
	fprintf(hf,"\n");
	fclose(hf);

}

CFile::CFile(char *filename, int mode)
{
	if (mode == CFILE_READ)
		file = fopen(filename, "rb");
	else
		if (mode == CFILE_WRITE)
			file = fopen(filename, "wb");
		else
		{
			file = NULL;
			sLog(DEFAULT_LOG_NAME, "WARNING", "Can't open file %s", filename);
		}
}

bool CFile::Open(const char *filename, int mode)
{
	if (filename == NULL)
	{
		sLog(DEFAULT_LOG_NAME, "WARNING", "Can't open file. Invalid filename");
		return false;
	}
	if (file != NULL)
	{
		sLog(DEFAULT_LOG_NAME, "WARNING", "Can't open file %s: another file is already opened.", filename);
		return false;
	}
	if (mode == CFILE_READ)
	{
		file = fopen(filename, "rb");
		if (file == NULL)
		{
			sLog(DEFAULT_LOG_NAME, "WARNING", "Can't open file %s.", filename);
			return false;
		}
		return true;
	}
	if (mode == CFILE_WRITE)
	{
		file = fopen(filename, "wb");
		if ( file == NULL )
		{
			sLog(DEFAULT_LOG_NAME, "WARNING", "Can't open file %s.", filename);
			return false;
		}
		return true;
	}
	if (mode!=CFILE_READ && mode!=CFILE_WRITE)
	{
		sLog(DEFAULT_LOG_NAME, "WARNING", "Can't open file %s: invalid mode.", filename);
		return false;
	}
	return true;
}

bool CFile::Close()
{
	if (file == NULL)
		return false;
	fclose(file);
	return true;
}

bool CFile::ReadByte(pbyte buffer)
{
	if (buffer == NULL)
		return false;
	if (file == NULL)
		return false;
	if (fread(buffer, 1, 1, file) != 1)
	{
		if (!Eof())
			sLog(DEFAULT_LOG_NAME, "WARNING", "FILE IO Error. Can't read byte.");
		return false;
	}
	return true;
}

bool CFile::Write(const void* buffer, DWORD nbytes)
{
	if (buffer == NULL)
		return false;
	if (file == NULL)
		return false;
	if (nbytes == 0)
		return false;
	if (fwrite(buffer, 1, nbytes, file) != nbytes)
	{
		if (!Eof())
			sLog(DEFAULT_LOG_NAME, "WARNING", "FILE IO Error. Can't write data.");
		return false;
	}
	return true;
}

bool CFile::WriteByte(pbyte buffer)
{
	if (buffer == NULL)
		return false;
	if (file == NULL)
		return false;
	if (fwrite(buffer, 1, 1, file) != 1)
	{
		if (!Eof())
			sLog(DEFAULT_LOG_NAME, "WARNING", "FILE IO Error. Can't write byte.");
		return false;
	}
	return true;
}

bool CFile::WriteByte(byte buffer)
{
	if (file == NULL)
		return false;
	if (fwrite(&buffer, 1, 1, file) != 1)
	{
		if (!Eof())
			sLog(DEFAULT_LOG_NAME, "WARNING", "FILE IO Error. Can't write byte.");
		return false;
	}
	return true;
}

bool CFile::Read(void* buffer, DWORD nbytes)
{
	if (buffer == NULL)
		return false;
	if (file == NULL)
		return false;
	if (nbytes == 0)
		return false;
	if (fread(buffer, 1, nbytes, file) != nbytes)
	{
		if (!Eof())
			sLog(DEFAULT_LOG_NAME, "WARNING", "FILE IO Error. Can't read data.");
		return false;
	}
	return true;
}

bool CFile::ReadLine(char* &data)
{
	if (file == NULL)
		return false;

	char *buffer = new char[CFILE_MAX_STRING_LENGTH];
	int count = -1;
	DWORD read = 0;
	do
	{
		fread(buffer+count+1, 1, 1, file);
		count++;
	} while ((buffer[count] != 0x00) && (count < CFILE_MAX_STRING_LENGTH));
	if (data != NULL)
		delete [] data;
	data = new char[count+1];
	for(int i=0; i<count; i++)
		data[i] = buffer[i];
	data[count] = 0x00;
	delete [] buffer;
	return true;
}

//note buffer must exists!!!!
bool CFile::ReadString(char* buffer)
{
	if (file == NULL)
		return false;
	byte b;
	int i = 0;
	if (!Read(&b,1))
		return false;
	while (b != 0)
	{
		buffer[i] = b;
		i++;
		if (!Read(&b,1))
			return false;
	}
	return true;
}

bool CFile::WriteString(const char* buffer)
{
	if (file == NULL)
		return false;
	byte b = 0;
	for (unsigned int i = 0;i < strlen(buffer);i++)
	{
		Write(&buffer[i], 1);
	}
	Write(&b, 1);
	return true;
}

bool CFile::ReadString(string &buffer)
{
	if (file == NULL)
		return false;
	byte b;
	buffer = "";
	int i = 0;
	if (!Read(&b, 1))
		return false;
	while (b != 0)
	{
		buffer += b;
		i++;
		if (!Read(&b, 1))
			return false;
	}
	return true;
}

bool CFile::WriteString(const string buffer)
{
	if (file == NULL)
		return false;
	byte b = 0;
	for (unsigned int i = 0;i<buffer.length();i++)
	{
		Write(&buffer[i], 1);
	}
	Write(&b, 1);
	return true;
}

bool CFile::Writeln(string buffer)
{
	if (file==NULL)
		return false;
	char b=13;
	Write((char*)buffer.data(), (DWORD)buffer.length());
	Write(&b,1);
	//for windows only 'cause of 13 10=)
#ifdef WIN32
	b = 10;
	Write(&b,1);
#endif
	return true;
}

DWORD CFile::Size()
{
	// Size return goes here.	
	return 0;
}

bool CFile::Eof()
{
	return !!feof(file);
}
bool CFile::Seek(unsigned int offset, byte kind)
{
	if (fseek(file, offset, kind))
		return true;
	return false;
}
