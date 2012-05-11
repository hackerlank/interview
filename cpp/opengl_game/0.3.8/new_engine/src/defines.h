#ifndef __DEFINES_H_
#define __DEFINES_H_


//////////////////////////////////////////////////////////////////////////
#define DELETEARRAY(x)		if (x) { delete[] x;	x = NULL; }
#define DELETESINGLE(x)		if (x) { delete x;		x = NULL; }

//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#define __INLINE __forceinline
#else
#define __INLINE inline
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Умолчания

//  Стандартный размер окна
#define DEFAULT_WINDOW_WIDTH		640
#define DEFAULT_WINDOW_HEIGHT		480

// Стандартные пути и файлы
#define DEFAULT_CONFIG_PATH				"\\config\\"
#define DEFAULT_CONFIG_NAME				"\\default.lua"

#define DEFAULT_TEXTURES_PATH			"\\textures\\"
#define DEFAULT_FONTS_PATH				"\\fonts\\"
//#define DEFAULT_DATA_PATH				"\\data"
#define DEFAULT_PROTO_PATH				"\\proto\\"
#define DEFAULT_SCRIPTS_PATH			"\\scripts\\"
#define DEFAULT_LOG_PATH				"\\log\\"
#define DEFAULT_LEVELS_PATH				"\\levels\\"
//#define DEFAULT_DIALOGS_PATH			"\\dialogs"

#define DEFAULT_LOG_NAME				"\\iichantra.log"
#define DEFAULT_GUI_LOG_NAME			"\\gui.log"
#define DEFAULT_SCRIPT_LOG_NAME			"\\script.log"
//#define DEFAULT_CONS_LOG_NAME			"\\console.log"

#define DEFAULT_INIT_SCRIPT				"init.lua"

#define DEFAULT_FONT					"default"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define LOG_ERROR_EV					"ERROR" 
#define LOG_WARNING_EV					"WARNING"
#define LOG_INFO_EV						"INFO"
#define LOG_SCRIPT_EV					"SCRIPT"


//////////////////////////////////////////////////////////////////////////
enum ProgrammStates {
	PROGRAMM_RUNNING,
	PROGRAMM_INACTIVE,
	//GAME_RUNNING,
	//GAME_GAMEMENU,
	//GAME_PAUSED,
	PROGRAMM_EXITING,
};

//////////////////////////////////////////////////////////////////////////
#define GAMENAME "iiСhantra"
#define VERSION "iiСhantra kernel_engine"
#define ENGINE "v. 0.0.1"

//////////////////////////////////////////////////////////////////////////

#define KEYDOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//////////////////////////////////////////////////////////////////////////

#define DEBUG_PRINT
#define TWO_CHANGABLE_PLAYERS


#endif // __DEFINES_H_