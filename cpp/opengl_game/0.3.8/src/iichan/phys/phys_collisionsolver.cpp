#include "StdAfx.h"

#include "phys_basebody.h"
#include "phys_staticbody.h"
#include "phys_dynamicbody.h"
#include "phys_misc.h"

//#include "phys_sap.h"

#include "phys_collisionsolver.h"
#include "../iichan_bullet.h"

#ifdef USING_SAP_PHYSICS


void SolveBullet(const PhysDynamicBody* dyn, const PhysBaseBody* body);
void SolveDynamicVSStatic(PhysDynamicBody* dyn, const PhysStaticBody* stat);

void ResolveCollisionMTD(PhysDynamicBody* dyn, const AABB &bb);
void ResolveCollision( int sX, int sY, PhysDynamicBody* dyn, const AABB &bb );

void SolveCollision(const PhysBaseBody* body1, const PhysBaseBody* body2)
{
	assert(body1);
	assert(body2);

	if (body1->GetType() == bodyDynamic)
	{
		if ( ((PhysDynamicBody*)body1)->bullet )
			SolveBullet((PhysDynamicBody*)body1, body2);
		else if (body2->GetType() == bodyStatic)
			SolveDynamicVSStatic((PhysDynamicBody*)body1, (PhysStaticBody*)body2);
	}	
	else if (body2->GetType() == bodyDynamic)
	{
		if ( ((PhysDynamicBody*)body2)->bullet )
			SolveBullet((PhysDynamicBody*)body2, body1);
		else if (body1->GetType() == bodyStatic)
			SolveDynamicVSStatic((PhysDynamicBody*)body2, (PhysStaticBody*)body1);
	}
}

void SolveDynamicVSStatic(PhysDynamicBody* dyn, const PhysStaticBody* stat)
{
	if (!stat->GetCollidable())
		return;

	bool collided = false;
	AABB tmp(dyn->aabb);
	tmp.p -= dyn->vel;
	scalar x = 0, y = 0;

	int SegStateX = SegProjIntersect(Vector2(tmp.p.x - tmp.W, tmp.p.x + tmp.W),
		Vector2(stat->aabb.p.x - stat->aabb.W, stat->aabb.p.x + stat->aabb.W));

	int SegStateY = SegProjIntersect(Vector2(tmp.p.y - tmp.H, tmp.p.y + tmp.H),
		Vector2(stat->aabb.p.y - stat->aabb.H, stat->aabb.p.y + stat->aabb.H));

	int oSx = SegProjIntersect(Vector2(dyn->old_aabb.p.x - dyn->old_aabb.W, dyn->old_aabb.p.x + dyn->old_aabb.W),
		Vector2(stat->aabb.p.x - stat->aabb.W, stat->aabb.p.x + stat->aabb.W));

	int oSy = SegProjIntersect(Vector2(dyn->old_aabb.p.y - dyn->old_aabb.H, dyn->old_aabb.p.y + dyn->old_aabb.H),
		Vector2(stat->aabb.p.y - stat->aabb.H, stat->aabb.p.y + stat->aabb.H));

	// Если мы пересекаемся ещё до применения скорости, то....
	if (SegStateX == SEG_PROJ_STATE_INTERSECT && SegStateY == SEG_PROJ_STATE_INTERSECT)
	{
		// ...используем данные с прошлого шага
		// А вдруг в угол нах на прошлом шаге?!+
		if ((oSx != SEG_PROJ_STATE_INTERSECT) && (oSy != SEG_PROJ_STATE_INTERSECT))
			CornerProblemSolve(oSx, oSx, tmp, stat->aabb, dyn->vel);

		if (!(oSx == SEG_PROJ_STATE_INTERSECT && oSy == SEG_PROJ_STATE_INTERSECT))
			ResolveCollision(oSx, oSy, dyn, stat->aabb);
		else
			// Но если и там бида, то резолвим по мин дистанции
			ResolveCollisionMTD(dyn, stat->aabb);
		// хз зачем мы это опять присваиваем...
		tmp = AABB(dyn->aabb);
		//oSx = SegStateX;
		//oSy = SegStateY;
		return;
	}

	if (Collide(dyn->aabb, stat->aabb))
	{
		collided = true;

			//Собственно если угол, то мы его решаем, и потом вызывем резолв колллижен.
			if ((SegStateX != SEG_PROJ_STATE_INTERSECT) && (SegStateY != SEG_PROJ_STATE_INTERSECT))
				CornerProblemSolve(SegStateX, SegStateY, tmp, stat->aabb, dyn->vel);

			ResolveCollision(SegStateX, SegStateY, dyn, stat->GetAABB());
			// хз зачем мы это опять присваиваем...
			tmp = AABB(dyn->aabb);
	}

/*	if (!collided)
	{
		//Если мы ни с чем не с толкнулись то..
		aabb.p -= vel;
		// ... но зачем я опять забыл
		//vel = Vector2(0.0f, 0.0f);
	}
*/
}

void SolveBullet(const PhysDynamicBody* dyn, const PhysBaseBody* body)
{
	((BULLET*)dyn->item)->BulletHit(body);
}


// Резолв коллиежена по MTD MTD = Minimal Translation Distance
// Код из функции void PhysDynamicBody::SatanaResolveCollisionMTD(const ASAP_AABB &bb)
void ResolveCollisionMTD(PhysDynamicBody* dyn, const AABB &bb)
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
// Код из функции void PhysDynamicBody::SatanaResolveCollision( int sX, int sY, const ASAP_AABB &bb )
void ResolveCollision( int sX, int sY, PhysDynamicBody* dyn, const AABB &bb )
{
	if (sX == SEG_PROJ_STATE_INTERSECT)
	{
		if (sY == SEG_PROJ_STATE_BEFORE)
		{
			dyn->vel.y = 0.0f;
			dyn->aabb.p.y = bb.p.y - bb.H - dyn->aabb.H - 1;
			dyn->on_plane = true;
		}
		if (sY == SEG_PROJ_STATE_AFTER)
		{
			dyn->vel.y = 0.0f;
			dyn->aabb.p.y = bb.p.y + bb.H + dyn->aabb.H + 1;
			dyn->on_plane = false;
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

#endif // USING_SAP_PHYSICS