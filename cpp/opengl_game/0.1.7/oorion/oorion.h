// FONT WEIGHT

//FW_THIN 100
//FW_EXTRALIGHT 200
//FW_ULTRALIGHT 200
//FW_LIGHT 300
//FW_NORMAL 400
//FW_REGULAR 400
//FW_MEDIUM 500
//FW_SEMIBOLD 600
//FW_DEMIBOLD 600
//FW_BOLD 700
//FW_EXTRABOLD 800
//FW_ULTRABOLD 800
//FW_HEAVY 900
//FW_BLACK 900

#ifdef OORION_EXPORTS
#define OORION_API __declspec(dllexport)
#else
#define OORION_API __declspec(dllimport)
#endif

OORION_API const UINT DEFAULT_SCR_WIDTH = 640;
OORION_API const UINT DEFAULT_SCR_HEIGHT = 480;
OORION_API const unsigned char DEFAULT_SCR_BPP = 16;
OORION_API const bool DEFAULT_SCR_FULLSCREEN = false;
OORION_API const bool DEFAULT_SCR_VERT_SYNC = false;

OORION_API const UCHAR MAX_FONTS = 10;
OORION_API const UINT MAX_TEXTURES = 500;

OORION_API bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp);


// Huffman compressing
// --------------------------
OORION_API void CompressFile(char src[], char dest[]);
OORION_API void DecompressFile(char src[], char dest[]);
// --------------------------

typedef void (*LPSEARCHFUNC)(LPCTSTR lpszFileName);

















