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


void I_WatchMemory(void** ptr, uint* length, uint* update_period);
bool I_UnwatchMemory(void** ptr);



void I_UpdateMemory(void);