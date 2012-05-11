//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Engine
//
// DESCRIPTION:
// Выделение памяти, менеджер памяти
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "iichan_memory.h"

mem_handle* MEM_TRUNC			=	NULL;	// Транк выделенных блоков
mem_handle* P_MEM_TRUNC			=	NULL;	// Текущее положение в TRUNC'е


mem_handle* MEM_FREE_TRUNC		=	NULL; // Транк свободных блоков
mem_handle* P_MEM_FREE_TRUNC	=	NULL; // Текущее положение в транке


uint	MEMORY_USED			=	0;
uint	MEMORY_FREE			=	0;
uint	MEMORY_ALLOCATED	=	0;

uint	BLOCKS_USED			=	0;
uint	BLOCKS_FREE			=	0;
uint	BLOCKS_ALLOCATED	=	0;






mem_handle* mem_NewAlloc(
	  uint size,		// Размер требуемой памяти в байтах
	  uint tag,		// Пока хз, что то вроде метки блока, для игровой логики
#ifdef MEMORY_USE_DESC
	  const char* why,	// Причина, по которой этот блок выделяется
#endif
	  const void** watcher	// Наблюдатель, если watcher не NULL, то этот блок будет
				// освобожден, если watcher мертв
	  )
{

		
	// Проверка параметров на валидность
	if(size == 0)
		return NULL;

	// 'why' может быть равен NULL впрочем
	
	// ------------------------------------------------------------------
	// Получаем память

	// Сначала смотрим по свободным блокам
	mem_handle* free_block = NULL;

	if(BLOCKS_FREE > 0)
	{
		for(uint i = 0; i < BLOCKS_FREE; i++)
		{
			if(

	void* mem_p = malloc(size);

	if(!mem_p)
	{
		// Фатальная ошибка, не удалось выделить MEMORY_INIT_BLOCK
#ifdef _DEBUG_MEMORY

		// Пишем лог
		m_log(S_LOG, "Failed to allocate memory, size: %d", size);

#endif // _DEBUG_MEMORY

		return NULL;
	}

	mem_handle* new_block = (mem_handle*)malloc(sizeof(mem_handle));
	ZeroMemory(new_block, sizeof(mem_handle));

	new_block->memory = mem_p;
	new_block->tag = tag;
	new_block->size = size;
	new_block->next = NULL;
	new_block->prev = NULL;
	new_block->status = MEM_USED_BLOCK;
	new_block->watcher = watcher;

	if(!PTRUNC)
	{
		// Если это первый блок
		MEM_TRUNC = P_MEM_TRUNC = new_block;
	}
	else
	{
		// Добавляем блок в TRUNC
		P_MEM_TRUNC->next = new_block;
		new_block->prev = P_MEM_TRUNC;
		P_MEM_TRUNC = P_MEM_TRUNC->next;
	}

	BLOCKS_USED++;
	BLOCKS_ALLOCATED++;

	MEMORY_USED += size;
	MEMORY_ALLOCATED += size;
	

#ifdef _DEBUG_MEMORY

	// Пишем лог
	m_log(S_LOG, "Allocated new block at %X, size: %d, reason: %s, watcher: %X",
		mem_p,
		size,
#ifdef MEMORY_USE_DESC
		why,
#endif
		watcher);

#endif // _DEBUG_MEMORY

	// Возвращаем результат
	return new_block;
}


// Когда нужно освободить блок, он сначала помечается как свободный
// И при новом выделении памяти сначала будет произведен поиск в свободных кусках
// Когда процент свободной памяти достигнет VACUUM_LEVEL, будет произведен
// MEM_VACUUM(), которая физически освободит все свободные блоки


void mem_Free(
	mem_handle* block,	// Блок памяти, который нужно пометить свободным
	const char* why		// Причина
	)
{

	if(!block)
		return;

	// Помечаем блок как свободный

	block->status = MEM_FREE_BLOCK;

	BLOCKS_USED--;
	MEMORY_USED -= block->size;


	if(BLOCKS_FREE == 0)
	{
		// Если это первый освобожденный блок
		MEM_FREE_TRUNC = P_MEM_FREE_TRUNC = block;


	BLOCKS_FREE++;
	MEMORY_FREE += block->size;

#ifdef _DEBUG_MEMORY

	// Пишем лог
	m_log(S_LOG, "Marked block as free at %X, size: %d, reason: %s, watcher: %X", mem_p, size, why, watcher);

#endif // _DEBUG_MEMORY

	return;

}











bool mem_Dealloc(
	mem_handle* block,	// Блок памяти, который нужно освободить
	const char* why		// Причина освобождения
	)
{

	if(!block)
		return;

	assert(TRUNC);

	
	// Сначала убираем этот блок из TRUNC'а

	mem_handle* tmp = TRUNC;

	for(;;)
	{
		
		if(tmp == block)
		{
			
			if(tmp->status != MEM_FREE_BLOCK)
			{
				// Попытка деаллоцировать блок, который используется

				// Пишем лог
				m_log(S_LOG,
					"Tried to deallocate non-free block at %X, size: %d, reason: %s, watcher: %X",
					block->memory,
					block->size,
					why,
					block->watcher);

				return false;

			}

			// Нашли блок который нужно освободить
			// Исключаем его из последовательности
			if(tmp->prev)
				tmp->prev->next = tmp->next;

			if(tmp->next)
				tmp->next->prev = tmp->prev;

			// Если PTRUNC указывает на этот блок, перемещаем его на
			// валидное значение
			if(MEM_TRUNC == tmp)
				MEM_TRUNC = tmp->next;
			if(P_MEM_TRUNC == tmp)
				if(tmp->next)
					P_MEM_TRUNC = tmp->next;
				else
					P_MEM_TRUNC = tmp->prev;


			// Теперь блок можно освободить
			BLOCKS_ALLOCATED--;
			MEMORY_ALLOCATED -= tmp->size;
			BLOCKS_FREE--;
			MEMORY_FREE -= tmp->size;

			// Сначала освобождаем память на которую он указывает
			if(tmp->memory)
				free(tmp->memory);

			// Теперь саму структуру блока
			free(tmp);

#ifdef _DEBUG_MEMORY

			// Пишем лог
			m_log(S_LOG,
				"Allocated new block at %X, size: %d, reason: %s, watcher: %X",
				mem_p,
				size,
				why,
				watcher);

#endif // _DEBUG_MEMORY

			// Выходим из цикла
			//break;
			return true;

		}

		if(tmp->next)
			tmp = tmp->next;
	}

	// Блок не найден

#ifdef _DEBUG_MEMORY

	// Пишем лог
	m_log(S_LOG,
		"Failed to free block at %X, size: %d, reason: %s, watcher: %X",
		block->memory,
		block->size,
		why,
		block->watcher);

#endif // _DEBUG_MEMORY

	return false;
}







/* Вся память игры (компонента iichan) представляет собой один протяженный кусок
 * памяти MEMORY_POINTER. Выделение памяти происходит расширением этого куска
 * либо выделением блока из выделенной памяти, если ее хватает.
 * Выделение блока логируется, все блоки складываются в список TRUNC.
 * Такое распределение памяти позволяет избежать утечек памяти и контролировать
 * аллокацию, а еще облегчение в отладке.
 *
 *
 *
 *
 *
 *
 *
 */

// TODO: const char* why заменить на uint, создать список соответствующих констант






//#include "stdafx.h"
//
//#include "iichan_memory.h"
//
//// Для логирования
//#include "iichan_interface.h"
//
//mem_handle* TRUNC	=	NULL;	// Транк выделенных блоков
//mem_handle* PTRUNC		=	NULL;	// текущее положение в TRUNC'е
//
//mem_handle* FREETRUNC	=	NULL;	// Транк свободных блоков
//mem_handle* PFREETRUNC		=	NULL;	// Текущее положение в транке
//
//uint		BLOCKS_USED					=	0;		// Количество используемых блоков (mem_handle'ов)
//uint		BLOCKS_FREE					=	0;		// Количество свободных блоков (mem_handle'ов)
//
//// Процентное соотношение свободной памяти
//// (MEMORY_USED / (MEMORY_ALLOCATED / 100))
//uint		MEMORY_FREE_RATIO			=	0;			
//
//void*		MEMORY_POINTER				=	NULL;
//uint		MEMORY_ALLOCATED			=	0;
//uint		MEMORY_USED					=	0;
//
//void*		MEMORY_LAST_POINTER			=	NULL;
//uint		MEMORY_LAST_POINTER_SIZE	=	0;
//
//
//#define		MEMORY_INIT_BLOCK		20480	// 20 Кб
//#define		MEMORY_BIG_BLOCK		10240	// 10 Кб
//
//// Количество свободных блоков, по достижении которого произойдет полная реаллокация
////#define		FREE_BLOCKS_LIMIT		50		// не используется
//
//// Процентное соотношение свободной памяти к выделенной
//// При достижении этого уровня происходит переаллокация (уплотнение) памяти
//#define		VACUUM_LEVEL			40
//
//
//
//
//// Сжатие памяти, перемещение всей свободной памяти в конец
//// Параметр ptrunc - указатель на последний блок в TRUNC'е
//
//void mem_vacuum(mem_handle* trunc_end)
//{
//	if(!trunc_end)
//		return;
//
//
//	mem_handle* ptrunc = trunc_end;
//	mem_handle* tmp	= ptrunc;
//
//	for(;;)
//	{
//
//		uint free_block_length = 0;
//
//		ptrunc = ptrunc->prev;
//
//		if(ptrunc->status == MEM_FREE_BLOCK)
//		{
//
//
//		}
//	}
//}
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
///* Выделение блока памяти */
//
//mem_handle* mem_NewAlloc(
//				  uint size,		// Размер требуемой памяти в байтах
//				  uint tag,			// Пока хз, что то вроде метки блока, для игровой логики
//				  const char* why	// Причина, по которой этот блок выделяется
//				  )
//{
//
//		// Проверка параметров на валидность
//		if(size == 0)
//			return NULL;
//
//		// 'why' может быть равен NULL впрочем
//		
//		// ------------------------------------------------------------------
//		// Получаем память
//
//		// Основная память выделена?
//		if(!MEMORY_ALLOCATED)
//		{
//			// Выделяем начальную память
//			MEMORY_POINTER = malloc(MEMORY_INIT_BLOCK);
//
//			if(!MEMORY_POINTER)
//			{
//				// Фатальная ошибка, не удалось выделить MEMORY_INIT_BLOCK
//#ifdef MEMORY_LOG
//
//				// Пишем лог
//				m_log(S_LOG, "Failed to allocate base memory, size: %d", MEMORY_INIT_BLOCK);
//
//#endif // MEMORY_LOG
//
//				return NULL;
//			}
//
//			MEMORY_ALLOCATED = MEMORY_INIT_BLOCK;
//			MEMORY_USED	= 0;
//			MEMORY_FREE = 0;
//
//			MEMORY_LAST_POINTER = NULL;
//			MEMORY_LAST_POINTER_SIZE = 0;
//
//			TRUNC = PTRUNC = 0;
//
//			BLOCKS_USED = 0;
//
//			FREETRUNC = PFREETRUNC = NULL;
//		}
//
//		// Смотрим, хватает ли ее для выделения блока
//		uint free_mem = MEMORY_ALLOCATED - MEMORY_USED;
//
//		if(free_mem < size)
//		{
//			// Не хватает памяти для выделения блока, нужно выделить еще основной памяти
//			// Желательно чтобы это случалось как можно реже, т.к. в данной ситуации
//			// вызывается realloc всей игровой памяти
//
//			// Хватит ли нам стандартного MEMORY_BIG_BLOCK для выполнения реквеста
//			uint required_mem = size - free_mem;
//
//			// Определяем количество блоков которых нужно добавить к основной памяти
//			uint blocks_to_add = (required_mem / MEMORY_BIG_BLOCK) + 1;
//
//			// Итого, приращение
//			uint mem_inc = blocks_to_add * MEMORY_BIG_BLOCK;
//		
//			// Пытаемся расширить память
//			// NOTE: обернуть в try..catch?
//			void* new_memory_pointer = NULL;
//			new_memory_pointer = realloc(MEMORY_POINTER, MEMORY_ALLOCATED + mem_inc);
//
//			if(new_memory_pointer)
//			{
//				// Память расширена успешно
//				MEMORY_POINTER = new_memory_pointer;
//			}
//			else
//			{
//				// Не удалось расширить память
//#ifdef MEMORY_LOG
//
//				// Пишем лог
//				m_log(S_LOG, "Failed to reallocate base memory, size: %d", MEMORY_ALLOCATED + mem_inc);
//
//#endif // MEMORY_LOG
//
//				return NULL;
//			}
//
//			MEMORY_ALLOCATED += mem_inc;
//
//			// Все указатели в TRUNC'е стали инвалидными, нужно их переуказать
//
//			// TODO
//			REINVALIDATE_POINTERS(TRUNC);
//
//
//			// ---
//		}
//
//		// У нас достаточно памяти, можно выделить требуемый блок
//
//		// Получаем последний использованный поинтер
//		void* pm = MEMORY_LAST_POINTER;
//
//		// Устанавливаем указатель в конец этого блока
//		// получаем таким образом указатель на нетронутую свободную память
//		// Этот указатель и будет указателем на память mem_handle'ра
//		pm += MEMORY_LAST_POINTER_SIZE;
//
//		// Теперь память выделена, увеличиваем счетчик использованной памяти
//		MEMORY_USED += size;
//
//		// Обновляем процентное соотношение
//		MEMORY_FREE_RATIO = int(MEMORY_USED / (MEMORY_ALLOCATED / 100));
//
//		// Обновляем последний использованный поинтер
//		MEMORY_LAST_POINTER = pm;
//		MEMORY_LAST_POINTER_SIZE = size;
//
//		// Все готово, создаем mem_handler
//		mem_handle* new_block = (mem_handle*)malloc(sizeof(mem_handle));
//		ZeroMemory(new_block, sizeof(mem_handle));
//
//		new_block->memory = pm;
//		new_block->size = size;
//		new_block->tag = tag;
//		new_block->status = MEM_VALID_BLOCK;
//
//#ifdef MEMORY_USE_DESC
//
//		// Строка why должна оканчиваться \0, то есть реальный размер строки описания:
//		// MEMORY_DESC_SIZE - 1
//		memcpy(new_block->desc, why, MEMORY_DESC_SIZE);
//
//#endif //MEMORY_USE_DESC
//
//		// Следующего объекта пока нет
//		new_block->next = NULL;
//
//		// Предыдущего тоже
//		new_block->prev = NULL;
//
//		// Блок создан, добавляем его в TRUNC, и перемещаемся в конец TRUNC'а
//		if(BLOCKS_USED > 0)
//		{
//			PTRUNC->next = new_block;
//			PTRUNC->next->prev = PTRUNC;
//			PTRUNC = PTRUNC->next;
//		}
//		else
//		{
//			TRUNC = new_block;
//			PTRUNC = TRUNC;
//		}
//
//#ifdef MEMORY_LOG
//
//		// Пишем лог
//		m_log(S_LOG, "Allocated new block at %X, size: %d, reason: %s", pm, size, why);
//
//#endif // MEMORY_LOG
//
//		BLOCKS_USED++;
//
//		// Возвращаем результат
//		return new_block;
//}
//
//
//
///* Освобождение блока памяти */
//// TODO: возвращать что либо? статус освобожденного блока?
//
//void mem_Free(
//			  mem_handle* block,	// Блок памяти, который нужно освободить
//			  const char* why		// Причина освобождения
//			  )
//{
//
//		if(!block)
//			return;
//
//		// Освобожденный блок просто помечается как свободный для использования,
//		// его память не трогается
//
//		block->status = MEM_FREE_BLOCK;
//
//
//		// Добавляем блок в FREETRUNC освобожденных блоков
//		if(BLOCKS_FREE > 0)
//		{
//			PFREETRUNC->next = block;
//			PFREETRUNC = PFREETRUNC->next;
//		}
//		else
//		{
//			FREETRUNC = block;
//			PFREETRUNC = FREETRUNC;
//		}
//
//
//		// Увеличиваем счетчик освобожденных блоков
//		BLOCKS_FREE++;
//
//		// Уменьшаем счетчик используемых блоков
//		BLOCKS_USED--;
//
//		// Уменьшаем счетчик использованной памяти
//		MEMORY_USED -= block->size;
//
//		// Обновляем процентное соотношение
//		MEMORY_FREE_RATIO = int(MEMORY_USED / (MEMORY_ALLOCATED / 100));
//
//		// Проверяем нужно ли уплотнять память
//
//		if(MEMORY_FREE_RATIO >= VACUUM_LEVEL)
//		{
//			// TODO
//			VACUUM_MEM(FREETRUNC);
//
//			// Все указатели в TRUNC'е стали инвалидными, нужно их переуказать
//
//			// TODO
//			REINVALIDATE_POINTERS(TRUNC);
//
//		}
//
//#ifdef MEMORY_LOG
//
//		// Пишем лог
//		m_log(S_LOG, "Freed block at %X, size: %d, reason: %s", block->memory, block->size, why);
//
//#endif // MEMORY_LOG
//
//		return;
//
//}
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
//void mem_Increase(
//				  mem_handle* block,
//				  uint addition,
//				  const char* why
//				  );
//
//
//void mem_Decrease(
//				  mem_handle* block,
//				  uint substraction,
//				  const char* why
//				  );
//
//
//
//
//
//void mem_Clear(
//			   mem_handle* block,
//			   const char* why
//			   );