#ifndef __PHYSSTATICBODY_H_
#define __PHYSSTATICBODY_H_

#include "phys_basebody.h"
#include "phys_misc.h"

class PhysStaticBody : public PhysBaseBody
{
private:

public:
	PhysStaticBody() 
	{ 
		SetType(bodyStatic);
		//SetWidth(0);
		//SetHeight(0);
		SetCollidable(false);
		SetBulletCollidable(false);
		SetMirrored(false);
		//SetCoord(Vector2::Blank());
	}
};

#endif