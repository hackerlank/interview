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

	int speedX;
    float speedY;

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
	bool walking;
	bool idling;
	bool standing;


	int yStart;
	int animationSpeed;
	int prevAnimTick;
	bool looped;
	bool reverse;

	bool walkingDirection;
	ANIMATION_TYPE currentAnimation;

	int realSitX;
	int realSitY;
	int realSitMirrorX;
	int realSitMirrorY;
	int realSitWidth;
	int realSitHeight;

	int realJumpX;
	int realJumpY;
	int realJumpMirrorX;
	int realJumpMirrorY;
	int realJumpWidth;
	int realJumpHeight;

	int realWalkX;
	int realWalkY;
	int realWalkMirrorX;
	int realWalkMirrorY;
	int realWalkWidth;
	int realWalkHeight;

	int realStandX;
	int realStandY;
	int realStandMirrorX;
	int realStandMirrorY;
	int realStandWidth;
	int realStandHeight;


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

	int flashJumpX;
	int flashJumpY;
	int flashWalkX;
	int flashWalkY;
	int flashStandX;
	int flashStandY;
	int flashSitX;
	int flashSitY;

	int flashJumpMirrorX;
	int flashJumpMirrorY;
	int flashWalkMirrorX;
	int flashWalkMirrorY;
	int flashStandMirrorX;
	int flashStandMirrorY;
	int flashSitMirrorX;
	int flashSitMirrorY;


