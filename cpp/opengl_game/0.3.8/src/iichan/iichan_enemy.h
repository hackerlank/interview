#ifndef __ENEMY_H_
#define __ENEMY_H_
#include "iichan_character.h"

// Типы врагов
enum ENEMY_TYPE {enemyNone, enemyFighter, enemyShooter};

// Сферический враг
class ENEMY : public CHARACTER
{
private:
	ENEMY_TYPE enemy_type;
	int attack_range;						// Расстояние, с которого враг начинает атаковать
	bool ai_enabled;						// Действует ли ИИ
	bool squashable;						// Можно ли раздавить
	bool attack_collide;					// Повреждается ли игрок, дотронувшись до врага
	int damage;								// Вред в ближнем бою

public:
	ENEMY(int health, ENEMY_TYPE et, bool squashable, bool attack_collide);

	virtual void Move();

	ENEMY_TYPE GetEnemyType() const { return enemy_type; }
	void SetEnemyType(ENEMY_TYPE new_value) { enemy_type = new_value; }

	int GetAttackRange() const { return attack_range; }
	void SetAttackRange(int new_value) { attack_range = new_value; }

	bool GetAIEnabled() const { return ai_enabled; }
	void SetAIEnabled(bool new_value) { ai_enabled = new_value; }

	bool GetSquashable() const { return squashable; }
	void SetSquashable(bool new_value) { squashable = new_value; }

	bool GetAttackCollide() const { return attack_collide; }
	void SetAttackCollide(bool new_value) { attack_collide = new_value; }

	void SetDamage(int new_value) { damage = new_value; }
	int GetDamage() const { return damage; }

};

#endif