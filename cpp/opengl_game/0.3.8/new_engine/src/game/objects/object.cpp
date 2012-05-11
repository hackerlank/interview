#include "StdAfx.h"

#include "../../render/draw.h"
#include "../../render/renderer.h"

#include "object.h"
#include "object_dynamic.h"
#include "../phys/phys_collisionsolver.h"

#include "../camera.h"

#include "../../misc.h"

#include "../../config.h"

//////////////////////////////////////////////////////////////////////////

extern float DBG_PRINT_VAL2;

extern Opcode::ArraySAP *asap;

extern config cfg;

//////////////////////////////////////////////////////////////////////////
// Транк всех объектов
GameObject* OBJECT_TRUNC		=	NULL;

// Текущий указатель в транке
GameObject* P_OBJECT_TRUNC	=	NULL;

UINT OBJECT_COUNT = 0;

UINT last_id = 1;

bool DRAW_DEBUG_RECTS = false;



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

vector<GameObject*> createdObjects;
void AddObject(GameObject* obj)
{
	createdObjects.push_back(obj);
	obj->id = last_id;
	last_id++;

	if (obj->IsPhysic())
	{
		((ObjPhysic*)obj)->sap_handle = asap->AddObject(obj, obj->id, obj->aabb.GetASAP_AABB());
	}
}

vector<GameObject*> removedObjects;
void RemoveObject(GameObject* obj)
{
	// Добавляем объект в список к удалению
	removedObjects.push_back(obj);
	if (obj->IsPhysic())
	{
		// Удаляем объект из ASAP
		asap->RemoveObject( ((ObjPhysic*)obj)->sap_handle );
	}
}

void BatchAdd()
{
	GameObject* new_obj = NULL;

	for(vector<GameObject*>::iterator it = createdObjects.begin();
		it != createdObjects.end();
		it++)
	{
		new_obj = *it;

		if(OBJECT_COUNT == 0)
		{
			// Если это первый объект
			// Указываем TRUNC на этот объект
			OBJECT_TRUNC = P_OBJECT_TRUNC = new_obj;
		}
		else
		{
			// Объект не первый, добавляем его в транк
			P_OBJECT_TRUNC->next = new_obj;
			new_obj->prev = P_OBJECT_TRUNC;
			P_OBJECT_TRUNC = P_OBJECT_TRUNC->next;
		}

		OBJECT_COUNT++;
		DBG_PRINT_VAL2++;

	}

	createdObjects.clear();
}

void RemObj(GameObject* obj)
{
	if (obj)
	{

		if (GetCameraAttachedObject() == obj)
		{
			// На этом объекте сфокусирована камера. Убираем это.
			CameraAttachToObject(NULL);
		}

		DELETESINGLE(obj);
	}
}

void BatchRemove()
{
	GameObject* obj = NULL;
	GameObject* prev = NULL;
	GameObject* next = NULL;

	for(vector<GameObject*>::iterator it = removedObjects.begin();
		it != removedObjects.end();
		it++)
	{
		obj = *it;
		prev = obj->prev;
		next = obj->next;

		if (obj == P_OBJECT_TRUNC) P_OBJECT_TRUNC = prev;
		if (obj == OBJECT_TRUNC) OBJECT_TRUNC = next;
		if (prev) prev->next = next;
		if (next) next->prev = prev;

		OBJECT_COUNT--;
		DBG_PRINT_VAL2--;

		RemObj(obj);
	}

	removedObjects.clear();
}

void RemoveAllObjects()
{
	for(vector<GameObject*>::iterator it = createdObjects.begin();
		it != createdObjects.end();
		it++)
	{
		RemObj(*it);
	}

	createdObjects.clear();
	removedObjects.clear();

	GameObject* obj = P_OBJECT_TRUNC;
	GameObject* prev = NULL;
	while (obj)
	{
		prev = obj->prev;
		RemObj(obj);
		obj = prev;
	}
	OBJECT_COUNT = 0;
	P_OBJECT_TRUNC = NULL;
	OBJECT_TRUNC = NULL;
}

//////////////////////////////////////////////////////////////////////////

