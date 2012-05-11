//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// System
//
// DESCRIPTION:
//	Memory stuff
//	
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "fs_def.h"

#include "i_memory.h"

#include "m_tools.h"


extern int appl_time;


typedef struct {

	void** ptr;
	uint*  length;
	uint   update_period;

	uint   prev_update;

} cm_unit; // controlled-memory unit

vector <cm_unit> units;



void _do_watch(cm_unit* unit)
{

	void* memory = *(unit->ptr);
	uint length = *(unit->length);

	memory = realloc(memory, length);

	*(unit->ptr) = memory;

#ifdef DEBUG_LOGGING
	M_Echo(S_LOG, "Updating watched memory: ptr=%X, length=%d",
		unit->ptr, unit->length);
#endif
}


void I_WatchMemory(void** ptr, uint* length, uint update_period)
{

	assert(ptr != NULL);
	assert(length != NULL);
	assert(update_period != NULL);
	assert(*ptr != NULL);
	assert(*length != 0);
	assert(update_period != 0);

	cm_unit new_unit;
	ZeroMemory(&new_unit, sizeof(cm_unit));

	new_unit.ptr = ptr;
	new_unit.length = length;
	new_unit.update_period = update_period;
	new_unit.prev_update = appl_time;

	units.push_back(new_unit);

#ifdef DEBUG_LOGGING
	M_Echo(S_LOG, "Watching memory: ptr=%X, length=%d, update_period=%d", 
			*ptr, *length, update_period);
#endif
}


bool I_UnwatchMemory(void** ptr)
{
	assert(ptr != NULL);

	uint counter = 0;

	uint size = units.size();

	for(uint i = 0; i < size; i++)
	{
		if(units[i].ptr == ptr)
		{
			// found this unit, remove it
			units.erase(units.begin() + counter);

#ifdef DEBUG_LOGGING
			M_Echo(S_LOG, "Unwatching memory: ptr=%X", *ptr);
#endif

			return true; // success

		}

		counter++;
	}

	// unit not found, wasn't watched thou
#ifdef DEBUG_LOGGING
	M_Echo(S_LOG, "Trying to unwatch memory at %X, which isn't watched", *ptr);
#endif
	return false;

}



void I_UpdateMemory()
{
	for(vector<cm_unit>::iterator it = units.begin();
		it != units.end();
		it++)
	{
		cm_unit* m = &(*it);

		if(appl_time - m->prev_update >= m->update_period)
		{
			_do_watch(m);
			m->prev_update = appl_time;
		}
	}
}