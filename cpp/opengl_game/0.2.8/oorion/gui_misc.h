// GID - Уникальный идентификатор каждого контрола
typedef int GID;

// Цвет
typedef struct _COLOR4 {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
} COLOR4;

// Цвет сообщения на экране
const COLOR4 DEFAULT_SCREEN_MESSAGE_COLOR = { 255, 255, 255, 255 };

// Цвет контрола по-умолчанию
const COLOR4 DEFAULT_CONTROL_COLOR = { 255, 255, 255, 255 };

// Цвет надписи на контроле по-умолчанию
const COLOR4 DEFAULT_TEXT_COLOR = { 255, 255, 255, 255 };

// Максимальная длина текстового поля
const UINT GUI_TEXTBOX_TEXT_LENGTH = 256;

// Сигналы
//	Это служебные сообщения, которые посылает контрол (влияет на всю игру
//	в целом). Остальные сообщения используются только внутри гуя
// --------------------------------------------------------------------------
const UINT GUI_SGN_OPEN_GROUP	= 550; // Открыть группу элементов
const UINT GUI_SGN_EXIT			= 650; // Выход (из программы)		
const UINT GUI_SGN_SET_VAR		= 750; // Установить значение сист. перем.

	// Возможные значения GUI_SGN_SET_VAR:
	const UINT GUI_SET_DIFF			= 200; // Установить сложность
	const UINT GUI_SET_GALAXY_SIZE	= 210; //	размер галактики
	const UINT GUI_SET_RACE			= 220; //	расу
	const UINT GUI_SET_OPP_COUNT	= 230; //	количество оппонентов
	const UINT GUI_SET_GAME_STATE	= 240; //	состояние игры

const UINT GUI_SGN_START_GAME	= 850; // Начать игру
// --------------------------------------------------------------------------