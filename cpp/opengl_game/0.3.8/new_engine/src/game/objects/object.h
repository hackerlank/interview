#ifndef __OBJECT_H_
#define __OBJECT_H_

enum ObjectType { objNone, objPlayer, objSprite, objBullet, objSpatialTrigger };

#include "../../script/script.h"

#include "../phys/phys_misc.h"
#include "../sprite.h"

#include "../proto.h"



class GameObject
{
public:
	UINT id;

	ObjectType type;

	GameObject* prev;		// ���������� � ��������� ������
	GameObject* next;		// ��������� � ��������� ������

	Sprite* sprite;			// ������

	CAABB aabb;				// �������������, ����������� ���������� ������; ���������� ����

	int scriptProcess;


	// �����
	// dead		1		������ �������� �������.
	// physic	2		����������, �������� � ASAP.
	// sleep	4
	BYTE objFlags;
	__INLINE BYTE IsDead()		{ return objFlags & 1; }
	__INLINE void SetDead()		{ objFlags |= 1; }
	__INLINE void ClearDead()	{ objFlags &= ~1; }

	__INLINE BYTE IsPhysic()		{ return objFlags & 2; }
	__INLINE void SetPhysic()		{ objFlags |= 2; }
	__INLINE void ClearPhysic()	{ objFlags &= ~2; }

	__INLINE BYTE IsSleep()		{ return objFlags & 4; }
	__INLINE void SetSleep()		{ objFlags |= 4; }
	__INLINE void ClearSleep()	{ objFlags &= ~4; }


	GameObject()
	{
		type = objNone;
		id = 0;
		prev = NULL;
		next = NULL;
		sprite = NULL;

		objFlags = 0;		
		scriptProcess = LUA_NOREF;
	}

	virtual ~GameObject();

	virtual void Process()
	{
	}

	

	void Draw();
};	

void AddObject(GameObject* obj);
void RemoveObject(GameObject* obj);
void RemoveAllObjects();

void BatchAdd();
void BatchRemove();

GameObject* GetGameObject(UINT id);

void ProcessAllObjects();
void DrawAllObjects();

BOOL LoadObjectFromProto(Proto* proto, GameObject* obj);
BOOL CheckAnimations(GameObject* obj, ObjectType ot);
void SetAnimation(GameObject* obj, string anim_name, bool looped, bool restart);


#endif // __OBJECT_H_