GameObject* GetGameObject(UINT id)
{
	GameObject* obj = OBJECT_TRUNC;
	GameObject* next = NULL;
	while (obj)
	{
		next = obj->next;

		if (obj->id == id)
			return obj;

		obj = next;
	}

	for(vector<GameObject*>::iterator it = createdObjects.begin();
		it != createdObjects.end();
		it++)
	{
		obj = *it;
		if (obj->id == id)
			return obj;
	}
	
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL LoadObjectFromProto(Proto* proto, GameObject* obj)
{
	if (!proto || !obj)
		return TRUE;

	Sprite* s = new Sprite;
	//memset(s, '\0', sizeof(Sprite));
	s->tex = TextureByName(proto->texture);
	s->z = proto->z;
	s->imageHeight = proto->image_height;
	s->imageWidth = proto->image_width;
	s->frameWidth = proto->frame_widht;
	s->frameHeight = proto->frame_height;
	s->frameX = proto->frame_x;
	s->frameY = proto->frame_y;
	s->SetVisible();


	if (proto->animations.size() > 0)
	{
		s->anims = new AnimationMap();

		for(vector<AnimProto*>::iterator it = proto->animations.begin();
			it != proto->animations.end();
			it++)
		{
			AnimProto* ap = *it;
			if(ap->anim_name)
			{
				if (s->GetAnimation(ap->anim_name))
				{
					sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Повторно объявлена анимация %s. Пропускаем.", ap->anim_name);
					continue;
				}

				Animation* a = new Animation;
				memset(a, '\0', sizeof(Animation));

				a->animationSpeed = ap->anim_speed;
				a->realX = ap->real_x;
				a->realY = ap->real_y;
				a->realWidth = ap->real_width;
				a->realHeight  = ap->real_height;
				a->realMirrorX = ap->real_mirror_x;
				a->realMirrorY = ap->real_mirror_y;
				//a->flashX = ap->;
				//a->flashY = ap->;
				//a->flashMirrorX = ap->;
				//a->flashMirrorY = ap->;
				a->frameCount = ap->frame_count;
				a->startFrame = ap->start_frame;

				(*s->anims)[string(ap->anim_name)] = a;
			}
		}
	}

	obj->sprite = s;

	if (obj->IsPhysic())
	{
		ObjPhysic* obp = (ObjPhysic*)obj;
		if (proto->phys_solid) obp->SetSolid();
		if (proto->phys_bullet_collidable) obp->SetBulletCollidable();
		if (obp->IsDynamic())
		{
			ObjDynamic* obd = (ObjDynamic*)obp;
			obd->max_x_vel = proto->phys_max_x_vel;
			obd->max_y_vel = proto->phys_max_y_vel;
			obd->walk_acc = proto->phys_walk_acc;
			obd->jump_vel = proto->phys_jump_vel;

		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

GameObject::~GameObject()
{
	DELETESINGLE(sprite);

	if (this->scriptProcess)
		SCRIPT::RemoveFromRegistry(this->scriptProcess);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL CheckAnimations(GameObject* obj, ObjectType ot)
{
	return 0;
}

void SetAnimation(GameObject* obj, string anim_name, bool looped, bool restart)
{
	if (obj->sprite)
	{
		if (!restart && obj->sprite->cur_anim == anim_name)
			return;

		if (obj->sprite->anims)
		{
			Animation* a = obj->sprite->GetAnimation(anim_name);

			if (a)
			{
				obj->aabb.p.y += obj->aabb.H - a->realHeight * 0.5f;
				obj->aabb.H = a->realHeight * 0.5f;
				obj->aabb.W = a->realWidth * 0.5f;

				obj->sprite->cur_anim = anim_name;
				obj->sprite->currentFrame = a->startFrame;

				if (obj->IsPhysic())
				{
					// Мы вероятно поменяли положение объекта, так что теперь надо обновить его положение в SAP.
					asap->UpdateObject( ((ObjPhysic*)obj)->sap_handle, obj->aabb.GetASAP_AABB() );
				}
			}
			else
			{
				obj->aabb.H = obj->sprite->frameHeight * 0.5f;
				obj->aabb.W = obj->sprite->frameWidth * 0.5f;
				obj->sprite->cur_anim = "";
			}
		}
		else
		{
			obj->aabb.H = obj->sprite->frameHeight * 0.5f;
			obj->aabb.W = obj->sprite->frameWidth * 0.5f;
			obj->sprite->cur_anim = "";
		}
		obj->sprite->ClearAnimDone();

		if (looped)
			obj->sprite->SetLooped();
		else
			obj->sprite->ClearLooped();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ProcessAllObjects()
{
	GameObject* obj = OBJECT_TRUNC;
	GameObject* next = NULL;
	while (obj)
	{
		next = obj->next;

		if (obj->scriptProcess >= 0)
		{
			if (SCRIPT::ExecChunkFromReg(obj->scriptProcess))
			{
				// В скрипте произошла какая-то ошибка. Будем испльзовать стандартный в следующий раз.
				SCRIPT::RemoveFromRegistry(obj->scriptProcess);
				obj->scriptProcess = LUA_NOREF;
			}
		}
		else
		{
			obj->Process();
		}

		if (obj->IsPhysic())
		{
			((ObjPhysic*)obj)->PhysProcess();

		}

		if (obj->IsDead())
		{
			RemoveObject(obj);
		}

		//obj->Draw();

		obj = next;
	}

	// Обработка пар столкновений
	ProcessCollisions();

	BatchRemove();
	BatchAdd();		// Будут доступны на следующем ходу
}




//////////////////////////////////////////////////////////////////////////

void DrawAllObjects()
{
	GameObject* obj = OBJECT_TRUNC;
	GameObject* next = NULL;
	while (obj)
	{
		next = obj->next;
		obj->Draw();
		obj = next;
	}
}


void GameObject::Draw()
{
	if (!this->sprite)
		return;

	if (!this->sprite->IsVisible())
		return;

	Sprite* s = this->sprite;

	float x = this->aabb.p.x - aabb.W;
	float y = this->aabb.p.y - aabb.H;
	float x_ph = x;
	float y_ph = y;
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;

	if (s->anims && 
		s->anims->size())
	{
		Animation* a = s->GetAnimation(s->cur_anim);

		if (a)
		{
			if (s->IsMirrored())
			{
				//x -= a->realWidth - a->realX;///a->realMirrorX;
				x -= a->realMirrorX;
				y -= a->realMirrorY;
			}
			else
			{
				x -= a->realX;
				y -= a->realY;
			}

			s->ChangeFrame(a);
		}
		else
		{
			s->SetAnimDone();
		}

		UINT colCount = 1;
		if (s->frameWidth != 0)
		{
			colCount = s->imageWidth / s->frameWidth;
		}

		UINT col = s->currentFrame % colCount;
		UINT row = s->currentFrame / colCount;

		x1 = col * s->frameWidth;
		x2 = x1 + s->frameWidth;
		y1 = row * s->frameHeight;
		y2 = y1 + s->frameHeight;
	}
	else
	{
		x1 = s->frameX;
		x2 = x1 + s->frameWidth;
		y1 = s->frameY;
		y2 = y1 + s->frameHeight;
	}

	if (this->sprite->IsFixed())
	{
		extern float CAMERA_OFF_X;
		extern float CAMERA_OFF_Y;
		x -= CAMERA_OFF_X;
		y -= CAMERA_OFF_Y;
	}

	extern float CAMERA_LEFT;
	extern float CAMERA_RIGHT;
	extern float CAMERA_TOP;
	extern float CAMERA_BOTTOM;

	// Если спрайт находится за пределами обзора камеры, то мы его не рисуем.
	if (x + x2-x1 < CAMERA_LEFT || x > CAMERA_RIGHT)
		return;
	if (y + x2-x1 < CAMERA_TOP || y > CAMERA_BOTTOM )
		return;

	RenderSprite(x, y, s->z, x1, y1, x2, y2, s->tex, s->IsMirrored());

	if (cfg.debug)
	{
		RenderBox(x, y, s->z, x2-x1, y2-y1);
		RenderBox(x_ph, y_ph, s->z, 2 * aabb.W, 2* aabb.H);
	}

}