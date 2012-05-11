#include "stdafx.h"
#include "iichan_animatedsprite.h"
#include "../misc.h"
#include "../draw/draw.h"


#include "../config.h"
extern config cfg;


extern int WORLD_X;
extern int WORLD_Y;

#define PATTERN_NOT_SET 65535

void ANIMATEDSPRITE::SetCurrentAnimation(enum ANIMATION_TYPE type, bool looped)
{
	if(GetCurrentAnimation() != type)
	{
		switch(type)
		{
		case animShoot:
			SetStartPattern(GetShootPattern());
			SetPatternCount(GetShootCount());
			SetAnimationSpeed(GetShootSpeed());
			break;
		//case animShoot2:
		//	SetStartPattern(GetShoot2Pattern());
		//	SetPatternCount(GetShoot2Count());
		//	break;
		//case animShoot3:
		//	SetStartPattern(GetShoot3Pattern());
		//	SetPatternCount(GetShoot3Count());
		//	break;
		//case animTalk:
		//	SetStartPattern(GetTalkPattern());
		//	SetPatternCount(GetTalkCount());
		//	break;
		case animIdle:
			SetStartPattern(GetIdlePattern());
			SetPatternCount(GetIdleCount());
			SetAnimationSpeed(GetIdleSpeed());
			break;
		case animWalk:
			SetStartPattern(GetWalkPattern());
			SetPatternCount(GetWalkCount());
			SetAnimationSpeed(GetWalkSpeed());
			break;
		case animJump:
			SetStartPattern(GetJumpPattern());
			SetPatternCount(GetJumpCount());
			SetAnimationSpeed(GetJumpSpeed());
			break;
		case animStand:
			SetStartPattern(GetStandPattern());
			SetPatternCount(GetStandCount());
			SetAnimationSpeed(GetStandSpeed());
			break;
		//case animSit:
		//	SetStartPattern(GetSitPattern());
		//	SetPatternCount(GetSitCount());
		//	break;
		case animLand:
			SetStartPattern(GetLandPattern());
			SetPatternCount(GetLandCount());
			SetAnimationSpeed(GetLandSpeed());
			break;
		default:
			break;
		}

		SetCurrentPattern(GetStartPattern());
		currentAnimation = type;
		SetLooped(looped);
		SetDoneAnimation(false);
	}
}

