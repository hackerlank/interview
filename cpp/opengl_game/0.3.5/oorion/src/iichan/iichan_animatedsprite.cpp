#include "stdafx.h"
#include "iichan_animatedsprite.h"
#include "../misc.h"
#include "../draw/draw.h"
#include "../config.h"

extern config cfg;
extern int WORLD_X;
extern int WORLD_Y;
extern unsigned int current_time;

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
		case animSit:
			SetStartPattern(GetSitPattern());
			SetPatternCount(GetSitCount());
			break;
		case animSitShoot:
			SetStartPattern(GetSitShootPattern());
			SetPatternCount(GetSitShootCount());
			break;
		case animLand:
			SetStartPattern(GetLandPattern());
			SetPatternCount(GetLandCount());
			SetAnimationSpeed(GetLandSpeed());
			break;
		case animDamage:
			SetStartPattern(GetDamagePattern());
			SetPatternCount(GetDamageCount());
			SetAnimationSpeed(GetDamageSpeed());
			break;
		case animDie:
			SetStartPattern(GetDiePattern());
			SetPatternCount(GetDieCount());
			SetAnimationSpeed(GetDieSpeed());
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
	if(!GetVisible())
		return;

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
	}

	if(!IsFixed())
		DrawTexture(GetX() - WORLD_X, GetY() + WORLD_Y, x1, y1, x2, y2, 
			GetTexture(), GetMirror(), false, 100, 100);
	else
		DrawTexture(GetX(), GetY(), x1, y1, x2, y2,
			GetTexture(), GetMirror(), false, 100, 100);

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

void ANIMATEDSPRITE::Move()
{
	__super::Move();
	if(GetPatternCount() > 0)
	{
		int now = current_time;
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
							SetCurrentPattern(GetStartPattern());
							SetDoneAnimation(false);
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

void ANIMATEDSPRITE::LoadFromResource(RESOURCE* res)
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
		SetSitPattern(res->sit_pattern);
		SetSitCount(res->sit_count);
		SetSitShootPattern(res->sit_shoot_pattern);
		SetSitShootCount(res->sit_shoot_count);
		SetSitSpeed(res->sit_speed);
		SetSitShootSpeed(res->sit_shoot_speed);
		SetDamagePattern(res->damage_pattern);
		SetDamageCount(res->damage_count);
		SetDamageSpeed(res->damage_speed);
		SetDiePattern(res->die_pattern);
		SetDieCount(res->die_count);
		SetDieSpeed(res->die_speed);
	}
}


ANIMATEDSPRITE::ANIMATEDSPRITE()
{
	__super::SPRITE();
	SetStartPattern(0);
	SetCurrentPattern(0);
	SetPatternCount(0);
	SetRealPatternCount(0);
	SetTexture(NULL);
	SetIdleSpeed(MOVE_DELAY);
	SetStandSpeed(MOVE_DELAY);
	SetWalkSpeed(MOVE_DELAY);
	SetJumpSpeed(MOVE_DELAY);
	SetLandSpeed(MOVE_DELAY);
	SetShootSpeed(MOVE_DELAY);
	SetSitShootSpeed(MOVE_DELAY);
	SetDamageSpeed(MOVE_DELAY);
	SetDieSpeed(MOVE_DELAY);
	SetIdlePattern(0);
	SetWalkPattern(0);
	SetJumpPattern(0);
	SetShootPattern(0);
	SetStandPattern(0);
	SetSitPattern(0);
	SetDiePattern(0);
	SetIdleCount(0);
	SetWalkCount(0);
	SetStandCount(0);
	SetJumpCount(0);
	SetSitCount(0);
	SetDamageCount(0);
	SetLandPattern(0);
	SetLandCount(0);
	SetShootCount(0);
	SetDieCount(0);
	SetSitShootCount(0);
	SetDamageCount(0);
	SetJumpDownPattern(0);
	SetCurrentAnimation(animNone, true);
	SetReverse(false);
	SetMoving(false);
	SetMovingDirection(false);
	SetLanding(false);
	SetYStart(0);
	SetDoneAnimation(false);
	SetAnimationSpeed(MOVE_DELAY);
	SetPrevAnimTick(current_time);
	SetJumping(false);
	SetJumpDirection(false);
	SetYSpeed(0);
	SetSitting(false);
	SetShooting(false);
	SetDamaging(false);
	SetDying(false);
}