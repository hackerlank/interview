


// Конструкторы, деструкторы
// Те функции, с которыми может взаимодействовать игрок из интерфейса меню
// Создание игры, пауза, выключение игры и т.д.

void iichan_CreateGame(const char* description_script, config_t* config);
void iichan_DestroyGame(void);

void iichan_ContinueGame();
void iichan_SaveGame();
void iichan_LoadGame();





// Глобальные игровые функции, с которыми могут взаимодействовать скрипты

void iichan_ToMenu(int menu_id);
void iichan_SetPause(bool value);
void iichan_LoadLevel(const char* file_name);
void iichan_Exec(const char* src_file); // отправка скрипта на выполнение
void iichan_ExecString(const char* src_string); // отправка скриптовой сторочки на выполнение
void iichan_Restart(void); // перезапуск уровня
void iichan_StartDialog(const char* src_file);
void iichan_StartBriefing(const char* src_file);
void iichan_HideHUD(void);
void iichan_ShowHUD(void);

// здесб еще будут всякие эффекты
void iichan_StartQuake(int power); // 



// Функции игровой логики, создающие объекты и т.д.

int iichan_CreateObject(const char* resource_file,
						const object_data* data,
						const object_handler* handler
						int x,
						int y);
// возвращаем ID объекта
// object_data будет структурой, с характеристиками объекта, здоровьем, амуницией и пр.
// object_handler указатель на функцию, которая будет обрабатывать этот объект (делать ему Process())




void iichan_DeleteObject(int id);



// Найти нужный нам объект, зная его id

object_t* iichan_GetObject(int id);


// основная структура игрового объекта
// любого, игрока, поверапа, противника
struct object_t {
	int x;
	int y;
	char* resource_file; // указатель на файл описания, ресурс
	texture_t* texture;

	object_data data; // данные объекта

	object_handler* handler; // функция-обработчик объекта

	int id; //
	int type; // тип объекта, для iichan_process_objects
};

// тайл карты, отдельно от объектов, чтобы не создавать лишних итераций
// алсо будем проверять, и рендерить тайлы одной текстуры одним махом
struct tile_t {
	int x;
	int y;
	char* resource_file;
	texture_t* texture;

	int id;
};




// Основные игровые функции, которые двигают игру

// iichan global data
bool pause;
bool in_game;
bool at_least_one_player_alive;
// etc.. это то, чем руководствуется iichan_Tick когда будет двигать игру

// и да, массив объектов
object_t* objects;
// ИЛИ
object_t objects[3500]; // ограничим максимальное количество



void iichan_Tick(void)
{
	iichan_process_objects();

	// а также, обработка выхода, паузы, заходов в меню и т.д.

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
	// вызываем обработчика для каждого объекта
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