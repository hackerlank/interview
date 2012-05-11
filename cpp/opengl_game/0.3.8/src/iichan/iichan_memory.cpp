//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Engine
//
// DESCRIPTION:
// ��������� ������, �������� ������
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "iichan_memory.h"

mem_handle* MEM_TRUNC			=	NULL;	// ����� ���������� ������
mem_handle* P_MEM_TRUNC			=	NULL;	// ������� ��������� � TRUNC'�


mem_handle* MEM_FREE_TRUNC		=	NULL; // ����� ��������� ������
mem_handle* P_MEM_FREE_TRUNC	=	NULL; // ������� ��������� � ������


uint	MEMORY_USED			=	0;
uint	MEMORY_FREE			=	0;
uint	MEMORY_ALLOCATED	=	0;

uint	BLOCKS_USED			=	0;
uint	BLOCKS_FREE			=	0;
uint	BLOCKS_ALLOCATED	=	0;






mem_handle* mem_NewAlloc(
	  uint size,		// ������ ��������� ������ � ������
	  uint tag,		// ���� ��, ��� �� ����� ����� �����, ��� ������� ������
#ifdef MEMORY_USE_DESC
	  const char* why,	// �������, �� ������� ���� ���� ����������
#endif
	  const void** watcher	// �����������, ���� watcher �� NULL, �� ���� ���� �����
				// ����������, ���� watcher �����
	  )
{

		
	// �������� ���������� �� ����������
	if(size == 0)
		return NULL;

	// 'why' ����� ���� ����� NULL �������
	
	// ------------------------------------------------------------------
	// �������� ������

	// ������� ������� �� ��������� ������
	mem_handle* free_block = NULL;

	if(BLOCKS_FREE > 0)
	{
		for(uint i = 0; i < BLOCKS_FREE; i++)
		{
			if(

	void* mem_p = malloc(size);

	if(!mem_p)
	{
		// ��������� ������, �� ������� �������� MEMORY_INIT_BLOCK
#ifdef _DEBUG_MEMORY

		// ����� ���
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
		// ���� ��� ������ ����
		MEM_TRUNC = P_MEM_TRUNC = new_block;
	}
	else
	{
		// ��������� ���� � TRUNC
		P_MEM_TRUNC->next = new_block;
		new_block->prev = P_MEM_TRUNC;
		P_MEM_TRUNC = P_MEM_TRUNC->next;
	}

	BLOCKS_USED++;
	BLOCKS_ALLOCATED++;

	MEMORY_USED += size;
	MEMORY_ALLOCATED += size;
	

#ifdef _DEBUG_MEMORY

	// ����� ���
	m_log(S_LOG, "Allocated new block at %X, size: %d, reason: %s, watcher: %X",
		mem_p,
		size,
#ifdef MEMORY_USE_DESC
		why,
#endif
		watcher);

#endif // _DEBUG_MEMORY

	// ���������� ���������
	return new_block;
}


// ����� ����� ���������� ����, �� ������� ���������� ��� ���������
// � ��� ����� ��������� ������ ������� ����� ���������� ����� � ��������� ������
// ����� ������� ��������� ������ ��������� VACUUM_LEVEL, ����� ����������
// MEM_VACUUM(), ������� ��������� ��������� ��� ��������� �����


void mem_Free(
	mem_handle* block,	// ���� ������, ������� ����� �������� ���������
	const char* why		// �������
	)
{

	if(!block)
		return;

	// �������� ���� ��� ���������

	block->status = MEM_FREE_BLOCK;

	BLOCKS_USED--;
	MEMORY_USED -= block->size;


	if(BLOCKS_FREE == 0)
	{
		// ���� ��� ������ ������������� ����
		MEM_FREE_TRUNC = P_MEM_FREE_TRUNC = block;


	BLOCKS_FREE++;
	MEMORY_FREE += block->size;

#ifdef _DEBUG_MEMORY

	// ����� ���
	m_log(S_LOG, "Marked block as free at %X, size: %d, reason: %s, watcher: %X", mem_p, size, why, watcher);

#endif // _DEBUG_MEMORY

	return;

}











bool mem_Dealloc(
	mem_handle* block,	// ���� ������, ������� ����� ����������
	const char* why		// ������� ������������
	)
{

	if(!block)
		return;

	assert(TRUNC);

	
	// ������� ������� ���� ���� �� TRUNC'�

	mem_handle* tmp = TRUNC;

	for(;;)
	{
		
		if(tmp == block)
		{
			
			if(tmp->status != MEM_FREE_BLOCK)
			{
				// ������� �������������� ����, ������� ������������

				// ����� ���
				m_log(S_LOG,
					"Tried to deallocate non-free block at %X, size: %d, reason: %s, watcher: %X",
					block->memory,
					block->size,
					why,
					block->watcher);

				return false;

			}

			// ����� ���� ������� ����� ����������
			// ��������� ��� �� ������������������
			if(tmp->prev)
				tmp->prev->next = tmp->next;

			if(tmp->next)
				tmp->next->prev = tmp->prev;

			// ���� PTRUNC ��������� �� ���� ����, ���������� ��� ��
			// �������� ��������
			if(MEM_TRUNC == tmp)
				MEM_TRUNC = tmp->next;
			if(P_MEM_TRUNC == tmp)
				if(tmp->next)
					P_MEM_TRUNC = tmp->next;
				else
					P_MEM_TRUNC = tmp->prev;


			// ������ ���� ����� ����������
			BLOCKS_ALLOCATED--;
			MEMORY_ALLOCATED -= tmp->size;
			BLOCKS_FREE--;
			MEMORY_FREE -= tmp->size;

			// ������� ����������� ������ �� ������� �� ���������
			if(tmp->memory)
				free(tmp->memory);

			// ������ ���� ��������� �����
			free(tmp);

#ifdef _DEBUG_MEMORY

			// ����� ���
			m_log(S_LOG,
				"Allocated new block at %X, size: %d, reason: %s, watcher: %X",
				mem_p,
				size,
				why,
				watcher);

#endif // _DEBUG_MEMORY

			// ������� �� �����
			//break;
			return true;

		}

		if(tmp->next)
			tmp = tmp->next;
	}

	// ���� �� ������

#ifdef _DEBUG_MEMORY

	// ����� ���
	m_log(S_LOG,
		"Failed to free block at %X, size: %d, reason: %s, watcher: %X",
		block->memory,
		block->size,
		why,
		block->watcher);

#endif // _DEBUG_MEMORY

	return false;
}







/* ��� ������ ���� (���������� iichan) ������������ ����� ���� ����������� �����
 * ������ MEMORY_POINTER. ��������� ������ ���������� ����������� ����� �����
 * ���� ���������� ����� �� ���������� ������, ���� �� �������.
 * ��������� ����� ����������, ��� ����� ������������ � ������ TRUNC.
 * ����� ������������� ������ ��������� �������� ������ ������ � ��������������
 * ���������, � ��� ���������� � �������.
 *
 *
 *
 *
 *
 *
 *
 */

// TODO: const char* why �������� �� uint, ������� ������ ��������������� ��������






//#include "stdafx.h"
//
//#include "iichan_memory.h"
//
//// ��� �����������
//#include "iichan_interface.h"
//
//mem_handle* TRUNC	=	NULL;	// ����� ���������� ������
//mem_handle* PTRUNC		=	NULL;	// ������� ��������� � TRUNC'�
//
//mem_handle* FREETRUNC	=	NULL;	// ����� ��������� ������
//mem_handle* PFREETRUNC		=	NULL;	// ������� ��������� � ������
//
//uint		BLOCKS_USED					=	0;		// ���������� ������������ ������ (mem_handle'��)
//uint		BLOCKS_FREE					=	0;		// ���������� ��������� ������ (mem_handle'��)
//
//// ���������� ����������� ��������� ������
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
//#define		MEMORY_INIT_BLOCK		20480	// 20 ��
//#define		MEMORY_BIG_BLOCK		10240	// 10 ��
//
//// ���������� ��������� ������, �� ���������� �������� ���������� ������ �����������
////#define		FREE_BLOCKS_LIMIT		50		// �� ������������
//
//// ���������� ����������� ��������� ������ � ����������
//// ��� ���������� ����� ������ ���������� ������������� (����������) ������
//#define		VACUUM_LEVEL			40
//
//
//
//
//// ������ ������, ����������� ���� ��������� ������ � �����
//// �������� ptrunc - ��������� �� ��������� ���� � TRUNC'�
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
///* ��������� ����� ������ */
//
//mem_handle* mem_NewAlloc(
//				  uint size,		// ������ ��������� ������ � ������
//				  uint tag,			// ���� ��, ��� �� ����� ����� �����, ��� ������� ������
//				  const char* why	// �������, �� ������� ���� ���� ����������
//				  )
//{
//
//		// �������� ���������� �� ����������
//		if(size == 0)
//			return NULL;
//
//		// 'why' ����� ���� ����� NULL �������
//		
//		// ------------------------------------------------------------------
//		// �������� ������
//
//		// �������� ������ ��������?
//		if(!MEMORY_ALLOCATED)
//		{
//			// �������� ��������� ������
//			MEMORY_POINTER = malloc(MEMORY_INIT_BLOCK);
//
//			if(!MEMORY_POINTER)
//			{
//				// ��������� ������, �� ������� �������� MEMORY_INIT_BLOCK
//#ifdef MEMORY_LOG
//
//				// ����� ���
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
//		// �������, ������� �� �� ��� ��������� �����
//		uint free_mem = MEMORY_ALLOCATED - MEMORY_USED;
//
//		if(free_mem < size)
//		{
//			// �� ������� ������ ��� ��������� �����, ����� �������� ��� �������� ������
//			// ���������� ����� ��� ��������� ��� ����� ����, �.�. � ������ ��������
//			// ���������� realloc ���� ������� ������
//
//			// ������ �� ��� ������������ MEMORY_BIG_BLOCK ��� ���������� ��������
//			uint required_mem = size - free_mem;
//
//			// ���������� ���������� ������ ������� ����� �������� � �������� ������
//			uint blocks_to_add = (required_mem / MEMORY_BIG_BLOCK) + 1;
//
//			// �����, ����������
//			uint mem_inc = blocks_to_add * MEMORY_BIG_BLOCK;
//		
//			// �������� ��������� ������
//			// NOTE: �������� � try..catch?
//			void* new_memory_pointer = NULL;
//			new_memory_pointer = realloc(MEMORY_POINTER, MEMORY_ALLOCATED + mem_inc);
//
//			if(new_memory_pointer)
//			{
//				// ������ ��������� �������
//				MEMORY_POINTER = new_memory_pointer;
//			}
//			else
//			{
//				// �� ������� ��������� ������
//#ifdef MEMORY_LOG
//
//				// ����� ���
//				m_log(S_LOG, "Failed to reallocate base memory, size: %d", MEMORY_ALLOCATED + mem_inc);
//
//#endif // MEMORY_LOG
//
//				return NULL;
//			}
//
//			MEMORY_ALLOCATED += mem_inc;
//
//			// ��� ��������� � TRUNC'� ����� �����������, ����� �� �����������
//
//			// TODO
//			REINVALIDATE_POINTERS(TRUNC);
//
//
//			// ---
//		}
//
//		// � ��� ���������� ������, ����� �������� ��������� ����
//
//		// �������� ��������� �������������� �������
//		void* pm = MEMORY_LAST_POINTER;
//
//		// ������������� ��������� � ����� ����� �����
//		// �������� ����� ������� ��������� �� ���������� ��������� ������
//		// ���� ��������� � ����� ���������� �� ������ mem_handle'��
//		pm += MEMORY_LAST_POINTER_SIZE;
//
//		// ������ ������ ��������, ����������� ������� �������������� ������
//		MEMORY_USED += size;
//
//		// ��������� ���������� �����������
//		MEMORY_FREE_RATIO = int(MEMORY_USED / (MEMORY_ALLOCATED / 100));
//
//		// ��������� ��������� �������������� �������
//		MEMORY_LAST_POINTER = pm;
//		MEMORY_LAST_POINTER_SIZE = size;
//
//		// ��� ������, ������� mem_handler
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
//		// ������ why ������ ������������ \0, �� ���� �������� ������ ������ ��������:
//		// MEMORY_DESC_SIZE - 1
//		memcpy(new_block->desc, why, MEMORY_DESC_SIZE);
//
//#endif //MEMORY_USE_DESC
//
//		// ���������� ������� ���� ���
//		new_block->next = NULL;
//
//		// ����������� ����
//		new_block->prev = NULL;
//
//		// ���� ������, ��������� ��� � TRUNC, � ������������ � ����� TRUNC'�
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
//		// ����� ���
//		m_log(S_LOG, "Allocated new block at %X, size: %d, reason: %s", pm, size, why);
//
//#endif // MEMORY_LOG
//
//		BLOCKS_USED++;
//
//		// ���������� ���������
//		return new_block;
//}
//
//
//
///* ������������ ����� ������ */
//// TODO: ���������� ��� ����? ������ �������������� �����?
//
//void mem_Free(
//			  mem_handle* block,	// ���� ������, ������� ����� ����������
//			  const char* why		// ������� ������������
//			  )
//{
//
//		if(!block)
//			return;
//
//		// ������������� ���� ������ ���������� ��� ��������� ��� �������������,
//		// ��� ������ �� ���������
//
//		block->status = MEM_FREE_BLOCK;
//
//
//		// ��������� ���� � FREETRUNC ������������� ������
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
//		// ����������� ������� ������������� ������
//		BLOCKS_FREE++;
//
//		// ��������� ������� ������������ ������
//		BLOCKS_USED--;
//
//		// ��������� ������� �������������� ������
//		MEMORY_USED -= block->size;
//
//		// ��������� ���������� �����������
//		MEMORY_FREE_RATIO = int(MEMORY_USED / (MEMORY_ALLOCATED / 100));
//
//		// ��������� ����� �� ��������� ������
//
//		if(MEMORY_FREE_RATIO >= VACUUM_LEVEL)
//		{
//			// TODO
//			VACUUM_MEM(FREETRUNC);
//
//			// ��� ��������� � TRUNC'� ����� �����������, ����� �� �����������
//
//			// TODO
//			REINVALIDATE_POINTERS(TRUNC);
//
//		}
//
//#ifdef MEMORY_LOG
//
//		// ����� ���
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