#include "stdafx.h"
#include "iichan_sprite.h"
#include "../draw/draw.h"

// remove this include:
#include "../misc.h"
// --

#include "../config.h"
extern config cfg;



extern int WORLD_X;
extern int WORLD_Y;


void SPRITE::Draw()
{
	int width;
	int x1 = GetX() - WORLD_X;
#ifdef OPTIMIZE_DRAW
	SetVisible((GetX() + GetPatternWidth() >= WORLD_X &&
		GetX() <= WORLD_X + cfg.scr_width) && 
		(GetY() >= WORLD_Y && GetY() <= WORLD_Y + cfg.scr_height));

	if(GetVisible() || GetFixed())
	{
		if(GetX() + GetPatternWidth() > WORLD_X + cfg.scr_width)
		{
			width = WORLD_X + cfg.scr_width - GetX();
			if(GetFixed())
				width = GetPatternWidth();
		}
		else if(GetX() < WORLD_X)
		{
			x1 = 0;
			width = GetPatternWidth() - (GetX() + WORLD_X);
			if(GetFixed())
			{
				x1 = GetX() - WORLD_X;
				width = GetPatternWidth();
			}
		}
		else
#endif
		width = GetPatternWidth();
		int y1 = GetY() + WORLD_Y;

		if(!GetFixed())
			DrawTexture(x1, y1, GetPatternX(), GetPatternY(),
				GetPatternX() + width, GetPatternY() + GetPatternHeight(),
				GetTexture(), GetMirror(), false, 100, 100);
		else
			DrawTexture(GetX(), GetY(), GetPatternX(), GetPatternY(),
				width, GetPatternHeight(),
				GetTexture(), GetMirror(), false, 100, 100);

#ifdef OPTIMIZE_DRAW
	}
#endif


	if(cfg.debug)
	{
		glDisable(GL_BLEND);
		glPushAttrib(GL_CURRENT_BIT);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		if(!GetFixed())
			Box(GetX() + GetRealX() - WORLD_X,
			GetY() + GetRealY() + WORLD_Y,
			GetRealWidth(),
			GetRealHeight());
		else
			Box(GetX() + GetRealX(),
			GetY() + GetRealY(),
			GetRealWidth(),
			GetRealHeight());
		glPopAttrib();
		glEnable(GL_BLEND);
	}
}

void SPRITE::Move()
{
}

void SPRITE::LoadFromResource(OBJECT_RES* res)
{
	if(res)
	{
		SetTexture(TextureByName(res->texture));
		SetPatternWidth(res->pattern_width);
		SetPatternHeight(res->pattern_height);
		SetRealX(res->real_x);
		SetRealY(res->real_y);
		SetRealWidth(res->real_width);
		SetRealHeight(res->real_height);
		SetRealMirrorX(res->real_mirror_x);
		SetRealMirrorY(res->real_mirror_y);
		SetImageWidth(res->image_width);
		SetImageHeight(res->image_height);
		SetPatternX(res->pattern_x);
		SetPatternY(res->pattern_y);
		SetZ(res->z);
	}
}



SPRITE::SPRITE()
{
	SetId(0);
	SetPatternWidth(0);
	SetPatternHeight(0);
	SetImageWidth(0);
	SetImageHeight(0);
	SetX(0);
	SetY(0);
	SetRealX(0);
	SetRealY(0);
	SetRealWidth(0);
	SetRealHeight(0);
	SetMirror(false);
	SetFixed(false);
	SetDead(false);
	SetZ(DEFAULT_Z);
#ifdef OPTIMIZE_DRAW
	SetVisible(true);
#endif
	SetPatternX(0);
	SetPatternY(0);
}