#include "StdAfx.h"

#include "../objects/object_dynamic.h"
#include "../objects/object_bullet.h"
#include "../objects/object_spatial_trigger.h"

#include "phys_collisionsolver.h"

//////////////////////////////////////////////////////////////////////////

extern Opcode::ArraySAP *asap;

//////////////////////////////////////////////////////////////////////////

void SolveDynamicVSStatic(ObjDynamic* objD, ObjPhysic* objS);
void SolveBullet(ObjBullet* bul, ObjPhysic* obj);

void ResolveCollisionMTD(ObjDynamic* dyn, const CAABB &bb);
void ResolveCollision( int sX, int sY, ObjDynamic* dyn, const CAABB &bb );

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Использовать можно. И даже скорость повышается. Вот только героиня сквозь
// стены ходит. Потому что так столкновения решаются только при их возникновении.
//#define USE_INCREMENTAL

#ifndef USE_INCREMENTAL
// Используется для пространственных триггеров при возникновении пары.
// Используется флаг on_first_collision_only у триггера.
// Лушче не объединять вместе с USE_INCREMENTAL в нынешнем виде, а то будут 
// двойные срабатывания.
#define USE_INCREMENTAL_FOR_TRIGGERS
#endif


#if defined(USE_INCREMENTAL) || defined(USE_INCREMENTAL_FOR_TRIGGERS)

#ifdef USE_INCREMENTAL
// Объекты, которые требуется обновить в SAP после решения их столкновений.
vector<ObjPhysic*> objects_to_update;
#endif // USE_INCREMENTAL

// Вызывается, когда возникает новая пара пересекающихся объектов.
void* OnCreatePair(const void* object1, const void* object2, void* user_data)
{
	ObjPhysic* obj1 = (ObjPhysic*)object1;
	ObjPhysic* obj2 = (ObjPhysic*)object2;

#ifdef USE_INCREMENTAL
	SolveCollision(obj1, obj2);

	objects_to_update.push_back(obj1);
	objects_to_update.push_back(obj2);
#endif // USE_INCREMENTAL

#ifdef USE_INCREMENTAL_FOR_TRIGGERS
	
	if (obj1->type == objSpatialTrigger)
	{
		if (((ObjSpatialTrigger*)obj1)->on_first_collision_only)
		{
			((ObjSpatialTrigger*)obj1)->Trigger(obj2);
			return NULL;	
		}
	}

	if (obj2->type == objSpatialTrigger)
	{
		if (((ObjSpatialTrigger*)obj2)->on_first_collision_only)
		{
			((ObjSpatialTrigger*)obj2)->Trigger(obj1);
			return NULL;	
		}
	}

#endif // USE_INCREMENTAL_FOR_TRIGGERS

	return NULL;
}

#endif // defined(USE_INCREMENTAL) || defined(USE_INCREMENTAL_FOR_TRIGGERS)

// Обработка столкновений
void ProcessCollisions()
{
#ifdef USE_INCREMENTAL

	asap->DumpPairs(OnCreatePair, NULL, NULL, NULL);

	for(vector<ObjPhysic*>::iterator it = objects_to_update.begin();
		it != objects_to_update.end();
		it++)
	{
		ObjPhysic* obj = *it;
		asap->UpdateObject(obj->sap_handle, obj->aabb.GetASAP_AABB() );
	}

	objects_to_update.clear();

#else

	Opcode::ASAP_Pair* pairs;

#ifdef USE_INCREMENTAL_FOR_TRIGGERS
	UINT pairsCount = asap->DumpPairs(OnCreatePair, NULL, NULL, &pairs);
#else
	UINT pairsCount = asap->DumpPairs(NULL, NULL, NULL, &pairs);
#endif // USE_INCREMENTAL_FOR_TRIGGERS
	
	if (pairsCount)
	{
		ObjPhysic** odArr = new ObjPhysic*[pairsCount * 2];
		for(UINT i = 0; i < pairsCount; i++)
		{
			odArr[i*2 + 0] = (ObjPhysic*)pairs[i].object0;
			odArr[i*2 + 1] = (ObjPhysic*)pairs[i].object1;
			// Решение конкретной пары столкновений
			SolveCollision(odArr[i*2 + 0], odArr[i*2 + 1]);
		}
		for(UINT i = 0, j = 0; i < pairsCount; i+=2)
		{
			j = i+1;
			// Обновление положения объектов после того, как их столкновение было решено
			asap->UpdateObject(odArr[i]->sap_handle, odArr[i]->aabb.GetASAP_AABB() );
			asap->UpdateObject(odArr[j]->sap_handle, odArr[j]->aabb.GetASAP_AABB() );
		}
		DELETEARRAY(odArr);
	}

#endif // USE_INCREMENTAL

}




