#include "stdafx.h"
#include "animatedsprite.h"
#include "misc.h"

extern int WORLD_X;
extern int WORLD_Y;

void ANIMATEDSPRITE::SetCurrentAnimation(enum ANIMATION_TYPE type, bool looped)
{
	if(GetCurrentAnimation() != type)
	{
		switch(type)
		{
		case animShoot1:
			SetStartPattern(GetShoot1Pattern());
			SetPatternCount(GetShoot1Count());
			break;
		case animShoot2:
			SetStartPattern(GetShoot2Pattern());
			SetPatternCount(GetShoot2Count());
			break;
		case animShoot3:
			SetStartPattern(GetShoot3Pattern());
			SetPatternCount(GetShoot3Count());
			break;
		case animTalk:
			SetStartPattern(GetTalkPattern());
			SetPatternCount(GetTalkCount());
			break;
		case animIdle:
			SetStartPattern(GetIdlePattern());
			SetPatternCount(GetIdleCount());
			break;
		case animWalk:
			SetStartPattern(GetWalkPattern());
			SetPatternCount(GetWalkCount());
			break;
		case animJump:
			SetStartPattern(GetJumpPattern());
			SetPatternCount(GetJumpCount());
			break;
		case animStand:
			SetStartPattern(GetStandPattern());
			SetPatternCount(GetStandCount());
			break;
		case animSit:
			SetStartPattern(GetSitPattern());
			SetPatternCount(GetSitCount());
			break;
		case animLand:
			SetStartPattern(GetLandPattern());
			SetPatternCount(GetLandCount());
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
	int colCount = GetImageWidth() / GetPatternWidth();
	//int rowCount = GetImageHeight() / GetPatternHeight();

	int col = GetCurrentPattern() % colCount;
	int row = GetCurrentPattern() / colCount;

	
	int x1 = col * GetPatternWidth();
	int x2 = x1 + GetPatternWidth();
	int y1 = row * GetPatternHeight();
	int y2 = y1 + GetPatternHeight();

	DrawTexture(GetX() - WORLD_X, GetY() + WORLD_Y, x1, y1, x2, y2, 
		GetTexture(), GetMirror(), false, 100, 100);
}

void ANIMATEDSPRITE::Move()
{

	if(GetPatternCount() != 0)
	{

		if(!GetReverse())
		{		
			if((GetCurrentPattern() == GetRealPatternCount() - 1)
				/*|| (GetStartPattern() + GetPatternCount() > GetRealPatternCount())*/)
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
						//else
						//{
						//	if(GetYSpeed() == 0)
						//	{
						//		SetCurrentPattern(GetCurrentPattern() + 1);
						//	}
						//}
					}

				}
			}
		}
		else
		{
			//if(GetCurrentPattern() == 0)
			//{
			//	SetCurrentPattern(GetStartPattern());
			//}
			//else
			//{
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
			//}
		}
	}
}


ANIMATEDSPRITE::tagANIMATEDSPRITE()
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
	SetIdlePattern(0);
	SetWalkPattern(0);
	SetTalkPattern(0);
	SetJumpPattern(0);
	SetShoot1Pattern(0);
	SetShoot2Pattern(0);
	SetShoot3Pattern(0);
	SetStandPattern(0);
	SetSitPattern(0);
	SetIdleCount(0);
	SetWalkCount(0);
	SetTalkCount(0);
	SetJumpCount(0);
	SetLandPattern(0);
	SetLandCount(0);
	SetShoot1Count(0);
	SetShoot2Count(0);
	SetShoot3Count(0);
	SetStandCount(0);
	SetSitCount(0);
	SetJumpDownPattern(0);
	SetX(0);
	SetY(0);
	SetCurrentAnimation(animNone, true);
	SetReverse(false);
	SetMoving(false);
	SetMovingDirection(false);
	SetLanding(false);
	SetYStart(0);
	SetDoneAnimation(true);
	SetAnimationSpeed(100);
	SetPrevAnimTick(0);
}