#ifndef __PHYSCOLLISIONSOLVER_H_
#define __PHYSCOLLISIONSOLVER_H_

#include "../objects/object_physic.h"

void ProcessCollisions();

void SolveCollision(ObjPhysic* obj1, ObjPhysic* obj2);

#endif // __PHYSCOLLISIONSOLVER_H_