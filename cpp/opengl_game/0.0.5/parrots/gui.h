#define MAX_CTRL 20
#define MAX_PANELS 10
#define MAX_BUTTONS 3
#define GUI_STATE_COUNT 3
#define GUI_DEFAULT_STATE 0
#define GUI_MOUSEOVER_STATE 1
#define GUI_PRESS_STATE 2

typedef int (*ONCLICK)(int x, int y);
typedef int (*ONMOUSEMOVE)(void);

#define Z_CTRL 100

enum GUIbuttons {GUI_OK, GUI_YESNO};

// GUIcontrol

struct GUIcontrol {
	GUIcontrol();
	GUIcontrol(GUIcontrol* prnt);
	~GUIcontrol();

	GUIcontrol children[MAX_CTRL];
	GUIcontrol* parent;

	int childCount;

	void Add(GUIcontrol ctrl);
	void Delete(GUIcontrol* ctrl);

	int coordX;
	int coordY;
	int Z;
	int parentCoordX;
	int parentCoordY;
	int sizeX;
	int sizeY;

	LPSTR name;

	texture_t* tex;
	int current_state;

	font_t* captionFont;
	LPSTR text;

	int textX;
	int textY;

	BYTE textClrRed;
	BYTE textClrGreen;
	BYTE textClrBlue;
	BYTE textClrAlpha;

	int OnClick();
	int OnMouseMove();

	bool visible;

	void Show(void);
	void Hide(void);
	void Draw(void);

	// here add stuff
};

// GUI

class GUI {
public:
	GUI();
	~GUI();

	GUIcontrol controls[MAX_CTRL];

	int ShowMessage(LPSTR message);
	int ShowDialog(LPSTR message, GUIbuttons btns);
	int ShowMainMenu(void);

	void Draw(void);
	void Tick(void);
	void AddCtrl(GUIcontrol ctrl);
	void DeleteCtrl(GUIcontrol* ctrl);
};






//struct GUIcontrol {
//	GUIcontrol();
//
//	UINT coordX;
//	UINT coordY;
//	UINT sizeX;
//	UINT sizeY;
//
//	UINT parentCoordX;
//	UINT parentCoordY;
//
//	
//	BOOL AntiAlias;
//	BOOL border;
//	BYTE borderWidth;
//
//	BYTE borderClrRed;
//	BYTE borderClrGreen;
//	BYTE borderClrBlue;
//	BYTE borderClrAlpha;
//
//	BYTE borderClrRed2;
//	BYTE borderClrGreen2;
//	BYTE borderClrBlue2;
//	BYTE borderClrAlpha2;
//
//	BOOL body;
//
//	BYTE bodyClrRed;
//	BYTE bodyClrGreen;
//	BYTE bodyClrBlue;
//	BYTE bodyClrAlpha;
//
//	BYTE bodyClrRed2;
//	BYTE bodyClrGreen2;
//	BYTE bodyClrBlue2;
//	BYTE bodyClrAlpha2;
//
//	UINT z;
//	
//	texture_t* tex;
//	UINT tex_default;
//	UINT tex_mouseover;
//	UINT tex_press;
//	UINT tex_pattern_width;
//	UINT tex_pattern_height;
//	UINT tex_current_state;
//	
//	font_t* font;
//	LPSTR text;
//	UINT oldTextX;
//	UINT oldTextY;
//	UINT oldCaptX;
//	UINT oldCaptY;
//	UINT textX;
//	UINT textY;
//	UINT captX;
//	UINT captY;
//	BYTE textClrRed;
//	BYTE textClrGreen;
//	BYTE textClrBlue;
//	BYTE textClrAlpha;
//
//	font_t* caption_font;
//	LPSTR caption;
//	BYTE captClrRed;
//	BYTE captClrGreen;
//	BYTE captClrBlue;
//	BYTE captClrAlpha;
//
//	GUIcontrol* children;
//	UINT countChildren;
//	
//	bool visible;
//
//	void Show(void);
//	void Hide(void);
//
//
//	void Draw(void);
//
//};
//
//struct GUIbutton : GUIcontrol  {
//	texture_t* glyph;
//	bool close;
//};
//
//struct GUIpanel : GUIcontrol {
//	UINT countButton;
//	GUIbutton buttons[MAX_BUTTONS];
//	void AddButton(GUIbutton btn);
//};
//
//// ********************* CONSTANTS & ENUMS ********************
//#define Z_BUTTON 100
//#define Z_DIALOG 150
//
//
//enum GUIButtons {GUI_OK, GUI_OK_CANCEL, GUI_YES, GUI_YES_NO};
//enum GUIIcons {GUI_WARNING, GUI_QUESTION, GUI_NOTIFY};
//
//
////const GUIbutton GUIButtonOk = { 50, 50, 84, 26, 0, 0, false, false, 0, 0, 0, 0, 0, 0, 0, 0, 0, false, 0, 0, 0, 0,
////0, 0, 0, 0, "Ok", 20, 15, 255, 255, 255, 255, 100, Z_BUTTON, &textures[FindByName(
////const GUIpanel GUIShowMessageBox = { "Courier New", 1, {
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	
//	
//
//
//
//
//
//
//
//
//
//
//class GUI {
//public:
//
//	GUI();
//	~GUI();
//
//	void Draw(void);
//	void Tick(void);
//	void AddPanel(GUIpanel pnl);
//	void DestroyPanel(int id);
//
//	// MessageBox: just shows a message
//	//		IN FUTURE: add notification icon, e.g. ICON_EXCLAMATION or ICON_QUESTION
//	void ShowMessage(char message[], char caption[], GUIIcons icons);
//
//	// DialogBox: shows a message, and returns a value
//	//int ShowDialog(char message[], enum GUIButtons buttons, enum GUIIcons icons);
//
//	bool LoadFromFile(char file_name[]);
//
//	UINT countPanel;
//
//	GUIpanel panels[MAX_PANELS];
//};