public:
	int GetFlashJumpX() const { return flashJumpX; }
	int GetFlashJumpY() const { return flashJumpY; }
	int GetFlashWalkX() const { return flashWalkX; }
	int GetFlashWalkY() const { return flashWalkY; }
	int GetFlashStandX() const { return flashStandX; }
	int GetFlashStandY() const { return flashStandY; }
	int GetFlashSitX() const { return flashSitX; }
	int GetFlashSitY() const { return flashSitY; }

	void SetFlashJumpX(int new_value) { flashJumpX = new_value; }
	void SetFlashJumpY(int new_value) { flashJumpY = new_value; }
	void SetFlashWalkX(int new_value) { flashWalkX = new_value; }
	void SetFlashWalkY(int new_value) { flashWalkY = new_value; }
	void SetFlashStandX(int new_value) { flashStandX = new_value; }
	void SetFlashStandY(int new_value) { flashStandY = new_value; }
	void SetFlashSitX(int new_value) { flashSitX = new_value; }
	void SetFlashSitY(int new_value) { flashSitY = new_value; }

	int GetFlashJumpMirrorX() const { return flashJumpMirrorX; }
	int GetFlashJumpMirrorY() const { return flashJumpMirrorY; }
	int GetFlashWalkMirrorX() const { return flashWalkMirrorX; }
	int GetFlashWalkMirrorY() const { return flashWalkMirrorY; }
	int GetFlashStandMirrorX() const { return flashStandMirrorX; }
	int GetFlashStandMirrorY() const { return flashStandMirrorY; }
	int GetFlashSitMirrorX() const { return flashSitMirrorX; }
	int GetFlashSitMirrorY() const { return flashSitMirrorY; }

	void SetFlashJumpMirrorX(int new_value) { flashJumpMirrorX = new_value; }
	void SetFlashJumpMirrorY(int new_value) { flashJumpMirrorY = new_value; }
	void SetFlashSitMirrorX(int new_value) { flashSitMirrorX = new_value; }
	void SetFlashSitMirrorY(int new_value) { flashSitMirrorY = new_value; }
	void SetFlashWalkMirrorX(int new_value) { flashWalkMirrorX = new_value; }
	void SetFlashWalkMirrorY(int new_value) { flashWalkMirrorY = new_value; }
	void SetFlashStandMirrorX(int new_value) { flashStandMirrorX = new_value; }
	void SetFlashStandMirrorY(int new_value) { flashStandMirrorY = new_value; }

	int GetRealWalkX() const { return realWalkX; }
	int GetRealWalkY() const { return realWalkY; }
	int GetRealStandX() const { return realStandX; }
	int GetRealStandY() const { return realStandY; }
	int GetRealSitX() const { return realSitX; }
	int GetRealSitY() const { return realSitY; }
	int GetRealJumpX() const { return realJumpX; }
	int GetRealJumpY() const { return realJumpY; }
	int GetRealWalkMirrorX() const { return realWalkMirrorX; }
	int GetRealWalkMirrorY() const { return realWalkMirrorY; }
	int GetRealStandMirrorX() const { return realStandMirrorX; }
	int GetRealStandMirrorY() const { return realStandMirrorY; }
	int GetRealSitMirrorX() const { return realSitMirrorX; }
	int GetRealSitMirrorY() const { return realSitMirrorY; }
	int GetRealJumpMirrorX() const { return realJumpMirrorX; }
	int GetRealJumpMirrorY() const { return realJumpMirrorY; }

	void SetRealWalkX(int new_value) { realWalkX = new_value; }
	void SetRealWalkY(int new_value) { realWalkY = new_value; }
	void SetRealStandX(int new_value) { realStandX = new_value; }
	void SetRealStandY(int new_value) { realStandY = new_value; }
	void SetRealSitX(int new_value) { realSitX = new_value; }
	void SetRealSitY(int new_value) { realSitY = new_value; }
	void SetRealJumpX(int new_value) { realJumpX = new_value; }
	void SetRealJumpY(int new_value) { realJumpY = new_value; }
	void SetRealWalkMirrorX(int new_value) { realWalkMirrorX = new_value; }
	void SetRealWalkMirrorY(int new_value) { realWalkMirrorY = new_value; }
	void SetRealStandMirrorX(int new_value) { realStandMirrorX = new_value; }
	void SetRealStandMirrorY(int new_value) { realStandMirrorY = new_value; }
	void SetRealSitMirrorX(int new_value) { realSitMirrorX = new_value; }
	void SetRealSitMirrorY(int new_value) { realSitMirrorY = new_value; }
	void SetRealJumpMirrorX(int new_value) { realJumpMirrorX = new_value; }
	void SetRealJumpMirrorY(int new_value) { realJumpMirrorY = new_value; }

	int GetRealWalkWidth() const { return realWalkWidth; }
	int GetRealWalkHeight() const { return realWalkHeight; }
	int GetRealStandWidth() const { return realStandWidth; }
	int GetRealStandHeight() const { return realStandHeight; }
	int GetRealJumpWidth() const { return realJumpWidth; }
	int GetRealJumpHeight() const { return realJumpHeight; }
	int GetRealSitWidth() const { return realSitWidth; }
	int GetRealSitHeight() const { return realSitHeight; }

	void SetRealWalkWidth(int new_value)  { realWalkWidth = new_value; }
	void SetRealWalkHeight(int new_value)  { realWalkHeight = new_value; }
	void SetRealStandWidth(int new_value)  { realStandWidth = new_value; }
	void SetRealStandHeight(int new_value)  { realStandHeight = new_value; }
	void SetRealJumpWidth(int new_value)  { realJumpWidth = new_value; }
	void SetRealJumpHeight(int new_value)  { realJumpHeight = new_value; }
	void SetRealSitWidth(int new_value)  { realSitWidth = new_value; }
	void SetRealSitHeight(int new_value)  { realSitHeight = new_value; }


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

	bool GetStanding() const { return standing; }
	void SetStanding(bool new_value) { standing = new_value; }

	bool GetIdling() const { return idling; }
	void SetIdling(bool new_value) { idling = new_value; }

	int GetLandPattern() const { return landPattern; }
	void SetLandPattern(int new_value) { landPattern = new_value; }

	int GetLandCount() const { return landCount; }
	void SetLandCount(int new_value) { landCount = new_value; }

	int GetYStart() const { return yStart; }
	void SetYStart(int new_value) { yStart = new_value; }

	float GetSpeedY() const { return speedY; }
	void SetSpeedY(float new_value) { speedY = new_value; }

	int GetSpeedX() const { return speedX; }
	void SetSpeedX(int new_value) { speedX = new_value; }

	bool GetJumping() const { return jumping; }
	void SetJumping(bool new_value) { jumping = new_value; }

	bool GetSitting() const { return sitting; }
	void SetSitting(bool new_value) { sitting = new_value; }

	bool GetWalking() const { return walking; }
	void SetWalking(bool new_value) { walking = new_value; }

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

	bool GetWalkingDirection() const { return walkingDirection; }
	void SetWalkingDirection(bool new_value) { walkingDirection = new_value; }

	ANIMATION_TYPE GetCurrentAnimation() const { return currentAnimation; }
	void SetCurrentAnimation(enum ANIMATION_TYPE type, bool looped);
	virtual void Draw();
	virtual void Move();
	virtual void LoadFromResource(RESOURCE* res);
	ANIMATEDSPRITE();
};

#endif