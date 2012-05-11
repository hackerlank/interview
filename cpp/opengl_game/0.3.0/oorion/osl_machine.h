struct OSL_OBJECT {
	OSL_OBJECT& linked_to;
};







enum OSL_MACHINE_STATE {
	omsIF,
	omsWHILE,
	omsFUNCTION,
	omsMATH
};

typedef struct _OSL_MACHINE {

	// ���������� ����������
	string				current_file;
	string				current_function;
	UINT				current_line;
	const string		version = "0.1";

	// ���������� ������������
	vector<string> OSL_VAR_TYPES;
	vector<string> OSL_OPERATORS;
	vector<string> OSL_COMPARISONS;
	vector<string> OSL_SYSFUNCTIONS;
	vector<string> OSL_KEYWORDS;

	// ���������� �������� � ����������
	vector<OSL_VAR> OSL_CONSTANTS;
	vector<OSL_VAR> OSL_VARIABLES;

	// ������
	bool ExecuteFile(const char* file_name);

	void DeclareVar(OSL_VAR var);
	void UndeclareVar(string var_name);

	void DeclareConstant(string const_name, OSL_VAR_DATA value);
	void DeclareConstant(OSL_VAR constant);
	void UndeclareConstant(string const_name);




	








} OSL_MACHINE;


