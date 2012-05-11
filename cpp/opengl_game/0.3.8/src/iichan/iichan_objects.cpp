//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Фейспалмовые треды^W файлы иичантры
//
// DESCRIPTION:
// Игровые объекты
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "iichan_objects.h"

#include "iichan_memory.h"


#define _DEBUG_OBJECTS		// Отладка объектов, логгинг


// Транк всех объектов
object_t* OBJECT_TRUNC		=	NULL;

// Текущий указатель в транке
object_t* P_OBJECT_TRUNC	=	NULL;

uint OBJECT_COUNT = 0;


object_t* i_CreateObject(int x, int y, resource_t* res)
{
	
	// Выделяем блок памяти для объекта
	mem_handle* block = mem_NewAlloc(
		sizeof(object_t),
		OBJECT_TAG,
#ifdef MEMORY_USE_DESC
		"New object", 
#endif
		NULL);

	if(!block)
	{
		// Не удалось создать объект
#ifdef _DEBUG_OBJECTS
		m_log(S_LOG, "Failed to create at object at (%d, %d), resource: %X", x, y, resource);
#endif
		return NULL;
	}

	// Создаем объект
	object_t* new_obj = (object_t*)(block->memory);
	ZeroMemory(new_obj, sizeof(object_t));

	new_obj->coord.x = x;
	new_obj->coord.y = y;

	new_obj->active = true;

	new_obj->data = 0;

	new_obj->prev = NULL;
	new_obj->next = NULL;

	if(res)
	{
		// Если в функцию передан ресурс, то загружаем объект из ресурса
		i_LoadObject(new_obj, res);
	}

	if(OBJECT_COUNT == 0)
	{
		// Если это первый объект
		// Указываем TRUNC на этот объект
		OBJECT_TRUNC = P_OBJECT_TRUNC = new_obj;
	}
	else
	{
		// Объект не первый, добавляем его в транк
		P_OBJECT_TRUNC->next = new_obj;
		new_obj->prev = P_OBJECT_TRUNC;
		P_OBJECT_TRUNC = P_OBJECT_TRUNC->next;
	}

	//
	OBJECT_COUNT++;

	// Пишем лог
#ifdef _DEBUG_OBJECTS
	m_log(S_LOG, "Created new object at (%d, %d), resource: %X", x, y, resource);
#endif


	//
	return new_obj;

}


void i_DeleteObject(object_t* obj);


void i_ConstructObject(object_t* obj, resource_t* res);