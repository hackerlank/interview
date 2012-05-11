//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Фейспалмовые треды^W файлы иичантры
//
// DESCRIPTION:
// Игровые объекты
//-----------------------------------------------------------------------------

#include "iichan_types.h"

// Уникальный идентификатор объекта типа
// Дабы разобрать сколько в памяти чего лежит
#define OBJECT_TAG 333


typedef struct {

	coord2i coord;

	
	bool	active;


	uint	flags;


	void*	data;


	object_t* prev;
	object_t* next;


} object_t;



object_t* i_CreateObject(int x, int y, resource_t* res);


void i_DeleteObject(object_t* obj);


void i_ConstructObject(object_t* obj, resource_t* res);



// ---

void i_UpdateObjects(void);
void i_RenderObjects(void);







