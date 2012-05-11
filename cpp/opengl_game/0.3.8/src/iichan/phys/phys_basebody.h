#ifndef __PHYSBASEBODY_H_
#define __PHYSBASEBODY_H_

#include <vector>
#include "phys_misc.h"



enum BodyType {bodyStatic, bodyDynamic};

class PhysBaseBody
{
protected:

	BodyType type;
	

	bool collidable;
	bool bullet_collidable;

	bool mirrored;
public:

	

	int id;
	AABB aabb; // Положил в паблик. Сатана.

#ifdef USING_SAP_PHYSICS
	AABB old_aabb;
	UINT sap_handle;
#endif // USING_SAP_PHYSICS

	virtual Vector2 Process(vector<PhysBaseBody*> *bodies) { return Vector2::Blank(); }

	void SetId (int new_id) { id = new_id; }
	int GetId() const { return id; }

	void SetType(BodyType new_type) { type = new_type; }
	BodyType GetType() const { return type; }

	void SetWidth(scalar new_width) 
	{
		if (aabb.H != new_width)
		{
			aabb.p.x += aabb.W - new_width;
			aabb.W = new_width; 
		}
	}
	scalar GetWidth() const { return aabb.W; }

	void SetHeight(scalar new_height) 
	{
		if (aabb.H != new_height)
		{
			aabb.p.y += aabb.H - new_height;
			aabb.H = new_height;
		}
	}
	scalar GetHeight() const { return aabb.H; }

	void SetCoord(Vector2 new_coord) { aabb.p = new_coord; }
	Vector2 GetCoord() const { return aabb.p; }

	void SetCollidable(bool new_val) { collidable = new_val; }
	bool GetCollidable() const { return collidable; }

	void SetBulletCollidable(bool new_val) { bullet_collidable = new_val; }
	bool GetBulletCollidable() const { return bullet_collidable; }

	void SetMirrored(bool new_val) { mirrored = new_val; }
	bool GetMirrored() const { return mirrored; }


	void SetAABB(scalar x, scalar y, float w, float h)
	{
		aabb.H = h*0.5f;
		aabb.W = w*0.5f;
		aabb.p.x = x + aabb.W;
		aabb.p.y = y + aabb.H;
	}

	//void SetAABB(AABB new_aabb) { this->aabb = new_aabb; }

	AABB GetAABB() const { return aabb; }
};


#endif