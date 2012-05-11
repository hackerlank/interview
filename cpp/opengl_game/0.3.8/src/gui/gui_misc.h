// GID - Уникальный идентификатор каждого контрола
typedef int GID;

// Цвет
struct COLOR4 {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
};

// Цвет сообщения на экране
const COLOR4 DEFAULT_SCREEN_MESSAGE_COLOR = { 255, 255, 255, 255 };

// Цвет контрола по-умолчанию
const COLOR4 DEFAULT_CONTROL_COLOR = { 255, 255, 255, 255 };

// Цвет надписи на контроле по-умолчанию
const COLOR4 DEFAULT_TEXT_COLOR = { 255, 255, 255, 255 };

// Фоновый цвет контрола по-умолчанию
const COLOR4 DEFAULT_BACKGROUND_COLOR = { 0, 0, 0, 0 };


// Максимальная длина текстового поля
const UINT GUI_TEXTBOX_TEXT_LENGTH = 256;

// Сигналы
//	Это служебные сообщения, которые посылает контрол (влияет на всю игру
//	в целом). Остальные сообщения используются только внутри гуя
// --------------------------------------------------------------------------
const UINT GUI_SGN_OPEN_GROUP	 = 550; // Открыть группу элементов
const UINT GUI_SGN_CLOSE_PROGRAM = 650; // Выход (из программы)	
const UINT GUI_SGN_EXIT_GAME	 = 652; // Выход из игры
//const UINT GUI_SGN_SET_VAR		 = 750; // Установить значение сист. перем.
const UINT GUI_SGN_START_GAME	 = 850; // Начать игру
const UINT GUI_SGN_CONTINUE_GAME = 855; // Продолжить игру
//const UINT GUI_SGN_LOAD_LEVEL	 = 950; // Загрузить уровень
// --------------------------------------------------------------------------