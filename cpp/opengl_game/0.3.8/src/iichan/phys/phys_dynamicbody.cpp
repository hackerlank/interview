#include "stdafx.h"
#include "phys_dynamicbody.h"
#include "phys_misc.h"

#include "../../misc.h"
#include "../../config.h"

#include "../iichan_item.h"
#include "../iichan_character.h"
#include "../iichan_bullet.h"

extern config cfg;

#ifdef USING_SAP_PHYSICS
//#include "phys/sap/SweepAndPrune.h"
#include "phys/sap/OPC_ArraySAP.h"
using namespace Opcode;
//#include "sap/OPC_ArraySAP.h"
extern ArraySAP* SAP;
#endif // USING_SAP_PHYSICS

Vector2 PhysDynamicBody::Process(vector<PhysBaseBody*> *bodies)
{
#ifdef KERNEL_BUG_PHYSICS
	vel = vel + acc + gravity;

	// Ограничения на скорость
	if (vel.x > max_x_vel)
		vel.x = max_x_vel;
	else if (vel.x < -max_x_vel)
		vel.x = -max_x_vel;

	if (vel.y > max_y_vel)
		vel.y = max_y_vel;
	else if (vel.y < -max_y_vel)
		vel.y = -max_y_vel;

	on_plane = false;

	Vector2 old_coord = aabb.p;
	aabb.p += vel;

	vector<PhysBaseBody*> collisions;			// Массив обьектов, от которых потом отталкиваться будем

	DetectCollision(bodies, &collisions);
	if (collisions.size() > 0)
	{
		vector<AABB*> groups;						// Массив груп прямоугольников пересечений. По групам строится вектор вывода из столкновений
		CreateGroups(&collisions, &groups);
		ProcessCollision(&groups);
	}

	return aabb.p - old_coord;
#endif // KERNEL_BUG_PHYSICS

#ifdef SATANA_PHYSICS
	// Если первый раз, то инициализируем стейты положений относительно остальных объектов.
	if (first_time_here)
	{
		first_time_here = false;
		InitCollisionHints(bodies);
	}
	Vector2 old_coord = aabb.p;
	// Проевряем, есть ли плоскость под обхектом.
	CheckUnderlyngPlane(bodies);
	// Гравитируем только если объект не на плоскоксти
	if (!on_plane)
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
	// Пагнали
	Collisions(bodies);
	aabb.p += vel;

	// Вот это - скольжение и всё такое.
	if (on_plane)
		vel.x *= 0.75f;
	else
		vel.x *= 0.9999f;
	return aabb.p - old_coord;
#endif // SATANA_PHYSICS

#ifdef USING_SAP_PHYSICS
	old_aabb = aabb;
	// Проевряем, есть ли плоскость под обхектом.
	//CheckUnderlyngPlane();
	CheckUnderlyngPlane(bodies);
	// Гравитируем только если объект не на плоскоксти
	if (!on_plane)
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

	// Вот это - скольжение и всё такое.
	if (on_plane)
		vel.x *= 0.75f;
	else
		vel.x *= 0.9999f;


	return aabb.p - old_aabb.p;
#endif // USING_SAP_PHYSICS
}

#ifdef KERNEL_BUG_PHYSICS


void PhysDynamicBody::DetectCollision(vector<PhysBaseBody*> *bodies, vector<PhysBaseBody*> *collisions)
{	
	float x = 0, y = 0;


	for(vector<PhysBaseBody*>::iterator it = bodies->begin();
		it != bodies->end();
		it++)
	{
		PhysBaseBody *body = *it;

		// Мы не путаемся в своих ногах
		if (body == this)
			continue;

		// Ха, выражение работает. Не зря я сохранил конспект по дискретке (это самое короткое, которое я смог получить)
		if ( (bullet || !body->GetCollidable()) && (!bullet || !body->GetBulletCollidable()) )
			continue;

		// Вы не можете выстрелить себе в ногу
		if (bullet)
			if (((BULLET*)item)->GetShooter()->GetPhysBody() == body)
				continue;

		AABB i;
		i.Intersect(this->aabb, body->GetAABB());
		if (!i.IsEmpty())
		{
			if (bullet)
				//ProcessBullet(body);
				((BULLET*)item)->BulletHit(body);
			else
				collisions->push_back(body);



		}

	}	
}