void SolveCollision(ObjPhysic* obj1, ObjPhysic* obj2)
{
	assert(obj1);
	assert(obj2);



	if (obj1->IsDynamic() && obj2->IsDynamic())
	{
		if (((ObjDynamic*)obj1)->IsBullet())
			SolveBullet((ObjBullet*)obj1, obj2);

		if (((ObjDynamic*)obj1)->IsBullet())
			SolveBullet((ObjBullet*)obj2, obj1);
	}
	else if (obj1->IsDynamic() && !obj2->IsDynamic())
	{
		if (((ObjDynamic*)obj1)->IsBullet())
			SolveBullet((ObjBullet*)obj1, obj2);
		else
			SolveDynamicVSStatic((ObjDynamic*)obj1, obj2);
	}
	else if (!obj1->IsDynamic() && obj2->IsDynamic())
	{
		if (((ObjDynamic*)obj1)->IsBullet())
			SolveBullet((ObjBullet*)obj2, obj1);
		else
			SolveDynamicVSStatic((ObjDynamic*)obj2, obj1);
	}

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void SolveDynamicVSStatic(ObjDynamic* objD, ObjPhysic* objS)
{
	if (objS->type == objSpatialTrigger)
	{
#ifdef USE_INCREMENTAL_FOR_TRIGGERS
		if (!((ObjSpatialTrigger*)objS)->on_first_collision_only)
#endif // USE_INCREMENTAL_FOR_TRIGGERS		
		{
			((ObjSpatialTrigger*)objS)->Trigger(objD);
			return;	
		}
	}

	if (!objS->IsSolid())
		return;

	bool collided = false;
	CAABB tmp(objD->aabb);
	tmp.p -= objD->vel;
	scalar x = 0, y = 0;

	int SegStateX = SegProjIntersect(Vector2(tmp.p.x - tmp.W, tmp.p.x + tmp.W),
		Vector2(objS->aabb.p.x - objS->aabb.W, objS->aabb.p.x + objS->aabb.W));

	int SegStateY = SegProjIntersect(Vector2(tmp.p.y - tmp.H, tmp.p.y + tmp.H),
		Vector2(objS->aabb.p.y - objS->aabb.H, objS->aabb.p.y + objS->aabb.H));

	int oSx = SegProjIntersect(Vector2(objD->old_aabb.p.x - objD->old_aabb.W, objD->old_aabb.p.x + objD->old_aabb.W),
		Vector2(objS->aabb.p.x - objS->aabb.W, objS->aabb.p.x + objS->aabb.W));

	int oSy = SegProjIntersect(Vector2(objD->old_aabb.p.y - objD->old_aabb.H, objD->old_aabb.p.y + objD->old_aabb.H),
		Vector2(objS->aabb.p.y - objS->aabb.H, objS->aabb.p.y + objS->aabb.H));
	
	// Если мы пересекаемся ещё до применения скорости, то....
	if (SegStateX == SEG_PROJ_STATE_INTERSECT && SegStateY == SEG_PROJ_STATE_INTERSECT)
	{
		// ...используем данные с прошлого шага
		// А вдруг в угол нах на прошлом шаге?!+
		if ((oSx != SEG_PROJ_STATE_INTERSECT) && (oSy != SEG_PROJ_STATE_INTERSECT))
			CornerProblemSolve(oSx, oSx, tmp, objS->aabb, objD->vel);

		if (!(oSx == SEG_PROJ_STATE_INTERSECT && oSy == SEG_PROJ_STATE_INTERSECT))
			ResolveCollision(oSx, oSy, objD, objS->aabb);
		else
			// Но если и там бида, то резолвим по мин дистанции
			ResolveCollisionMTD(objD, objS->aabb);
		// хз зачем мы это опять присваиваем...
		tmp = CAABB(objD->aabb);
		//oSx = SegStateX;
		//oSy = SegStateY;
		return;
	}

	if (Collide(objD->aabb, objS->aabb))
	{
		collided = true;

		//Собственно если угол, то мы его решаем, и потом вызывем резолв колллижен.
		if ((SegStateX != SEG_PROJ_STATE_INTERSECT) && (SegStateY != SEG_PROJ_STATE_INTERSECT))
			CornerProblemSolve(SegStateX, SegStateY, tmp, objS->aabb, objD->vel);

		ResolveCollision(SegStateX, SegStateY, objD, objS->aabb);
		// хз зачем мы это опять присваиваем...
		//tmp = AABB(dyn->aabb);
	}
}

void SolveBullet(ObjBullet* bul, ObjPhysic* obj)
{
	bul->Hit(obj);
}

//////////////////////////////////////////////////////////////////////////

// Резолв коллиежена по MTD MTD = Minimal Translation Distance
// Код из функции void PhysDynamicBody::SatanaResolveCollisionMTD(const ASAP_AABB &bb)
void ResolveCollisionMTD(ObjDynamic* dyn, const CAABB &bb)
{
	scalar deltaX, deltaY;
	SegIntersect(	Vector2(dyn->aabb.p.x - dyn->aabb.W, dyn->aabb.p.x + dyn->aabb.W), 
		Vector2(bb.p.x - bb.W, bb.p.x + bb.W), deltaX	);
	SegIntersect(	Vector2(dyn->aabb.p.y - dyn->aabb.H, dyn->aabb.p.y + dyn->aabb.H), 
		Vector2(bb.p.y - bb.H, bb.p.y + bb.H), deltaY	);
	if (deltaX < deltaY)
	{
		dyn->aabb.p.x += deltaX;
		dyn->vel.x = 0.0f;
	}
	else
	{
		dyn->aabb.p.y += deltaY;
		dyn->vel.y = 0.0f;
	}
}

// Там внизу всё очевидно
void ResolveCollision( int sX, int sY, ObjDynamic* dyn, const CAABB &bb )
{
	if (sX == SEG_PROJ_STATE_INTERSECT)
	{
		if (sY == SEG_PROJ_STATE_BEFORE)
		{
			dyn->vel.y = 0.0f;
			dyn->aabb.p.y = bb.p.y - bb.H - dyn->aabb.H - 1;
			dyn->SetOnPlane();
		}
		if (sY == SEG_PROJ_STATE_AFTER)
		{
			dyn->vel.y = 0.0f;
			dyn->aabb.p.y = bb.p.y + bb.H + dyn->aabb.H + 1;
			dyn->ClearOnPlane();
		}
	}

	if (sY == SEG_PROJ_STATE_INTERSECT)
	{
		if (sX == SEG_PROJ_STATE_BEFORE)
		{
			dyn->vel.x = 0.0f;
			dyn->aabb.p.x = bb.p.x - bb.W - dyn->aabb.W - 1;
		}
		if (sX == SEG_PROJ_STATE_AFTER)
		{
			dyn->vel.x = 0.0f;
			dyn->aabb.p.x = bb.p.x + bb.W + dyn->aabb.W + 1;
		}
	}

	if (sY == SEG_PROJ_STATE_BEFORE)
	{
		if (sX == SEG_PROJ_STATE_BEFORE )
		{

			dyn->vel.x = 0.0f;
			dyn->vel.y = 0.0f;
			dyn->aabb.p.x = bb.p.x - bb.W - dyn->aabb.W - 1;
			dyn->aabb.p.y = bb.p.y - bb.H - dyn->aabb.H - 1;
		}
		if (sX == SEG_PROJ_STATE_AFTER )
		{

			dyn->vel.x = 0.0f;
			dyn->vel.y = 0.0f;
			dyn->aabb.p.x = bb.p.x + bb.W + dyn->aabb.W + 1;
			dyn->aabb.p.y = bb.p.y - bb.H - dyn->aabb.H - 1;
		}
	}

	if (sY == SEG_PROJ_STATE_AFTER)
	{
		if (sX == SEG_PROJ_STATE_BEFORE )
		{

			dyn->vel.y = 0.0f;
			dyn->aabb.p.y = bb.p.y + bb.H + dyn->aabb.H + 1;

		}
		if (sX == SEG_PROJ_STATE_AFTER )
		{

			dyn->vel.y = 0.0f;
			dyn->aabb.p.y = bb.p.y + bb.H + dyn->aabb.H + 1;

		}
	}

}