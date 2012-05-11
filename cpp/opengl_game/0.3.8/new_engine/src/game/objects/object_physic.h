#ifndef __OBJECT_PHYSIC_H_
#define __OBJECT_PHYSIC_H_

#include "object.h"

class ObjPhysic : public GameObject
{
public:
	
	UINT sap_handle;

	BYTE physFlags;

	// Флаги
	// solid					1		Объект находится на поверхности
	// bullet_collidable		2		Объект является препятсвием для пуль
	// dynamic					4		Динамический, движущийся
	__INLINE BYTE IsSolid()				const	{ return physFlags & 1; }
	__INLINE BYTE IsBulletCollidable()	const	{ return physFlags & 2; }
	__INLINE BYTE IsDynamic()			const	{ return physFlags & 4; }

	__INLINE void SetSolid()				{ physFlags |= 1; }
	__INLINE void SetBulletCollidable()		{ physFlags |= 2; }
	__INLINE void SetDynamic()				{ physFlags |= 4; }

	__INLINE void ClearSolid()				{ physFlags &= ~1; }
	__INLINE void ClearBulletCollidable() 	{ physFlags &= ~2; }
	__INLINE void ClearDynamic()			{ physFlags &= ~4; }

	ObjPhysic()
	{
		sap_handle = 0;
		physFlags = 0;

		this->SetPhysic();
	}

	virtual void PhysProcess()
	{ }
	
};


#endif // __OBJECT_PHYSIC_H_