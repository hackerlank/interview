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
	OSL_MACHINE_STATE	current_state;

	// ����������
	//vector
} OSL_MACHINE;


