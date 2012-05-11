#include "stdafx.h"
#include "iichan_sprite.h"
#include "../draw/draw.h"

#include "../config.h"

extern config cfg;
extern int WORLD_X;
extern int WORLD_Y;

void SPRITE::DrawCollideRect()
{
	RECT rect = GetCollideRect();
	RectBox(rect.left, rect.top, rect.right, rect.bottom);
}

void SPRITE::DrawMirrorCollideRect()
{
	RECT rect = GetMirrorCollideRect();
	RectBox(rect.left, rect.top, rect.right, rect.bottom);
}


void SPRITE::Draw()
{
	if(!GetVisible())
		return;

	int x1 = GetX() - WORLD_X;
	int width = GetPatternWidth();
	int y1 = GetY() + WORLD_Y;

	if(GetBlend() != 0.0)
		glEnable(GL_BLEND);

	if(!IsFixed())
		DrawTexture(x1, y1, GetPatternX(), GetPatternY(),
			GetPatternX() + width, GetPatternY() + GetPatternHeight(),
			GetTexture(), GetMirror(), false, 100, 100);
	else
		DrawTexture(GetX(), GetY(), GetPatternX(), GetPatternY(),
			width, GetPatternHeight(),
			GetTexture(), GetMirror(), false, 100, 100);
	if(GetBlend() != 0.0)
		glDisable(GL_BLEND);

	if(cfg.debug && IsCollidable())
	{
		glPushAttrib(GL_CURRENT_BIT);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		if(GetMirror())
			DrawMirrorCollideRect();
		else
			DrawCollideRect();
		glPopAttrib();
	}
}

void SPRITE::Move()
{
	RECT rect;
	rect.left = GetX() + GetRealX();
	rect.right = GetX() + GetRealX() + GetRealWidth();
	rect.top = GetY() + GetRealY();
	rect.bottom = GetY() + GetRealY() + GetRealHeight();
	if(!IsFixed())
	{
		rect.left -= WORLD_X;
		rect.right -= WORLD_X;
		rect.top += WORLD_Y;
		rect.bottom += WORLD_Y;
	}
	SetCollideRect(rect);

	rect.left = GetX() + GetRealMirrorX();
	rect.right = GetX() + GetRealMirrorX() + GetRealWidth();
	rect.top = GetY() + GetRealMirrorY();
	rect.bottom = GetY() + GetRealMirrorY() + GetRealHeight();
	if(!IsFixed())
	{
		rect.left -= WORLD_X;
		rect.right -= WORLD_X;
		rect.top += WORLD_Y;
		rect.bottom += WORLD_Y;
	}
	SetMirrorCollideRect(rect);
}

void SPRITE::LoadFromResource(RESOURCE* res)
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
		SetShadowX(res->shadow_x);
		SetShadowY(res->shadow_y);
		SetShadowMirrorX(res->shadow_mirror_x);
		SetShadowMirrorY(res->shadow_mirror_y);
		SetZ(res->z);
		SetBlend(res->blend);
		RECT rect;
		rect.left = GetX() + GetRealX();
		rect.top = GetY() + GetRealY();
		rect.right = GetX() + GetRealX() + GetRealWidth();
		rect.bottom = GetY() + GetRealY() + GetRealHeight();
		SetCollideRect(rect);
		RECT mirror_rect;
		mirror_rect.left = GetX() + GetRealMirrorX();
		mirror_rect.top = GetY() + GetRealMirrorY();
		mirror_rect.right = GetX() + GetRealMirrorX() + GetRealWidth();
		mirror_rect.bottom = GetY() + GetRealMirrorY() + GetRealHeight();
		SetMirrorCollideRect(mirror_rect);

		if(strcmp(res->name, "slowpoke") == 0)
		{
			SetSlowpoke(true);
		}

	}
}



SPRITE::SPRITE()
{
	static int __count = 0;

	SetId(__count);
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
	SetPatternX(0);
	SetPatternY(0);
	SetCollidable(true);
	SetShadowed(true);
	SetShadowX(0);
	SetShadowY(0);
	SetShadowMirrorX(0);
	SetShadowMirrorY(0);
	SetBlend(0.0); // no blending by default
	RECT rect;
	RECT mirror_rect;
	memset(&rect, 0, sizeof(rect));
	memset(&mirror_rect, 0, sizeof(mirror_rect));
	SetCollideRect(rect);
	SetMirrorCollideRect(mirror_rect);
	SetVisible(true);
	SetSlowpoke(false);
	__count++;
}