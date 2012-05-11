class IICHAN;


typedef struct tagSPRITE {
	texture_t* tex;
	int x;
	int y;
	int patternWidth;
	int patternHeight;
	int imageWidth;
	int imageHeight;
	bool mirror;
	IICHAN* iichan;

	int GetX() const { return x; }
	void SetX(int new_value) { x = new_value; }
	int GetY() const { return y; }
	void SetY(int new_value) { y = new_value; }
	IICHAN* GetIICHAN() const { return iichan; }
	void SetIICHAN(IICHAN* new_value) { iichan = new_value; }

	texture_t* GetTexture() const { return tex; }
	void SetTexture(texture_t* new_value) { tex = new_value; }

	int GetPatternWidth() const { return patternWidth; }
	void SetPatternWidth(int new_value) { patternWidth = new_value; }
	int GetPatternHeight() const { return patternHeight; }
	void SetPatternHeight(int new_value) { patternHeight = new_value; }

	int GetImageWidth() const { return imageWidth; }
	void SetImageWidth(int new_value) { imageWidth = new_value; }
	int GetImageHeight() const { return imageHeight; }
	void SetImageHeight(int new_value) { imageHeight = new_value; }

	
	bool GetMirror() const { return mirror; }
	void SetMirror(bool new_value) { mirror = new_value; }

	virtual void Draw()
	{
		DrawTexture(x, y, 0, 0, GetPatternWidth(), GetPatternHeight(),
			GetTexture(), GetMirror(), false, 100, 100);
	}

} SPRITE;

typedef struct tagBULLET : SPRITE {
	int life;
	int speed;
	int damage;

	void SetLife(int new_value) { life = new_value; }
	int GetLife() const { return life; }

	void SetSpeed(int new_value) { speed = new_value; }
	int GetSpeed() const { return speed; }

	void SetDamage(int new_value) { damage = new_value; }
	int GetDamage() const { return damage; }
} BULLET;

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
	animNone
};


