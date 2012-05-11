#ifndef __OBJECT_DYNAMIC_H_
#define __OBJECT_DYNAMIC_H_

#include "object_physic.h"

enum ObjectMovementType { omtIdling, omtWalking, omtSitting, omtJumping, omtLanding/*, omtAimingUp, omtAimingDown, omtWalkingAimingUp*/ };
enum ObjectActivityType { oatIdling, oatShooting, oatDying, oatInjuring };



class ObjDynamic abstract : public ObjPhysic
{
public:
	CAABB old_aabb;
	Vector2 vel;
	Vector2 acc;
	Vector2 gravity;
	scalar max_x_vel;
	scalar max_y_vel;

	scalar walk_acc;
	scalar jump_vel;


	bool movementDirectionX;		// Напавление движения объекта
	

	ObjectMovementType movement;		// Тип движения объекта
	ObjectActivityType activity;		// Тип действий объекта


	// Флаги
	// on_plane		16		Объект находится на поверхности
	// bullet		32		Объект - пуля
	__INLINE BYTE IsOnPlane()		{ return physFlags & 16; }
	__INLINE BYTE IsBullet()		{ return physFlags & 32; }

	__INLINE void SetOnPlane()		{ physFlags |= 16; }
	__INLINE void SetBullet()		{ physFlags |= 32; }

	__INLINE void ClearOnPlane()	{ physFlags &= ~16; }
	__INLINE void ClearBullet()		{ physFlags &= ~32; }

	ObjDynamic()
	{
		this->SetDynamic();

		max_x_vel = 0.0f;
		max_y_vel = 0.0f;

		walk_acc = 0.0f;
		jump_vel = 0.0f;

		movementDirectionX = true;

		movement = omtIdling;
		activity = oatIdling;
	}


	void PhysProcess();
	
	void CheckUnderlyngPlane();
};


#endif // __OBJECT_DYNAMIC_H_
