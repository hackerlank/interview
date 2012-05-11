#include "stdafx.h"
#include "sprite.h"

extern int WORLD_X;
extern int WORLD_Y;

void SPRITE::Draw()
{
	if(!GetFixed())
		DrawTexture(GetX() - WORLD_X, GetY() + WORLD_Y, 0, 0,
			GetPatternWidth(), GetPatternHeight(),
			GetTexture(), GetMirror(), false, 100, 100);
	else
		DrawTexture(GetX(), GetY(), 0, 0,
			GetPatternWidth(), GetPatternHeight(),
			GetTexture(), GetMirror(), false, 100, 100);
}

SPRITE::tagSPRITE()
{
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
}