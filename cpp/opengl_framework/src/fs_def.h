//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
//	Basic definitions, typedefs, etc..
//	This included in almost every file
//-----------------------------------------------------------------------------

#ifndef __FS_DEF_H_
#define __FS_DEF_H_

// ignore 'unknown pragma' warning

#pragma warning(disable : 4068)

// ignore crt warnings

#define _CRT_SECURE_NO_WARNINGS



//?

#define VERSION "0.1"

//?

#define RENDERER "OpenGL"

//?

#define LOGFILE "oorion.log"

//?

#define GAME_NAME "fs_framework"

//?

#define WINDOW_CLASS_NAME "fs_wnd_class"

//?

#define DEFAULT_FONT "default"

//?

#define DEFAULT_FONT_NAME "Courier New"

//?

#define FPS_X 0
#define FPS_Y 0


//?
#define DEBUG_LOGGING



typedef unsigned int uint;

typedef unsigned char byte;

typedef unsigned short word;





// game state type
// game might have multiple state

typedef enum {
	GS_FREE		=	0,
	GS_PLAYING	=	1,
	GS_MENU		=	2,
	GS_PAUSED	=	4,
	GS_EXITING	=	8

} gamestate_t;


// stream for output errors, logging, etc..
// multiple streams also could be

typedef enum {
	S_NULL		=	0,
	S_LOG		=	1,
	S_SCREEN	=	2,
	S_CONSOLE	=	4,
	S_STDERR	=	8

} stream_t;


// point type

typedef struct {
	int x;
	int y;

} pointi_t;

typedef struct {
	float x;
	float y;

} pointf_t;

// vertex type

typedef struct {
	float x;
	float y;

} vertex2f_t;

typedef struct {
	float x;
	float y;
	float z;

} vertex3f_t;

typedef struct {
	int x;
	int y;

} vertex2i_t;

typedef struct {
	int x;
	int y;
	int z;

} vertex3i_t;

// color type

typedef struct {
	byte r;
	byte g;
	byte b;
	byte a;

} color4b_t;

typedef struct {
	byte r;
	byte g;
	byte b;

} color3b_t;

typedef struct {
	float r;
	float g;
	float b;
	float a;

} color4f_t;

typedef struct {
	float r;
	float g;
	float b;

} color3f_t;


// coord type

typedef struct {
	float x;
	float y;

} coord2f_t;

// render param

typedef struct {

	void* remove_me;

} render_param_t;



#endif // __FS_DEF_H_