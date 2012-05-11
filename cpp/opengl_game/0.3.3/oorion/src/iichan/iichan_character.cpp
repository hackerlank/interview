#include "stdafx.h"

#include "iichan_character.h"
#include "iichan_small_types.h"

#include "../config.h"

extern config cfg;


void CHARACTER::Shoot()
{
	SetAmmo(GetAmmo()-1);
}

CHARACTER::CHARACTER()
{
	__super::ITEM();
	SetAmmo(0);
	SetShoots(0);
	SetShooting(false);
	SetWeight(0);
}