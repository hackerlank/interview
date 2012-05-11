#include "stdafx.h"
#include "memory.h"
#include "misc.h"

// this shit doesnt work

void SetLength(void* data, int elemSz, int newLen)
{
	void* tmp;
	//int nowCount = sizeof(&data) / elemSz;

	//void* temp = NULL;

	//memcpy(temp, data, nowCount);
	//ShowNumber(memledata);

	if(data != NULL)
	{
		tmp = realloc(data, newLen * elemSz * 6);
		memcpy(data, tmp, sizeof(tmp));
		ShowNumber(sizeof(tmp));
	}
	else
	{
		data = calloc(1, elemSz);
		//ShowNumber(elemSz);
		//ShowNumber(555);
	}
}