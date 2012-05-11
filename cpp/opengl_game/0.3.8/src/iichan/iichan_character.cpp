#include "stdafx.h"

#include "iichan_character.h"
#include "iichan_small_types.h"

#include "../config.h"

extern config cfg;


void CHARACTER::Shoot()
{
	SetAmmo(GetAmmo() - 1);
}

void CHARACTER::Ouch(int damage)
{
	SetHealth(GetHealth() - damage);
	ANIMATEDSPRITE* spr = GetSprite();
	if (spr)
		spr->SetDamaging(true);
	//SetHealth(GetHealth() - 1);
}

void CHARACTER::Move()
{
	__super::Move();

	if(IsDead())
		if(GetShadow())
			GetShadow()->SetDead(true);

	if(GetShadow() && GetSprite())
	{
		if(GetShadow()->GetSprite())
		{
			if(GetSprite()->GetMirror())
			{
				GetShadow()->GetSprite()->SetX(GetSprite()->GetX() + GetSprite()->GetShadowMirrorX());
			}
			else
			{
				GetShadow()->GetSprite()->SetX(GetSprite()->GetX() + GetSprite()->GetShadowX());
			}
		}
	}
}


CHARACTER::CHARACTER()
{
	__super::ITEM();
	SetAmmo(0);
	SetShoots(0);
	SetShadow(NULL);
	SetVulnerable(true);
	SetVulnTicks(0);
	SetReloadSpeed(0);
	SetBulletType(NULL);
	SetShootSound("");
}