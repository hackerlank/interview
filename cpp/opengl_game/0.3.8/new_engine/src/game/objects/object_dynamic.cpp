#include "StdAfx.h"

// #include "../phys/phys_misc.h"

#include "object_dynamic.h"

#include "../phys/sap/OPC_ArraySAP.h"

extern Opcode::ArraySAP *asap;

void ObjDynamic::PhysProcess()
{
	if (IsSleep())
		return;

	old_aabb = aabb;

	// Проевряем, есть ли плоскость под обхектом.
	CheckUnderlyngPlane();

	// Гравитируем только если объект не на плоскоксти
	if (!IsOnPlane())
		vel += acc + gravity;
	else
		vel += acc;

	if (vel.x > max_x_vel)
		vel.x = max_x_vel;
	else if (vel.x < -max_x_vel)
		vel.x = -max_x_vel;

	if (vel.y > max_y_vel)
		vel.y = max_y_vel;
	else if (vel.y < -max_y_vel)
		vel.y = -max_y_vel;

	aabb.p += vel;

	if (IsOnPlane())
		vel.x *= 0.75f;
	else
		vel.x *= 0.9999f;


	asap->UpdateObject(sap_handle, aabb.GetASAP_AABB());
}

// Проверка наличия плоскости "под"
using namespace Opcode;

void ObjDynamic::CheckUnderlyngPlane()
{
	if (!this->IsOnPlane())
		return;

	assert(asap);

	const ASAP_Box* boxes = asap->GetBoxes();
	const ASAP_EndPoint* epX = asap->GetEndPoints(X_);
	const ASAP_EndPoint* epY = asap->GetEndPoints(Y_);
	assert(boxes);
	assert(epX);
	assert(epY);

	const ASAP_Box* object = boxes + this->sap_handle;

	const Opcode::ASAP_EndPoint* current = epY + (boxes + this->sap_handle)->mMax[Y_];
	const ValType limit = current->mValue + 1;
	
	while ((++current)->mValue <= limit)
	{
		if (!current->IsMax())
		{
			if ( ((ObjDynamic*)(boxes[current->GetOwner()].mObject))->IsSolid() )
			{
				const ASAP_Box* id1 = boxes + current->GetOwner();
				// Our max passed a min => start overlap
				if (object != id1 &&
					Intersect2D(*object, *id1, X_, Z_) &&
					Intersect1D_Min(this->aabb.GetASAP_AABB(), *id1, epY, Y_) )
				{
					this->SetOnPlane();
					return;
				}
			}

		}
	}
	this->ClearOnPlane();

}