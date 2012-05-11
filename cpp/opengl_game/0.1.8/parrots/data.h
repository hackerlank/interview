//enum GUI_LOAD_STATE { glsDEFINITION, glsCONTROL, glsNO };
//
//const LPSTR GUI_LOAD_STATE_NAMES[] = { "GUIDEFINITION", "GUICONTROL" };
//
//enum GUI_FIELD_TYPE {gftNAME, gftVISIBLE, gftTEXTURE_NAME, gftPATTERN_COUNT, gftPATTERN_CURRENT,
//gftPATTERN_WIDTH, gftPATTERN_HEIGHT, gftX, gftY, gftNEXT, gftACTION,  gftNO };
//
//const LPSTR GUI_FIELD_TYPE_NAMES[] = { "NAME", "VISIBLE", "TEXTURE_NAME", "PATTERN_COUNT",
//"PATTERN_CURRENT", "PATTERN_WIDTH", "PATTERN_HEIGHT", "X", "Y", "NEXT", "ACTION" };
//
//enum RACE_LOAD_STATE { rlsRACEDEFINITION, rlsRACE, rlsNO };
//
//const LPSTR RACE_LOAD_STATE_NAMES[] = { "RACEDEFINITION", "RACE" };
//
//enum RACE_FIELD_TYPE { rftNAME };
//
//const LPSTR RACE_FIELD_TYPE_NAMES[] = { "NAME" };
//
//
//struct GUIDEFINITIONFIELD {
//	GUI_FIELD_TYPE type;
//	LPSTR value;
//};


struct GUI_DEFINITION_ELEM {
	LPSTR type;
	char name[MAX_FIELD_LENGTH];
	bool visible;
	char texture_name[MAX_FIELD_LENGTH];
	int action;
	char caption_font_name[MAX_FIELD_LENGTH];
	char text_font_name[MAX_FIELD_LENGTH];
	int caption_x;
	int caption_y;
	int text_x;
	int text_y;
	BYTE color_red;
	BYTE color_green;
	BYTE color_blue;
	BYTE color_alpha;
	BYTE text_color_red;
	BYTE text_color_green;
	BYTE text_color_blue;
	BYTE text_color_alpha;
	BYTE caption_color_red;
	BYTE caption_color_green;
	BYTE caption_color_blue;
	BYTE caption_color_alpha;
	int pattern_count;
	int pattern_current;
	int pattern_width;
	int pattern_height;
	int x;
	int y;
	char caption[MAX_FIELD_LENGTH];
	char text[MAX_FIELD_LENGTH];
	LPSTR next;
	char parent[MAX_FIELD_LENGTH];
	int data;

	GUI_DEFINITION_ELEM()
	{
		type = NULL;
		memset(name, '\0', MAX_FIELD_LENGTH);
		visible = false;
		memset(texture_name, '\0', MAX_FIELD_LENGTH);
		action = 0;
		memset(caption_font_name, '\0', MAX_FIELD_LENGTH);
		memset(text_font_name, '\0', MAX_FIELD_LENGTH);
		caption_x = -1;
		caption_y = -1;
		text_x = -1;
		text_y = -1;
		color_red = 0;
		color_green = 0;
		color_blue = 0;
		color_alpha = 0;
		text_color_red = 0;
		text_color_green = 0;
		text_color_blue = 0;
		text_color_alpha = 0;
		caption_color_red = 0;
		caption_color_green = 0;
		caption_color_blue = 0;
		caption_color_alpha = 0;
		pattern_count = -1;
		pattern_current = - 1;
		pattern_width = -1;
		pattern_height = -1;
		x = -1;
		y = -1;
		memset(caption, '\0', MAX_FIELD_LENGTH);
		memset(text, '\0', MAX_FIELD_LENGTH);
		//memset(next, '\0', MAX_FIELD_LENGTH);	
		next = NULL;
		memset(parent, '\0', MAX_FIELD_LENGTH);
	}


};

struct GUIDEFINITION {
public:
	std::vector<GUI_DEFINITION_ELEM> elements;
	bool LoadFromFile(char* file_name);
private:
	void Parse(GUI_DEFINITION_ELEM* it, string right, string value);
};

struct RACEDEFINITION {
	std::vector<LPSTR> race_names;
	bool LoadFromFile(char* file_name);
};

struct STARDEFINITION {
	std::vector<LPSTR> star_names;
	bool LoadFromFile(char* file_name);
};

struct GAMEDEFINITION {
	struct RACEDEFINITION races;
	struct STARDEFINITION stars;
	struct GUIDEFINITION gui;
};