void ANIMATEDSPRITE::Draw()
{
#ifdef OPTIMIZE_DRAW
	SetVisible((GetX() + GetPatternWidth() >= WORLD_X &&
		GetX() <= WORLD_X + cfg.scr_width) && 
		(GetY() >= WORLD_Y && GetY() <= WORLD_Y + cfg.scr_height));
	if(GetVisible())
	{
#endif
		ANIMATEDSPRITE* s = this;
		int colCount = 1;
		if(GetPatternWidth() != 0)
		colCount = GetImageWidth() / GetPatternWidth();

		int x1;
		int x2;
		int y1;
		int y2;


		if(GetCurrentPattern() == PATTERN_NOT_SET)
		{
			x1 = GetPatternX();
			x2 = x1 + GetPatternWidth();
			y1 = GetPatternY();
			y2 = y1 + GetPatternHeight();
		}
		else
		{
			int col = GetCurrentPattern() % colCount;
			int row = GetCurrentPattern() / colCount;
			
			x1 = col * GetPatternWidth();
			x2 = x1 + GetPatternWidth();
			y1 = row * GetPatternHeight();
			y2 = y1 + GetPatternHeight();

			;
		}

		if(!GetFixed())
			DrawTexture(GetX() - WORLD_X, GetY() + WORLD_Y, x1, y1, x2, y2, 
				GetTexture(), GetMirror(), false, 100, 100);
		else
			DrawTexture(GetX(), GetY(), x1, y1, x2, y2,
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

void ANIMATEDSPRITE::Move()
{
	if(GetPatternCount() > 0)
	{
		int now = GetTickCount();
		if(now - GetPrevAnimTick() >= GetAnimationSpeed())
		{
			if(!GetReverse())
			{
				if((GetCurrentPattern() == GetRealPatternCount() - 1 ))
				{
					SetCurrentPattern(GetStartPattern());
				}
				else
				{
					if(GetCurrentPattern() == GetPatternCount() + GetStartPattern() - 1)
					{
						if(!GetLooped())
						{
							SetPatternCount(0);
							SetDoneAnimation(true);
						}
						else
						{
							//if(GetCurrentAnimation() == animWalk)
							//{
								SetCurrentPattern(GetStartPattern());
								SetDoneAnimation(false);
							//}
						}
					}
					else
					{
						if(GetCurrentAnimation() != animJump)
							SetCurrentPattern(GetCurrentPattern() + 1);
						else
						{
							if(!GetJumpDirection())
							{
								if(GetCurrentPattern() < GetJumpDownPattern())
									SetCurrentPattern(GetCurrentPattern() + 1);
							}
						}
					}
				}
			}
			else
			{
				if(GetStartPattern() - GetCurrentPattern() == GetPatternCount()-1)
				{
					SetCurrentPattern(GetStartPattern());
					if(!GetLooped())
					{
						SetPatternCount(0);
						SetDoneAnimation(true);
					}
					else
						SetDoneAnimation(false);
				}
				else
				{
					if(GetCurrentPattern() == 0)
						SetCurrentPattern(GetStartPattern());
					else
						SetCurrentPattern(GetCurrentPattern() - 1);
				}
			}
			SetPrevAnimTick(now);
		}
	}

}

void ANIMATEDSPRITE::LoadFromResource(OBJECT_RES* res)
{
	if(res)
	{
		__super::LoadFromResource(res);
		SetPatternWidth(res->pattern_width);
		SetPatternHeight(res->pattern_height);
		SetRealPatternCount(res->real_pattern_count);
		SetShootPattern(res->shoot_pattern);
		SetShootCount(res->shoot_count);
		SetShootSpeed(res->shoot_speed);
		SetWalkPattern(res->walk_pattern);
		SetWalkCount(res->walk_count);
		SetWalkSpeed(res->walk_speed);
		SetJumpPattern(res->jump_pattern);
		SetJumpCount(res->jump_count);
		SetJumpSpeed(res->jump_speed);
		SetIdlePattern(res->idle_pattern);
		SetIdleCount(res->idle_count);
		SetIdleSpeed(res->idle_speed);
		SetLandPattern(res->land_pattern);
		SetLandCount(res->land_count);
		SetLandSpeed(res->land_speed);
		SetStandPattern(res->stand_pattern);
		SetStandCount(res->stand_count);
		SetStandSpeed(res->stand_speed);
		SetJumpDownPattern(res->jump_down_pattern);
		SetPatternCount(res->pattern_count);
		SetAnimationSpeed(res->animation_speed);
		SetStartPattern(res->start_pattern);
		SetCurrentPattern(res->current_pattern);
	}
}


ANIMATEDSPRITE::ANIMATEDSPRITE()
{
	SetMirror(false);
	SetStartPattern(0);
	SetCurrentPattern(0);
	SetPatternWidth(0);
	SetPatternHeight(0);
	SetPatternCount(0);
	SetRealPatternCount(0);
	SetTexture(NULL);
	SetImageWidth(0);
	SetImageHeight(0);

	SetIdleSpeed(MOVE_DELAY);
	SetStandSpeed(MOVE_DELAY);
	SetWalkSpeed(MOVE_DELAY);
	SetJumpSpeed(MOVE_DELAY);
	SetLandSpeed(MOVE_DELAY);
	SetShootSpeed(MOVE_DELAY);

	SetIdlePattern(0);
	SetWalkPattern(0);
	//SetTalkPattern(0);
	SetJumpPattern(0);
	SetShootPattern(0);
	//SetShoot2Pattern(0);
	//SetShoot3Pattern(0);
	SetStandPattern(0);
	//SetSitPattern(0);
	SetIdleCount(0);
	SetWalkCount(0);
	SetStandCount(0);
	//SetTalkCount(0);
	SetJumpCount(0);
	SetLandPattern(0);
	SetLandCount(0);
	SetShootCount(0);
	//SetShoot2Count(0);
	//SetShoot3Count(0);
	//SetSitCount(0);
	SetJumpDownPattern(0);
	SetX(0);
	SetY(0);
	SetCurrentAnimation(animNone, true);
	SetReverse(false);
	SetMoving(false);
	SetMovingDirection(false);
	SetLanding(false);
	SetYStart(0);
	SetDoneAnimation(false);
	SetAnimationSpeed(MOVE_DELAY);
	SetPrevAnimTick(GetTickCount());
	SetJumping(false);
	SetJumpDirection(false);
	SetYSpeed(0);
}