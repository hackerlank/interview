#ifndef __OBJECT_SPATIAL_TRIGGER_H_
#define __OBJECT_SPATIAL_TRIGGER_H_

#include "object_physic.h"

// Пространственый триггер. Вызывает скрипт, когда с ним пересекается 
// объект с определеным id или, если id не определен, то тогда с объект
// определеного типа.
// Скрипт может вызываться на каждом ходу или только при возникновении
// пересечения.

class ObjSpatialTrigger : public ObjPhysic
{
public:

	ObjectType actors_type;			// Тип объектов, на котрых реагирует.
	UINT actor_id;					// id объекта, на который реагирует.

	bool on_first_collision_only;	// Если true, то скрипт вызываетя только при возникновении пересечения.

	int scriptOnTrigger;			//Номер скрипта в реестре.

	ObjSpatialTrigger();
	virtual ~ObjSpatialTrigger();

	void Trigger(GameObject* obj);	// Вызывается при пересечении каким-либо объектом.
	
};


ObjSpatialTrigger* CreateSpatialTrigger(CAABB aabb, ObjectType actors_type, UINT actor_id, int script, bool on_first_collision_only);

#endif // __OBJECT_SPATIAL_TRIGGER_H_