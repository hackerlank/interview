bool Load_v_04(const char* file_name);
bool Load_v_03(const char* file_name);
bool Load_v_01(char* file_name);
bool Load(char* file_name);
void InitStorages();

typedef void (*GLFUNCTION)(...);


typedef struct tagGL_CONSTANT_DEFINITION {
	string name;
	int value;
} GL_CONSTANT_DEFINITION;

typedef struct tagGL_CONSTANTS_STORAGE {
	vector<GL_CONSTANT_DEFINITION> fields;

	int ByString(string s)
	{
		for(std::vector<GL_CONSTANT_DEFINITION>::iterator it = fields.begin();
			it != fields.end();
			it++)
			if((*it).name == s)
				return (*it).value;

		return -1;
	}

	string ByValue(int v)
	{
		for(std::vector<GL_CONSTANT_DEFINITION>::iterator it = fields.begin();
			it != fields.end();
			it++)
			if((*it).value == v)
				return (*it).name;

		return "NotFound";
	}

	void Add(string s, int v)
	{
		GL_CONSTANT_DEFINITION* aField = new GL_CONSTANT_DEFINITION();

		aField->name = s;
		aField->value = v;

		fields.push_back(*aField);
	}
} GL_CONSTANTS_STORAGE;

typedef struct tagGL_FUNCTION_DEFINITION {
	string name;
	GLFUNCTION function;
	int param_count;
} GL_FUNCTION_DEFINITION;

typedef struct tagGL_FUNCTIONS_STORAGE {
	vector<GL_FUNCTION_DEFINITION> fields;

	GLFUNCTION ByName(string s)
	{
		for(std::vector<GL_FUNCTION_DEFINITION>::iterator it = fields.begin();
			it != fields.end();
			it++)
			if((*it).name == s)
				return (*it).function;
	}

	void Add(string n, GLFUNCTION f, int pc)
	{
		GL_FUNCTION_DEFINITION* aField = new GL_FUNCTION_DEFINITION();

		aField->name = n;
		aField->function = f;
		aField->param_count = pc;

		fields.push_back(*aField);
	}

} GL_FUNCTIONS_STORAGE;

enum KEYWORD { kwGuiDef, kwRaceDef, kwStarDef, kwFor, kwWhile, kwDo, kwIf, kwStatement, kwNo };
enum CONDITION { LESS, LESSEQUAL, MORE, MOREEQUAL, NOCOND };

typedef struct tagKEYWORD_STORAGE_FIELD {
	string name;
	KEYWORD key;
} KEYWORD_STORAGE_FIELD;

typedef struct tagKEYWORD_STORAGE {
	vector<KEYWORD_STORAGE_FIELD> fields;

	KEYWORD ByName(string s)
	{
		for(std::vector<KEYWORD_STORAGE_FIELD>::iterator it = fields.begin();
			it != fields.end();
			it++)
			if((*it).name == s)
				return (*it).key;
		return kwNo;
	}

	void Add(string n, KEYWORD k)
	{
		KEYWORD_STORAGE_FIELD* aField = new KEYWORD_STORAGE_FIELD();

		aField->name = n;
		aField->key = k;

		fields.push_back(*aField);
	}
} KEYWORD_STORAGE;



typedef struct tagGL_DEFINITION_STORAGE {
	GL_FUNCTIONS_STORAGE functions;
	GL_CONSTANTS_STORAGE constants;
} GL_DEFINITION_STORAGE;