void PhysDynamicBody::CreateGroups(vector<PhysBaseBody*> *collisions, vector<AABB*> *groups)
{
	bool newgroup = true; 
	for(vector<PhysBaseBody*>::iterator cit = collisions->begin();
		cit != collisions->end();
		cit++)
	{
		PhysBaseBody *col = *cit;

		for(vector<AABB*>::iterator git = groups->begin();
			git != groups->end();
			git++)
		{
			AABB* group = *git;

			if (group->p.x == col->GetAABB().p.x)
			{
				// Группа-столбец
				scalar old_h = group->H;
				group->H = (fabs(group->p.y - col->GetAABB().p.y) + group->H + col->GetHeight()) / 2;
				if (group->p.y > col->GetAABB().p.y)
				{
					group->p.y = col->GetAABB().p.y - col->GetHeight() + group->H;
				}
				else
				{
					group->p.y = group->p.y - old_h + group->H;
				}

				break;
			}
			else if (group->p.y == col->GetAABB().p.y)
			{
				scalar old_w = group->W;
				group->W = (fabs(group->p.x - col->GetAABB().p.x) + group->W + col->GetWidth()) / 2;
				if (group->p.x > col->GetAABB().p.x)
				{
					group->p.x = col->GetAABB().p.x - col->GetWidth() + group->W;
				}
				else
				{
					group->p.x = group->p.x - old_w + group->W;
				}
				break;
			}
			else
				newgroup = true;
		}

		if (newgroup)
		{
			AABB *ng = new AABB(col->GetAABB());
			groups->push_back(ng);
			newgroup = false;
		}
	}
}

void PhysDynamicBody::ProcessCollision(vector <AABB*> *groups)
{
	Vector2 d, p;

	for(vector<AABB*>::iterator git = groups->begin();
		git != groups->end();
		git++)
	{
		AABB* group = *git;
		d = aabb.p - group->p;					// Расстояние между центрами
		// Величина взаимопроникновения
		p.x = (aabb.W + group->W) - fabs(d.x);
		p.y = (aabb.H + group->H) - fabs(d.y);

		if (p.x > 0 && p.y > 0)
		{
			if (p.x < p.y)
			{
				// По X
				if (d.x < 0)
				{
					// Влево
					p.x = -p.x;
				}
				p.y = 0;
			}
			else
			{
				// По Y
				if (d.y < 0)
				{
					// Вверх
					p.y = -p.y;
					on_plane = true;
				}
				p.x = 0;
			}

			// TODO: Тут наверное общий вектор строить надо, а не для каждой группы по отдельности
			scalar depth = p.Normalize();
			ResolveCollision(p, depth);
		}
	}
}

void PhysDynamicBody::ResolveCollision(Vector2 n, float depth)
{
	Vector2 nV, t;
	Vector2 v = vel;
	float dp = v * n;					// Проекция скорости на нормаль
	nV = dp * n;							// Нормальная скорость
	t = v - nV;							// Тангенциальная скорость

	Vector2 f;			// Влияние трения
	Vector2 b;			// Отскок
	if (dp < 0)
	{		
		// TODO: Поменять констаты на свойства физического тела
		//		f = t * 0;			
		b = nV * 1;
	}

	//	v -= f + b;
	v -= b;
	vel = (v);

	aabb.p += n*depth;
}


#endif // KERNEL_BUG_PHYSICS



PhysDynamicBody::PhysDynamicBody()
{
	SetType(bodyDynamic);
	mass = (0.0);
	//SetWidth(0);
	//SetHeight(0);
	SetCollidable(false);
	SetBulletCollidable(false);
	max_x_vel = (0.0);
	max_y_vel = (0.0);
	on_plane = (false);
	gravity = (Vector2(0, /*cfg.gravity - вот нахуй гравитация в кфг? Хотя...*/ 0.6f));
	SetMirrored(false);
	bullet = false;
	item = NULL;

#ifdef SATANA_PHYSICS
	first_time_here = true;
#endif // SATANA_PHYSICS
}


#ifdef SATANA_PHYSICS

