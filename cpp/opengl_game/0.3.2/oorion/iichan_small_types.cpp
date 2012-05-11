#include "stdafx.h"




#include "iichan_small_types.h"


void PHRASE::Draw()
{
		__super::Draw();

		PrintText(GetText().c_str(), FontByName("default"), GetX()+10, GetY()+10);
}

POWERUP::tagPOWERUP(enum POWERUP_TYPE type, int count)
{
	SetType(type);
	SetCount(count);
	SetDead(false);
}