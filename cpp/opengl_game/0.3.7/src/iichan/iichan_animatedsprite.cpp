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
			SetRealX(GetRealWalkX());
			SetRealY(GetRealWalkY());
			SetRealMirrorX(GetRealWalkMirrorX());
			SetRealMirrorY(GetRealWalkMirrorY());
			SetRealWidth(GetRealWalkWidth());
			SetRealHeight(GetRealWalkHeight());
			break;
		case animJump:
			SetStartPattern(GetJumpPattern());
			SetPatternCount(GetJumpCount());
			SetAnimationSpeed(GetJumpSpeed());
			SetRealX(GetRealJumpX());
			SetRealY(GetRealJumpY());
			SetRealMirrorX(GetRealJumpMirrorX());
			SetRealMirrorY(GetRealJumpMirrorY());
			SetRealWidth(GetRealJumpWidth());
			SetRealHeight(GetRealJumpHeight());
			break;
		case animStand:
			SetStartPattern(GetStandPattern());
			SetPatternCount(GetStandCount());
			SetAnimationSpeed(GetStandSpeed());
			SetRealX(GetRealStandX());
			SetRealY(GetRealStandY());
			SetRealMirrorX(GetRealStandMirrorX());
			SetRealMirrorY(GetRealStandMirrorY());
			SetRealWidth(GetRealStandWidth());
			SetRealHeight(GetRealStandHeight());
			break;
		case animSit:
			SetStartPattern(GetSitPattern());
			SetPatternCount(GetSitCount());
			SetRealX(GetRealSitX());
			SetRealY(GetRealSitY());
			SetRealMirrorX(GetRealSitMirrorX());
			SetRealMirrorY(GetRealSitMirrorY());
			SetRealWidth(GetRealSitWidth());
			SetRealHeight(GetRealSitHeight());
			break;
		case animSitShoot:
			SetStartPattern(GetSitShootPattern());
			SetPatternCount(GetSitShootCount());
			break;
		case animLand:
			SetStartPattern(GetLandPattern());
			SetPatternCount(GetLandCount());
			SetAnimationSpeed(GetLandSpeed());
			SetRealX(GetRealStandX());
			SetRealY(GetRealStandY());
			SetRealMirrorX(GetRealStandMirrorX());
			SetRealMirrorY(GetRealStandMirrorY());
			SetRealWidth(GetRealStandWidth());
			SetRealHeight(GetRealStandHeight());
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
		SetSpeedX(res->speed_x);
		SetSpeedY((float)res->speed_y);

		SetFlashJumpX(res->flash_jump_x);
		SetFlashJumpY(res->flash_jump_y);
		SetFlashStandX(res->flash_stand_x);
		SetFlashStandY(res->flash_stand_y);
		SetFlashSitX(res->flash_sit_x);
		SetFlashSitY(res->flash_sit_y);
		SetFlashWalkX(res->flash_walk_x);
		SetFlashWalkY(res->flash_walk_y);
		SetFlashJumpMirrorX(res->flash_jump_mirror_x);
		SetFlashJumpMirrorY(res->flash_jump_mirror_y);
		SetFlashStandMirrorX(res->flash_stand_mirror_x);
		SetFlashStandMirrorY(res->flash_stand_mirror_y);
		SetFlashSitMirrorX(res->flash_sit_mirror_x);
		SetFlashSitMirrorY(res->flash_sit_mirror_y);
		SetFlashWalkMirrorX(res->flash_walk_mirror_x);
		SetFlashWalkMirrorY(res->flash_walk_mirror_y);

		SetRealStandX(res->real_stand_x);
		SetRealStandY(res->real_stand_y);
		SetRealStandWidth(res->real_stand_width);
		SetRealStandHeight(res->real_stand_height);
		SetRealWalkX(res->real_walk_x);
		SetRealWalkY(res->real_walk_y);
		SetRealWalkWidth(res->real_walk_width);
		SetRealWalkHeight(res->real_walk_height);
		SetRealSitX(res->real_sit_x);
		SetRealSitY(res->real_sit_y);
		SetRealSitWidth(res->real_sit_width);
		SetRealSitHeight(res->real_sit_height);
		SetRealJumpX(res->real_jump_x);
		SetRealJumpY(res->real_jump_y);
		SetRealJumpWidth(res->real_jump_width);
		SetRealJumpHeight(res->real_jump_height);
		SetRealStandMirrorX(res->real_stand_mirror_x);
		SetRealStandMirrorY(res->real_stand_mirror_y);
		SetRealWalkMirrorX(res->real_walk_mirror_x);
		SetRealWalkMirrorY(res->real_walk_mirror_y);
		SetRealSitMirrorX(res->real_sit_mirror_x);
		SetRealSitMirrorY(res->real_sit_mirror_y);
		SetRealJumpMirrorX(res->real_jump_mirror_x);
		SetRealJumpMirrorY(res->real_jump_mirror_y);		
	}
}


ANIMATEDSPRITE::ANIMATEDSPRITE()
{
	__super::SPRITE();

	SetRealStandX(0);
	SetRealStandY(0);
	SetRealSitX(0);
	SetRealSitY(0);
	SetRealWalkX(0);
	SetRealWalkY(0);
	SetRealJumpX(0);
	SetRealJumpY(0);

	SetRealStandMirrorX(0);
	SetRealStandMirrorY(0);
	SetRealSitMirrorX(0);
	SetRealSitMirrorY(0);
	SetRealWalkMirrorX(0);
	SetRealWalkMirrorY(0);
	SetRealJumpMirrorX(0);
	SetRealJumpMirrorY(0);

	SetRealStandWidth(0);
	SetRealStandHeight(0);
	SetRealSitWidth(0);
	SetRealSitHeight(0);
	SetRealWalkWidth(0);
	SetRealWalkHeight(0);
	SetRealJumpWidth(0);
	SetRealJumpHeight(0);


	SetFlashStandX(0);
	SetFlashStandY(0);
	SetFlashJumpX(0);
	SetFlashJumpY(0);
	SetFlashWalkX(0);
	SetFlashWalkY(0);
	SetFlashSitX(0);
	SetFlashSitY(0);
	SetFlashStandMirrorX(0);
	SetFlashStandMirrorY(0);
	SetFlashJumpMirrorX(0);
	SetFlashJumpMirrorY(0);
	SetFlashWalkMirrorX(0);
	SetFlashWalkMirrorY(0);
	SetFlashSitMirrorX(0);
	SetFlashSitMirrorY(0);

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
	SetWalking(false);
	SetWalkingDirection(false);
	SetLanding(false);
	SetYStart(0);
	SetDoneAnimation(false);
	SetAnimationSpeed(MOVE_DELAY);
	SetPrevAnimTick(current_time);
	SetJumping(false);
	SetJumpDirection(false);
	SetSpeedY(0);
	SetSpeedX(0);
	SetSitting(false);
	SetShooting(false);
	SetDamaging(false);
	SetIdling(false);
	SetStanding(true);
	SetDying(false);
}