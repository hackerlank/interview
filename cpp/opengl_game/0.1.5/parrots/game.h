typedef struct tagPLAYER {
	char name[20];
	std::vector<STARID> stars;
	int overall_RP;
	int money;
	RACE race;
	int _race;

	BYTE color[4];

} PLAYER;

typedef struct tagGAMESPRITE {
	texture_t* tex;
	int patternCount;
	int currentPattern;
	int patternWidth;
	int patternHeight;
	int x;
	int y;

	int GetCurrentPattern() const { return currentPattern; }
	void SetCurrentPattern(int new_value) { currentPattern = new_value; }
	int GetPatternWidth() const { return patternWidth; }
	void SetPatternWidth(int new_value) { patternWidth = new_value; }
	int GetPatternHeight() const { return patternHeight; }
	void SetPatternHeight(int new_value) { patternHeight = new_value; }

	virtual void Draw()
	{
		DrawTexture(x, y, GetCurrentPattern() * GetPatternWidth(), 0, GetCurrentPattern() * GetPatternWidth() + GetPatternWidth(), GetPatternHeight(), tex, false, false, 100, 100);
	}
} GAMESPRITE;

typedef struct tagSTAR : GAMESPRITE {
	LPSTR name;
	PLAYER* owner;
	STARID id;
	STARCOLOR starcolor;
	STARSIZE starsize;
	STARTYPE startype;

	void Draw();

} STAR;


class GAME {
private:
	std::vector<PLAYER> players;
	std::vector<STAR> stars;
	int playerCount;
	int starCount;
	int sizeX;
	int sizeY;
public:
	GAME(LPSTR playername, int difficulty_pick, int size_pick, int race_pick, int opponents_pick);
	~GAME();

	void Process();
	void Draw();

	void WriteDebug();
	void DebugToScreen();

	void InitPlayers(int race_pick, int opponents_pick);
	void InitStars(int size_pick);

	void SetPlayerCount(int new_value) { playerCount = new_value; }
	int GetPlayerCount() const { return playerCount; }
	void SetStarCount(int new_value) { starCount = new_value; }
	int GetStarCount() const { return starCount; }

	void AddPlayer(PLAYER new_player);
	PLAYER* PlayerByName(LPSTR name);
	void AddStar(STAR new_star);
	STAR* StarByName(LPSTR name);
	STAR* StarByID(STARID id);

	int GetSizeX() const { return sizeX; }
	int GetSizeY() const { return sizeY; }
	void SetSizeX(int new_value) { sizeX = new_value; }
	void SetSizeY(int new_value) { sizeY = new_value; }



	int GenerateStar(STAR* dest);
};






