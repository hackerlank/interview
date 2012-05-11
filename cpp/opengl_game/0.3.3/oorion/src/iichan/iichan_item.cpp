#include "stdafx.h"
#include "iichan_item.h"

ITEM::ITEM()
{
	SetType(itemNone);
	SetSprite(NULL);
	SetDead(false);
	SetId(0);
}

void ITEM::Move()
{
}