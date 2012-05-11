#include "stdafx.h"
#include "iichan_enemy.h"

ENEMY::ENEMY(int health, ENEMY_TYPE et, bool squashable, bool attack_collide)
{
	__super::CHARACTER();
	SetHealth(health);
	SetType(itemEnemy);
	SetEnemyType(et);
	SetAIEnabled(true);
	SetSquashable(squashable);
	SetAttackCollide(attack_collide);
}

void ENEMY::Move()
{
	__super::Move();
}