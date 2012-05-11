#ifndef __ANIMATEDSPRITE_H_
#define __ANIMATEDSPRITE_H_
#include "sprite.h"

enum ANIMATION_TYPE { 
	animShoot1,
	animShoot2,
	animShoot3,
	animIdle,
	animWalk,
	animTalk,
	animJump,
	animStand,
	animSit,
	animLand,
	animNone
};

typedef struct tagANIMATEDSPRITE : SPRITE {
	int patternCount;
	int currentPattern;
	int startPattern;
	int realPatternCount;

	int shoot1Pattern;
	int shoot1Count;
	int shoot2Pattern;
	int shoot2Count;
	int shoot3Pattern;
	int shoot3Count;
	int jumpPattern;
	int jumpCount;
	int jumpDownPattern;
	int talkPattern;
	int talkCount;
	int walkPattern;
	int walkCount;
	int idlePattern;
	int idleCount;
	int standPattern;
	int standCount;
	int sitPattern;
	int sitCount;
	int landPattern;
	int landCount;

	bool doneAnimation;

	bool jumping;
	bool jumpDirection;

	bool landing;

	int yStart;

	int animationSpeed;

	int prevAnimTick;

	int GetAnimationSpeed() const { return animationSpeed; }
	void SetAnimationSpeed(int new_value) { animationSpeed = new_value; }

	int GetPrevAnimTick() const { return prevAnimTick; }
	void SetPrevAnimTick(int new_value) { prevAnimTick = new_value; }

	bool GetDoneAnimation() const { return doneAnimation; }
	void SetDoneAnimation(bool new_value) { doneAnimation = new_value; }

	bool GetLanding() const { return landing; }
	void SetLanding(bool new_value) { landing = new_value ; }

	int GetLandPattern() const { return landPattern; }
	void SetLandPattern(int new_value) { landPattern = new_value; }

	int GetLandCount() const { return landCount; }
	void SetLandCount(int new_value) { landCount = new_value; }

	float ySpeed;
	int GetYStart() const { return yStart; }
	void SetYStart(int new_value) { yStart = new_value; }

	float GetYSpeed() const { return ySpeed; }
	void SetYSpeed(float new_value) { ySpeed = new_value; }

	bool GetJumping() const { return jumping; }
	void SetJumping(bool new_value) { jumping = new_value; }

	bool GetJumpDirection() const { return jumpDirection; }
	void SetJumpDirection(bool new_value) { jumpDirection = new_value; }

	ANIMATION_TYPE currentAnimation;
	bool looped;
	bool reverse;
	bool moving;
	bool movingDirection;

	int GetShoot1Pattern() const { return shoot1Pattern; }
	void SetShoot1Pattern(int new_value) { shoot1Pattern = new_value; }
	int GetShoot1Count() const { return shoot1Count; }
	void SetShoot1Count(int new_value) { shoot1Count = new_value; }

	int GetShoot2Pattern() const { return shoot2Pattern; }
	void SetShoot2Pattern(int new_value) { shoot2Pattern = new_value; }
	int GetShoot2Count() const { return shoot2Count; }
	void SetShoot2Count(int new_value) { shoot2Count = new_value; }

	int GetShoot3Pattern() const { return shoot3Pattern; }
	void SetShoot3Pattern(int new_value) { shoot3Pattern = new_value; }
	int GetShoot3Count() const { return shoot3Count; }
	void SetShoot3Count(int new_value) { shoot3Count = new_value; }

	int GetJumpPattern() const { return jumpPattern; }
	void SetJumpPattern(int new_value) { jumpPattern = new_value; }
	int GetJumpDownPattern() const { return jumpDownPattern; }
	void SetJumpDownPattern(int new_value) { jumpDownPattern = new_value; }
	int GetJumpCount() const { return jumpCount; }
	void SetJumpCount(int new_value) { jumpCount = new_value; }

	int GetTalkPattern() const { return talkPattern; }
	void SetTalkPattern(int new_value) { talkPattern = new_value; }
	int GetTalkCount() const { return talkCount; }
	void SetTalkCount(int new_value) { talkCount = new_value; }

	int GetWalkPattern() const { return walkPattern; }
	void SetWalkPattern(int new_value) { walkPattern = new_value; }
	int GetWalkCount() const { return walkCount; }
	void SetWalkCount(int new_value) { walkCount = new_value; }

	int GetIdlePattern() const { return idlePattern; }
	void SetIdlePattern(int new_value) { idlePattern = new_value; }
	int GetIdleCount() const { return idleCount; }
	void SetIdleCount(int new_value) { idleCount = new_value; }

	int GetStandPattern() const { return standPattern; }
	void SetStandPattern(int new_value) { standPattern = new_value; }
	int GetStandCount() const { return standCount; }
	void SetStandCount(int new_value) { standCount = new_value; }

	int GetSitPattern() const { return sitPattern; }
	void SetSitPattern(int new_value) { sitPattern = new_value; }
	int GetSitCount() const { return sitCount; }
	void SetSitCount(int new_value) { sitCount = new_value; }

	int GetCurrentPattern() const { return currentPattern; }
	void SetCurrentPattern(int new_value) { currentPattern = new_value; }

	int GetPatternCount() const { return patternCount; }
	void SetPatternCount(int new_value) { patternCount = new_value; }

	int GetStartPattern() const { return startPattern; }
	void SetStartPattern(int new_value) { startPattern = new_value; }

	int GetRealPatternCount() const { return realPatternCount; }
	void SetRealPatternCount(int new_value) { realPatternCount = new_value; }

	bool GetLooped() const { return looped; }
	void SetLooped(bool new_value) { looped = new_value; }
	
	bool GetReverse() const { return reverse; }
	void SetReverse(bool new_value) { reverse = new_value; }

	bool GetMoving() const { return moving; }
	void SetMoving(bool new_value) { moving = new_value; }

	bool GetMovingDirection() const { return movingDirection; }
	void SetMovingDirection(bool new_value) { movingDirection = new_value; }

	ANIMATION_TYPE GetCurrentAnimation() const { return currentAnimation; }
	void SetCurrentAnimation(enum ANIMATION_TYPE type, bool looped);


	virtual void Draw();


	virtual void Move();


	tagANIMATEDSPRITE();

} ANIMATEDSPRITE;

#endif