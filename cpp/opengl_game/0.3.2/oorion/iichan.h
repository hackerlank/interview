
#include "player.h"
#include "iichan_small_types.h"


class IICHAN;
class HUD;

class HUD {
private:
	vector<SPRITE*> health;
	vector<SPRITE*> ammo;
	vector<SPRITE*> score;
	int healthCount;
	int ammoCount;
	int scoreCount;
public:
	void AddHealth();
	void AddAmmo();
	void AddScore();

	void RemoveHealth();
	void RemoveAmmo();
	void RemoveScore();

	void Draw();
	void Destroy();

	HUD()
	{
		health.resize(0);
		ammo.resize(0);
		score.resize(0);

		healthCount = 0;
		ammoCount = 0;
		scoreCount = 0;
	}
};


class IICHAN {
private:
	PLAYER* player;
	vector<BULLET*> bullets;
	vector<POWERUP*> powerups;
	vector<SPRITE*> background;
	vector<EXPLOSION*> explosions;
	vector<CHARACTER*> enemies;
	vector<PHRASE*> phrases;
	//vector<SPRITE*> objects;

	//int worldX;
	//int worldY;
	HUD* hud;

public:
	IICHAN();

	//int GetWorldX() const { return worldX; }
	//void SetWorldX(int new_value) { worldX = new_value; }
	//int GetWorldY() const { return worldY; }
	//void SetWorldY(int new_value) { worldY = new_value; }

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

	void DrawPlayer();
	void DrawEnemies();
	void DrawBullets();
	void DrawPowerups();
	void DrawExplosions();
	void DrawPhrases();
	void DrawBackground();
	void DrawHUD();
	
	void Draw();

	//void AddObject(SPRITE* obj);
	void AddBullet(BULLET* spr);
	void AddPowerup(POWERUP* pwr);
	void AddBackground(SPRITE* spr);
	void AddExplosion(EXPLOSION* exp);
	void AddEnemy(CHARACTER* enemy);
	void AddPhrase(PHRASE* p);

	void CreatePowerup(enum POWERUP_TYPE type, int count, int x, int y);
	void CreateExplosion(int x, int y, bool reverse, bool mirror);
	void CreateEnemy(int x, int y, int health);
	void CreatePhrase(int x, int y, string text, int time);
	void CreateBullet(int x, int y, bool mirror);

	void DeletePowerups();
	void DeleteExplosions();
	void DeleteEnemies();
	void DeletePhrases();
	void DeleteBullets();
	void DeletePlayer();
	void DeleteBackground();


	void Init();
	void Destroy();

	void SetWorldMaxX(int value);
	void SetWorldMinX(int value);

};





