#ifndef __ANIMATEDSPRITE_H_
#define __ANIMATEDSPRITE_H_
#include "iichan_sprite.h"

enum ANIMATION_TYPE { 
	animShoot,
	animIdle,
	animWalk,
	animSitShoot,
	animDamage,
	animJump,
	animStand,
	animSit,
	animLand,
	animDie,
	animNone
};


class ANIMATEDSPRITE : public SPRITE
{
private:
	int patternCount;
	int currentPattern;
	int startPattern;
	int realPatternCount;

    float ySpeed;
	int shootPattern;
	int shootCount;
	int shootAnimationSpeed;
	int jumpPattern;
	int jumpCount;
	int jumpDownPattern;
	int jumpAnimationSpeed;
	int walkPattern;
	int walkCount;
	int idlePattern;
	int idleCount;
	int standPattern;
	int standCount;
	int sitPattern;
	int sitCount;
	int sitShootPattern;
	int sitShootCount;
	int landPattern;
	int landCount;
	int damagePattern;
	int damageCount;
	int diePattern;
	int dieCount;

	bool doneAnimation;
	bool jumpDirection;

	bool jumping;
	bool landing;
	bool sitting;
	bool shooting;
	bool damaging;
	bool dying;

	int yStart;
	int animationSpeed;
	int prevAnimTick;
	bool looped;
	bool reverse;
	bool moving;
	bool movingDirection;
	ANIMATION_TYPE currentAnimation;



	int shootSpeed;
	int walkSpeed;
	int jumpSpeed;
	int idleSpeed;
	int standSpeed;
	int landSpeed;
	int sitSpeed;
	int sitShootSpeed;
	int damageSpeed;
	int dieSpeed;

public:

	int GetShootSpeed() const { return shootSpeed; }
	int GetWalkSpeed() const { return walkSpeed; }
	int GetJumpSpeed() const { return jumpSpeed; }
	int GetIdleSpeed() const { return idleSpeed; }
	int GetStandSpeed() const { return standSpeed; }
	int GetLandSpeed() const { return landSpeed; }
	int GetSitSpeed() const { return sitSpeed; }
	int GetSitShootSpeed() const { return sitShootSpeed; }
	int GetDamageSpeed() const { return damageSpeed; }
	int GetDieSpeed() const { return dieSpeed; }
	void SetShootSpeed(int new_value) { shootSpeed = new_value; }
	void SetWalkSpeed(int new_value) { walkSpeed = new_value; }
	void SetJumpSpeed(int new_value) { jumpSpeed = new_value; }
	void SetIdleSpeed(int new_value) { idleSpeed = new_value; }
	void SetStandSpeed(int new_value) { standSpeed = new_value; }
	void SetLandSpeed(int new_value) { landSpeed = new_value; }
	void SetSitSpeed(int new_value) { sitSpeed = new_value; }
	void SetSitShootSpeed(int new_value) { sitShootSpeed = new_value; }
	void SetDamageSpeed(int new_value) { damageSpeed = new_value; }
	void SetDieSpeed(int new_value) { dieSpeed = new_value; }

	int GetAnimationSpeed() const { return animationSpeed; }
	void SetAnimationSpeed(int new_value) { animationSpeed = new_value; }

	int GetPrevAnimTick() const { return prevAnimTick; }
	void SetPrevAnimTick(int new_value) { prevAnimTick = new_value; }

	bool GetDoneAnimation() const { return doneAnimation; }
	void SetDoneAnimation(bool new_value) { doneAnimation = new_value; }

	bool GetLanding() const { return landing; }
	void SetLanding(bool new_value) { landing = new_value ; }

	bool GetShooting() const { return shooting; }
	void SetShooting(bool new_value) { shooting = new_value; }

	bool GetDamaging() const { return damaging; }
	void SetDamaging(bool new_value) { damaging = new_value; }

	int GetLandPattern() const { return landPattern; }
	void SetLandPattern(int new_value) { landPattern = new_value; }

	int GetLandCount() const { return landCount; }
	void SetLandCount(int new_value) { landCount = new_value; }

	int GetYStart() const { return yStart; }
	void SetYStart(int new_value) { yStart = new_value; }

	float GetYSpeed() const { return ySpeed; }
	void SetYSpeed(float new_value) { ySpeed = new_value; }

	bool GetJumping() const { return jumping; }
	void SetJumping(bool new_value) { jumping = new_value; }

	bool GetSitting() const { return sitting; }
	void SetSitting(bool new_value) { sitting = new_value; }

	bool GetMoving() const { return moving; }
	void SetMoving(bool new_value) { moving = new_value; }

	bool GetDying() const { return dying; }
	void SetDying(bool new_value) { dying = new_value; }

	bool GetJumpDirection() const { return jumpDirection; }
	void SetJumpDirection(bool new_value) { jumpDirection = new_value; }

	int GetShootPattern() const { return shootPattern; }
	void SetShootPattern(int new_value) { shootPattern = new_value; }
	int GetShootCount() const { return shootCount; }
	void SetShootCount(int new_value) { shootCount = new_value; }

	int GetJumpPattern() const { return jumpPattern; }
	void SetJumpPattern(int new_value) { jumpPattern = new_value; }
	int GetJumpDownPattern() const { return jumpDownPattern; }
	void SetJumpDownPattern(int new_value) { jumpDownPattern = new_value; }
	int GetJumpCount() const { return jumpCount; }
	void SetJumpCount(int new_value) { jumpCount = new_value; }

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

	int GetSitShootPattern() const { return sitShootPattern; }
	void SetSitShootPattern(int new_value) { sitShootPattern = new_value; }
	int GetSitShootCount() const { return sitShootCount; }
	void SetSitShootCount(int new_value) { sitShootCount = new_value; }

	int GetDamagePattern() const { return damagePattern; }
	void SetDamagePattern(int new_value) { damagePattern = new_value; }
	int GetDamageCount() const { return damageCount; }
	void SetDamageCount(int new_value) { damageCount = new_value; }

	int GetDiePattern() const { return diePattern; }
	void SetDiePattern(int new_value) { diePattern = new_value; }
	int GetDieCount() const { return dieCount; }
	void SetDieCount(int new_value) { dieCount = new_value; }

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

	bool GetMovingDirection() const { return movingDirection; }
	void SetMovingDirection(bool new_value) { movingDirection = new_value; }

	ANIMATION_TYPE GetCurrentAnimation() const { return currentAnimation; }
	void SetCurrentAnimation(enum ANIMATION_TYPE type, bool looped);
	virtual void Draw();
	virtual void Move();
	virtual void LoadFromResource(RESOURCE* res);
	ANIMATEDSPRITE();
};

#endif