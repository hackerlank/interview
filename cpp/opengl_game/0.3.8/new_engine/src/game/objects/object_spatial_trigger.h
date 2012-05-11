#ifndef __OBJECT_SPATIAL_TRIGGER_H_
#define __OBJECT_SPATIAL_TRIGGER_H_

#include "object_physic.h"

// ��������������� �������. �������� ������, ����� � ��� ������������ 
// ������ � ����������� id ���, ���� id �� ���������, �� ����� � ������
// ������������ ����.
// ������ ����� ���������� �� ������ ���� ��� ������ ��� �������������
// �����������.

class ObjSpatialTrigger : public ObjPhysic
{
public:

	ObjectType actors_type;			// ��� ��������, �� ������ ���������.
	UINT actor_id;					// id �������, �� ������� ���������.

	bool on_first_collision_only;	// ���� true, �� ������ ��������� ������ ��� ������������� �����������.

	int scriptOnTrigger;			//����� ������� � �������.

	ObjSpatialTrigger();
	virtual ~ObjSpatialTrigger();

	void Trigger(GameObject* obj);	// ���������� ��� ����������� �����-���� ��������.
	
};


ObjSpatialTrigger* CreateSpatialTrigger(CAABB aabb, ObjectType actors_type, UINT actor_id, int script, bool on_first_collision_only);

#endif // __OBJECT_SPATIAL_TRIGGER_H_