typedef struct tagANIMATEDSPRITE : SPRITE {
	int patternCount;
	int currentPattern;
	int startPattern;


	int shoot1Pattern;
	int shoot1Count;
	int shoot2Pattern;
	int shoot2Count;
	int shoot3Pattern;
	int shoot3Count;
	int jumpPattern;
	int jumpCount;
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

	bool GetLooped() const { return looped; }
	void SetLooped(bool new_value) { looped = new_value; }
	
	bool GetReverse() const { return reverse; }
	void SetReverse(bool new_value) { reverse = new_value; }

	bool GetMoving() const { return moving; }
	void SetMoving(bool new_value) { moving = new_value; }

	bool GetMovingDirection() const { return movingDirection; }
	void SetMovingDirection(bool new_value) { movingDirection = new_value; }

	ANIMATION_TYPE GetCurrentAnimation() const { return currentAnimation; }
	void SetCurrentAnimation(enum ANIMATION_TYPE type, bool looped)
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
			default:
				break;
			}
			SetCurrentPattern(GetStartPattern());
			currentAnimation = type;
			SetLooped(looped);
		}
	}


	virtual void Draw()
	{
		int colCount = GetImageWidth() / GetPatternWidth();
		int rowCount = GetImageHeight() / GetPatternHeight();

		int col = GetCurrentPattern() % colCount;
		int row = GetCurrentPattern() / colCount;

		int x1 = col * GetPatternWidth();
		int x2 = x1 + GetPatternWidth();
		int y1 = row * GetPatternHeight();
		int y2 = y1 + GetPatternHeight();

		DrawTexture(x, y, x1, y1, x2, y2, 
			GetTexture(), GetMirror(), false, 100, 100);
	}

	virtual void Move()
	{
		if(GetPatternCount() != 0)
		{
			if(!GetReverse())
			{		
				if(GetCurrentPattern() - GetStartPattern() == GetPatternCount()-1)
				{
					SetCurrentPattern(GetStartPattern());
					if(!GetLooped())
						SetPatternCount(0);
				}
				else
					SetCurrentPattern(GetCurrentPattern() + 1);
			}
			else
			{
				if(GetStartPattern() - GetCurrentPattern() == GetPatternCount()-1)
				{
					SetCurrentPattern(GetStartPattern());
					if(!GetLooped())
						SetPatternCount(0);
				}
				else
				{
					if(GetCurrentPattern() == 0)
						SetCurrentPattern(GetStartPattern());
					else
						SetCurrentPattern(GetCurrentPattern() - 1);
				}
			}
		}
	}

	tagANIMATEDSPRITE()
	{
		SetMirror(false);
		SetStartPattern(0);
		SetCurrentPattern(0);
		SetPatternWidth(0);
		SetPatternHeight(0);
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
		SetShoot1Count(0);
		SetShoot2Count(0);
		SetShoot3Count(0);
		SetStandCount(0);
		SetSitCount(0);
		SetX(0);
		SetY(0);
		SetCurrentAnimation(animNone, true);
		SetReverse(false);
		SetMoving(false);
		SetMovingDirection(false);
	}

	//tagANIMATEDSPRITE(
	//	texture_t* texture,
	//	int x,
	//	int y,
	//	int pattern_width,
	//	int pattern_height,
	//	int image_width,
	//	int image_height,
	//	int shoot1_p,
	//	int shoot1_c,
	//	int shoot2_p,
	//	int shoot2_c,
	//	int shoot3_p,
	//	int shoot3_c,
	//	int walk_p,
	//	int walk_c,
	//	int stand_p,
	//	int stand_c,
	//	int sit_p,
	//	int sit_c,
	//	int idle_p,
	//	int idle_c,
	//	int talk_p,
	//	int talk_c,
	//	ANIMATION_TYPE current_animation,
	//	bool looped
	//	)
	//{
	//	SetTexture(texture);
	//	SetX(x);
	//	SetY(y);
	//	SetPatternWidth(pattern_width);
	//	SetPatternHeight(pattern_height);
	//	SetImageWidth(image_width);
	//	SetImageHeight(image_height);
	//	SetShoot1Pattern(shoot1_p);
	//	SetShoot1Count(shoot1_c);
	//	SetShoot2Pattern(shoot2_p);
	//	SetShoot2Count(shoot2_c);
	//	SetShoot3Pattern(shoot3_p);
	//	SetShoot3Count(shoot3_c);
	//	SetWalkPattern(walk_p);
	//	SetWalkCount(walk_c);
	//	SetStandPattern(stand_p);
	//	SetStandCount(stand_c);
	//	SetSitPattern(sit_p);
	//	SetSitCount(sit_c);
	//	SetIdlePattern(idle_p);
	//	SetIdleCount(idle_c);
	//	SetTalkPattern(talk_p);
	//	SetTalkCount(talk_c);
	//	SetCurrentAnimation(current_animation, looped);
	//}


	//virtual void Process();
} ANIMATEDSPRITE;

enum POWERUP_TYPE { pwrHealth, pwrScore, pwrAmmo, pwrNone };

typedef struct tagPOWERUP : ANIMATEDSPRITE {
	POWERUP_TYPE type;
	int count;
	bool dead;

	POWERUP_TYPE GetType() const { return type; }
	void SetType(enum POWERUP_TYPE new_type) { type = new_type; }

	int GetCount() const { return count; }
	void SetCount(int new_count) { count = new_count; }

	bool GetDead() const { return dead; }
	void SetDead(bool new_dead) { dead = new_dead; }

	tagPOWERUP(enum POWERUP_TYPE type, int count);
} POWERUP;

typedef struct tagPHRASE : SPRITE {
	string text;
	int time;
	bool dead;

	string GetText() const { return text; }
	void SetText(string new_value) { text = new_value; }

	int GetTime() const { return time; }
	void SetTime(int new_value) { time = new_value; }

	bool GetDead() const { return dead; }
	void SetDead(bool new_value) { dead = new_value; }

	virtual void Draw();

	tagPHRASE(string text, int time)
	{
		SetText(text);
		SetTime(time);
		SetDead(false);
	}
} PHRASE;

typedef struct tagEXPLOSION : ANIMATEDSPRITE {
	bool dead;

	bool GetDead() const { return dead; }
	void SetDead(bool new_dead) { dead = new_dead; }

	virtual void Move()
	{
		//__super::Move();

		if(!GetReverse())
		{		
			if(GetCurrentPattern() - GetStartPattern() == GetPatternCount() - 1)
			{
				//MessageBox(0, "asdasd", 0 ,0);
				SetDead(true);
			}
			else
				SetCurrentPattern(GetCurrentPattern() + 1);
		}
		else
		{
			if(GetStartPattern() - GetCurrentPattern() == GetPatternCount()-1)
			{
				SetDead(true);
			}
			else
			{
				if(GetCurrentPattern() == 0)
					SetCurrentPattern(GetStartPattern());
				else
					SetCurrentPattern(GetCurrentPattern() - 1);
			}
		}
	}
	tagEXPLOSION()
	{
		__super::tagANIMATEDSPRITE();
	}
} EXPLOSION;


