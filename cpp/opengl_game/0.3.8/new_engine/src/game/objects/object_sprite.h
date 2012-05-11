#ifndef __OBJECT_SPRITE_H_
#define __OBJECT_SPRITE_H_

#include "object.h"
#include "object_physic.h"

GameObject* CreateSprite(const char* proto_name, Vector2 coord, float z, bool fixed);
GameObject* CreateSprite(const char* proto_name, Vector2 coord, bool fixed);
//GameObject* CreateSprite(Proto* proto, Vector2 coord,bool overr_z, float z, bool fixed);

#endif // __OBJECT_SPRITE_H_