void PhysDynamicBody::Collisions(vector<PhysBaseBody*> *bodies)
{
	float x = 0, y = 0;
	bool collided = false;
	AABB tmp(this->aabb);
	aabb.p += vel;

	for(vector<PhysBaseBody*>::iterator it = bodies->begin();
		it != bodies->end();
		it++)
	{
		PhysBaseBody *body = *it;

		if (body == this)
			continue;
		// Ха, выражение работает. Не зря он сохранил конспект по дискретке (это самое короткое, которое он смог получить)
		if ( (bullet || !body->GetCollidable()) && (!bullet || !body->GetBulletCollidable()) )
			continue;
		// Вы не можете выстрелить себе в ногу
		if (bullet)
			if (((BULLET*)item)->GetShooter()->GetPhysBody() == body)
				continue;


		// Вырви глаз...
		// Считаем статы боксов тел по проекциям.
		SegStateX[body->id] = SegProjIntersect(Vector2(tmp.p.x - tmp.W, 
			tmp.p.x + tmp.W),
			Vector2(body->GetAABB().p.x - body->GetAABB().W,
			body->GetAABB().p.x + body->GetAABB().W));


		SegStateY[body->id] = SegProjIntersect(Vector2(tmp.p.y - tmp.H, 
			tmp.p.y + tmp.H),
			Vector2(body->GetAABB().p.y - body->GetAABB().H,
			body->GetAABB().p.y + body->GetAABB().H));

		if (!bullet)
		{
			// Если мы пересекаемся ещё до применения скорости, то....
			if (SegStateX[body->id] == SEG_PROJ_STATE_INTERSECT && SegStateY[body->id] == SEG_PROJ_STATE_INTERSECT)
			{
				// ...используем данные с прошлого шага
				// А вдруг в угол нах на прошлом шаге?!+
				if ((oSx[body->id] != SEG_PROJ_STATE_INTERSECT) && (oSy[body->id] != SEG_PROJ_STATE_INTERSECT))
					CornerProblemSolve(oSx[body->id], oSx[body->id], tmp, body->aabb, vel);

				if (!(oSx[body->id] == SEG_PROJ_STATE_INTERSECT && oSy[body->id] == SEG_PROJ_STATE_INTERSECT))
					SatanaResolveCollision(oSx[body->id], oSy[body->id], body->aabb);
				else
					// Но если и там бида, то резолвим по мин дистанции
					SatanaResolveCollisionMTD(body->aabb);
				// хз зачем мы это опять присваиваем...
				tmp = AABB(aabb);
				//oSx = SegStateX;
				//oSy = SegStateY;
				continue;
			}
		}

		if (Collide(this->aabb, body->GetAABB()))
		{
			collided = true;
			if (bullet)
			{
				((BULLET*)item)->BulletHit(body);
			}
			else
			{
				//Собственно если угол, то мы его решаем, и потом вызывем резолв колллижен.
				if ((SegStateX[body->id] != SEG_PROJ_STATE_INTERSECT) && (SegStateY[body->id] != SEG_PROJ_STATE_INTERSECT))
					CornerProblemSolve(SegStateX[body->id], SegStateY[body->id], tmp, body->aabb, vel);

				SatanaResolveCollision(SegStateX[body->id], SegStateY[body->id], body->GetAABB());
				// хз зачем мы это опять присваиваем...
				tmp = AABB(aabb);
			}

		}

		// Не забывать запомнить, что было на этом ходу :)
		oSx[body->id] = SegStateX[body->id];
		oSy[body->id] = SegStateY[body->id];

	}	
	if (!collided)
	{
		//Если мы ни с чем не с толкнулись то..
		aabb.p -= vel;
		// ... но зачем я опять забыл
		//vel = Vector2(0.0f, 0.0f);
	}


}


// Там внизу всё очевидно
void PhysDynamicBody::SatanaResolveCollision( int sX, int sY, const AABB &bb )
{
	//Vector2 n = ResolveVector(sX, sY);
	if (sX == SEG_PROJ_STATE_INTERSECT)
	{
		if (sY == SEG_PROJ_STATE_BEFORE)
		{
			vel.y = 0.0f;
			aabb.p.y = bb.p.y - bb.H - aabb.H - 1;
			on_plane = true;
		}
		if (sY == SEG_PROJ_STATE_AFTER)
		{
			vel.y = 0.0f;
			aabb.p.y = bb.p.y + bb.H + aabb.H + 1;
			on_plane = false;
		}
	}

	if (sY == SEG_PROJ_STATE_INTERSECT)
	{
		if (sX == SEG_PROJ_STATE_BEFORE)
		{
			vel.x = 0.0f;
			aabb.p.x = bb.p.x - bb.W - aabb.W - 1;
		}
		if (sX == SEG_PROJ_STATE_AFTER)
		{
			vel.x = 0.0f;
			aabb.p.x = bb.p.x + bb.W + aabb.W + 1;
		}
	}

	if (sY == SEG_PROJ_STATE_BEFORE)
	{
		if (sX == SEG_PROJ_STATE_BEFORE )
		{

			vel.x = 0.0f;
			vel.y = 0.0f;
			aabb.p.x = bb.p.x - bb.W - aabb.W - 1;
			aabb.p.y = bb.p.y - bb.H - aabb.H - 1;
		}
		if (sX == SEG_PROJ_STATE_AFTER )
		{

			vel.x = 0.0f;
			vel.y = 0.0f;
			aabb.p.x = bb.p.x + bb.W + aabb.W + 1;
			aabb.p.y = bb.p.y - bb.H - aabb.H - 1;
		}
	}

	if (sY == SEG_PROJ_STATE_AFTER)
	{
		if (sX == SEG_PROJ_STATE_BEFORE )
		{

			vel.y = 0.0f;
			aabb.p.y = bb.p.y + bb.H + aabb.H + 1;

		}
		if (sX == SEG_PROJ_STATE_AFTER )
		{

			vel.y = 0.0f;
			aabb.p.y = bb.p.y + bb.H + aabb.H + 1;

		}
	}

	//else
	{
		//	on_plane = true;
	}
}