typedef struct tagCHARACTER : ANIMATEDSPRITE {
	int health;
	int ammo;

	bool jumping;
	int jumps;
	bool jumpDirection;

	bool shooting;
	int shoots;

	bool dead;

	void Shoot();

	bool GetDead() const { return dead; }
	void SetDead(bool new_value) { dead = new_value; }

	bool GetJumping() const { return jumping; }
	void SetJumping(bool new_value) { jumping = new_value; }
	int GetJumps() const { return jumps; }
	void SetJumps(int new_value) { jumps = new_value; }
	bool GetJumpDirection() const { return jumpDirection; }
	void SetJumpDirection(bool new_value) { jumpDirection = new_value; }
	bool GetShooting() const { return shooting; }
	void SetShooting(bool new_value) { shooting = new_value; }
	int GetShoots() const { return shoots; }
	void SetShoots(int new_value) { shoots = new_value; }

	int GetHealth() const { return health; }
	void SetHealth(int new_value) { health = new_value; }

	int GetAmmo() const { return ammo; }
	void SetAmmo(int new_value) { ammo = new_value; }
	tagCHARACTER()
	{
		__super::tagANIMATEDSPRITE();
		SetShoots(0);
		SetJumps(0);
		SetJumping(false);
		SetShooting(false);
		SetJumpDirection(false);
		SetDead(false);
	}

} CHARACTER;

typedef struct tagPLAYER : CHARACTER {
	int score;

	int GetScore() const { return score; }
	void SetScore(int new_value) { score = new_value; }

	void CollectPowerUp(enum POWERUP_TYPE type, int count);

	/*
		health
		ammo
		score

		texture
		x
		y
		pattern_width
		pattern_height
		image_width
		image_height

		shoot1_p
		shoot1_c
		shoot2_p
		shoot2_c
		shoot3_p
		shoot3_c
		walk_p
		walk_c
		stand_p
		stand_c
		sit_p
		sit_c
		idle_p
		idle_c
		talk_p
		talk_c
		current_animation
		looped
	*/

	tagPLAYER(int health, int ammo,	int score)
	{
		__super::tagCHARACTER();
		SetHealth(health);
		SetAmmo(ammo);
		SetScore(score);
	}

} PLAYER;

typedef struct tagLEVELCELL {
	BYTE index;
	WORD x;
	WORD y;
} LEVELCELL;


typedef struct tagLEVEL {
	UINT sizeX;
	UINT sizeY;
	UINT heroX;
	UINT heroY;
	LEVELCELL* cells;
} LEVEL;


class IICHAN {
private:
	PLAYER* player;
	vector<BULLET*> bullets;
	//vector<LEVEL*> levels;
	vector<POWERUP*> powerups;
	vector<SPRITE*> background;
	vector<EXPLOSION*> explosions;
	vector<CHARACTER*> enemies;
	vector<PHRASE*> phrases;

public:
	IICHAN();

	PLAYER* GetPlayer() const { return player; }
	void SetPlayer(PLAYER* new_player) { player = new_player; }

	void ProcessPlayer();

	void ProcessBullets();
	void MoveBullets();

	void ProcessPowerups();
	void MovePowerups();

	void ProcessExplosions();
	void MoveExplosions();

	void ProcessEnemies();
	void MoveEnemies();

	void ProcessPhrases();
	void MovePhrases();

	void Process();

	
	void Draw();

	void AddBullet(BULLET* spr);
	void AddPowerup(POWERUP* pwr);
	void AddBackground(SPRITE* spr);
	void AddExplosion(EXPLOSION* exp);
	void AddEnemy(CHARACTER* enemy);
	void AddPhrase(PHRASE* p);

	void CreatePowerup(enum POWERUP_TYPE type, int count, int x, int y);
	void CreateExplosion(int x, int y, bool reverse);
	void CreateEnemy(int x, int y, int health);
	void CreatePhrase(int x, int y, string text, int time);

	bool LoadLevel(const char* file_name);
	void Init();

	void Destroy();

};





