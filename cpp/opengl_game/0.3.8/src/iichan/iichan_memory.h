//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Engine
//
// DESCRIPTION:
// Выделение памяти, менеджер памяти
//-----------------------------------------------------------------------------



// Некоторые опции
//-----------------------------------------------------------------------------

#define MEMORY_USE_DESC		// Использовать описания блоков (строка desc)
#define _DEBUG_MEMORY		// Логгировать операции с памятью
										
//-----------------------------------------------------------------------------




#ifdef	MEMORY_USE_DESC

// Размер строки описания блока
#define MEMORY_DESC_SIZE 8

#endif // MEMORY_USE_DESC


typedef enum {

	MEM_USED_BLOCK		=	1,
	MEM_INVALID_BLOCK	=	2,	// пока не используется
	MEM_FREE_BLOCK		=	3

} mem_status;


typedef struct {

	void*				memory;

	uint				size;

	uint				tag;

#ifdef MEMORY_USE_DESC

	char[MEMORY_DESC_SIZE]	desc;

#endif // MEMORY_USE_DESC

	uint				status;

	void**				watcher;


	mem_handle*			prev;
	mem_handle*			next;

} mem_handle;


#ifdef MEMORY_USE_DESC

mem_handle* mem_NewAlloc(
				  uint size,
				  uint tag,
				  const char* why,
				  const void** watcher,
				  );

#else

mem_handle* mem_NewAlloc(
					uint size,
					uint tag,
					const void** watcher
					);

#endif // MEMORY_USE_DESC


void mem_Increase(
				  mem_handle* block,
				  uint addition,
				  const char* why
				  );


void mem_Decrease(
				  mem_handle* block,
				  uint substraction,
				  const char* why
				  );


void mem_Free(
			  mem_handle* block,
			  const char* why
			  );


void mem_Clear(
			   mem_handle* block,
			   const char* why
			   );



// ----------------


void mem_Init(void);


void mem_Cleanup(void);








