


// ������������, �����������
// �� �������, � �������� ����� ����������������� ����� �� ���������� ����
// �������� ����, �����, ���������� ���� � �.�.

void iichan_CreateGame(const char* description_script, config_t* config);
void iichan_DestroyGame(void);

void iichan_ContinueGame();
void iichan_SaveGame();
void iichan_LoadGame();





// ���������� ������� �������, � �������� ����� ����������������� �������

void iichan_ToMenu(int menu_id);
void iichan_SetPause(bool value);
void iichan_LoadLevel(const char* file_name);
void iichan_Exec(const char* src_file); // �������� ������� �� ����������
void iichan_ExecString(const char* src_string); // �������� ���������� �������� �� ����������
void iichan_Restart(void); // ���������� ������
void iichan_StartDialog(const char* src_file);
void iichan_StartBriefing(const char* src_file);
void iichan_HideHUD(void);
void iichan_ShowHUD(void);

// ����� ��� ����� ������ �������
void iichan_StartQuake(int power); // 



// ������� ������� ������, ��������� ������� � �.�.

int iichan_CreateObject(const char* resource_file,
						const object_data* data,
						const object_handler* handler
						int x,
						int y);
// ���������� ID �������
// object_data ����� ����������, � ���������������� �������, ���������, ��������� � ��.
// object_handler ��������� �� �������, ������� ����� ������������ ���� ������ (������ ��� Process())




void iichan_DeleteObject(int id);



// ����� ������ ��� ������, ���� ��� id

object_t* iichan_GetObject(int id);


// �������� ��������� �������� �������
// ������, ������, ��������, ����������
struct object_t {
	int x;
	int y;
	char* resource_file; // ��������� �� ���� ��������, ������
	texture_t* texture;

	object_data data; // ������ �������

	object_handler* handler; // �������-���������� �������

	int id; //
	int type; // ��� �������, ��� iichan_process_objects
};

// ���� �����, �������� �� ��������, ����� �� ��������� ������ ��������
// ���� ����� ���������, � ��������� ����� ����� �������� ����� �����
struct tile_t {
	int x;
	int y;
	char* resource_file;
	texture_t* texture;

	int id;
};




// �������� ������� �������, ������� ������� ����

// iichan global data
bool pause;
bool in_game;
bool at_least_one_player_alive;
// etc.. ��� ��, ��� ��������������� iichan_Tick ����� ����� ������� ����

// � ��, ������ ��������
object_t* objects;
// ���
object_t objects[3500]; // ��������� ������������ ����������



void iichan_Tick(void)
{
	iichan_process_objects();

	// � �����, ��������� ������, �����, ������� � ���� � �.�.

	iichan_process_player();
}

void iichan_Draw(void)
{
	draw_tiles();
	draw_objects();
	draw_messages();
	draw_hud();
}



void iichan_process_objects()
{
	// �������� ����������� ��� ������� �������
	for(uint i = 0; i < objects_count; i++)
	{
		objects[i].object_handler();

		switch(objects[i].type)
		{
		case POWERUP: // ...
		case ENEMY: // ...
		}
	}
}