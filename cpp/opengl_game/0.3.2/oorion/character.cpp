#include "stdafx.h"

#include "character.h"
#include "iichan_small_types.h"

#include "config.h"

extern config cfg;


void CHARACTER::Shoot()
{


	SetAmmo(GetAmmo()-1);
	
}