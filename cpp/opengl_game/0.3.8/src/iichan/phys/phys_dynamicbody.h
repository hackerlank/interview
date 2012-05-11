#ifndef __PHYSDYNAMICBODY_H_
#define __PHYSDYNAMICBODY_H_

#include <vector>
#include "phys_basebody.h"
#include "phys_misc.h"

#include "../iichan_item.h"

class PhysDynamicBody : public PhysBaseBody
{
private:

#ifdef KERNEL_BUG_PHYSICS
	void DetectCollision(vector<PhysBaseBody*> *bodies, vector<PhysBaseBody*> *collisions);
	void CreateGroups(vector<PhysBaseBody*> *collisions, vector <AABB*> *groups);
	void ProcessCollision(vector <AABB*> *groups);
	void ResolveCollision(Vector2 n, float depth);
	bool ProcessBullet(PhysBaseBody* body);
#endif // KERNEL_BUG_PHYSICS

#ifdef SATANA_PHYSICS
	void Collisions(vector<PhysBaseBody*> *bodies);
	void SatanaResolveCollision(int sX, int sY, const AABB &bb);
	void SatanaResolveCollisionMTD(const AABB &bb);
	void InitCollisionHints(vector<PhysBaseBody*> *bodies);
	#endif // SATANA_PHYSICS
	void CheckUnderlyngPlane(vector<PhysBaseBody*> *bodies);



#ifdef USING_SAP_PHYSICS
	void CheckUnderlyngPlane();
#endif // USING_SAP_PHYSICS

public:
	ITEM* item;

#ifdef SATANA_PHYSICS

	// В массивах что ниже мы храним типы пересечения данного объекта со всеми остальными объектами
	// SegState - на текущем шаге, до применения скорости, oSx - на предыдущем шаге.
	// Да, знаю, пока криво, что массивы по 512 итд
	int SegStateX[512];	
	int SegStateY[512];
	int oSx[512];
	int oSy[512];

	bool first_time_here;
#endif // SATANA_PHYSICS

	float mass;

	Vector2 vel;
	Vector2 acc;

	float max_x_vel;
	float max_y_vel;

	Vector2 jump_speed;
	Vector2 walk_acc;

	bool on_plane;

	Vector2 gravity;

	bool bullet;


	Vector2 Process(vector<PhysBaseBody*> *bodies);

	PhysDynamicBody();
	~PhysDynamicBody();

	void AddVelocity(Vector2 new_vel) { vel += new_vel; }
	//void AddAcceleration(Vector2 new_acc) { acc += new_acc; }

};

#endif