// См что это выше. Сильно выше, там где вызов.
void PhysDynamicBody::InitCollisionHints(vector<PhysBaseBody*> *bodies)
{
	AABB tmp(this->aabb);

	for(vector<PhysBaseBody*>::iterator it = bodies->begin();
		it != bodies->end();
		it++)
	{
		PhysBaseBody *body = *it;

		if (body == this)
			continue;
		// Ха, выражение работает. Не зря он сохранил конспект по дискретке (это самое короткое, которое он смог получить)
		if ( (bullet || !body->GetCollidable()) && (!bullet || !body->GetBulletCollidable()) )
			continue;
		// Вы не можете выстрелить себе в ногу
		if (bullet)
			if (((BULLET*)item)->GetShooter()->GetPhysBody() == body)
				continue;


		// Вырви глаз...
		SegStateX[body->id] = SegProjIntersect(Vector2(tmp.p.x - tmp.W, 
			tmp.p.x + tmp.W),
			Vector2(body->GetAABB().p.x - body->GetAABB().W,
			body->GetAABB().p.x + body->GetAABB().W));


		SegStateY[body->id] = SegProjIntersect(Vector2(tmp.p.y - tmp.H, 
			tmp.p.y + tmp.H),
			Vector2(body->GetAABB().p.y - body->GetAABB().H,
			body->GetAABB().p.y + body->GetAABB().H));

		oSx[body->id] = SegStateX[body->id];
		oSy[body->id] = SegStateY[body->id];

	}	
}

// Резолв коллиежена по MTD MTD = Minimal Translation Distance
void PhysDynamicBody::SatanaResolveCollisionMTD(const AABB &bb)
{
	scalar deltaX, deltaY;
	SegIntersect(Vector2(aabb.p.x - aabb.W, aabb.p.x + aabb.W), Vector2(bb.p.x - bb.W, bb.p.x + bb.W), deltaX);
	SegIntersect(Vector2(aabb.p.y - aabb.H, aabb.p.y + aabb.H), Vector2(bb.p.y - bb.H, bb.p.y + bb.H), deltaY);
	if (deltaX < deltaY)
	{
		aabb.p.x += deltaX;
		vel.x = 0.0f;
	}
	else
	{
		aabb.p.y += deltaY;
		vel.y = 0.0f;
	}
}
#endif
// Проверка наличия плоскости "под"
void PhysDynamicBody::CheckUnderlyngPlane(vector<PhysBaseBody*> *bodies)
{
	if (!on_plane)
		return;
	AABB tmp(this->aabb);
	for(vector<PhysBaseBody*>::iterator it = bodies->begin();
		it != bodies->end();
		it++)
	{
		PhysBaseBody *body = *it;
		if (body == this)
			continue;
		if ( (bullet || !body->GetCollidable()) && (!bullet || !body->GetBulletCollidable()) )
			continue;
		if (bullet)
			if (((BULLET*)item)->GetShooter()->GetPhysBody() == body)
				continue;

		int SX = SegProjIntersect(Vector2(tmp.p.x - tmp.W, 
			tmp.p.x + tmp.W),
			Vector2(body->GetAABB().p.x - body->GetAABB().W,
			body->GetAABB().p.x + body->GetAABB().W));


		if (tmp.p.y + tmp.H + 1 == body->aabb.p.y - body->aabb.H)
		{
			if (SX == SEG_PROJ_STATE_INTERSECT)
			{
				on_plane = true;
				return;
			}
			continue;
		}
	}	
	on_plane = false;
}


#ifdef USING_SAP_PHYSICS

// Проверка наличия плоскости "под"
void PhysDynamicBody::CheckUnderlyngPlane()
{
	/*if (!on_plane)
		return;

	assert(SAP);
	const ASAP_Box* boxes = SAP->GetBoxes();
	const ASAP_EndPoint* epX = SAP->GetEndPoints(X_);
	const ASAP_EndPoint* epY = SAP->GetEndPoints(Y_);
	assert(boxes);
	assert(epX);
	assert(epY);

	const ASAP_EndPoint* current = epY + (boxes + this->sap_handle)->max[Y_];
	scalar limit = current->mValue + 1;

	while ((++current)->mValue <= limit)
	{
		if (!current->isMax)
		{
			if ( ((PhysBaseBody*)(boxes[current->owner].mObject))->GetCollidable() )
			{
				if (Intersect2D(boxes[this->sap_handle], boxes[current->owner], X_, Z_))
				{
					on_plane = true;
					return;
				}
			}

		}
	}
	on_plane = false;*/
	
}
#endif // USING_SAP_PHYSICS