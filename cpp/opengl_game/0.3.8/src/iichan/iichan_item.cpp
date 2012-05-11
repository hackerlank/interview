#include "stdafx.h"
#include "iichan_item.h"

ITEM::ITEM()
{
	SetType(itemNone);
	SetSprite(NULL);
	SetDead(false);
	SetId(0);
	SetPhysBody(NULL);
	//is_new = true;
}

void ITEM::Move()
{
	if(IsDead())
		if(GetSprite())
			GetSprite()->SetDead(true);

}