#include "iichan_player.h"
#include "iichan_small_types.h"
#include "iichan_hud.h"
#include "iichan_eventalloc.h"
#include "iichan_spritealloc.h"
#include "iichan_itemalloc.h"
#include "iichan_scrtextalloc.h"
#include "iichan_instancealloc.h"



class ii;
class iiSpriteAllocator;
class iiItemAllocator;
class iiEventAllocator;
class iiScreenTextAllocator;
class iiInstanceAllocator;



class ii {
private:
	iiSpriteAllocator* SpriteAllocator;
	iiItemAllocator* ItemAllocator;
	iiEventAllocator* EventAllocator;
	iiScreenTextAllocator* ScreenTextAllocator;
	iiInstanceAllocator* InstanceAllocator;
	HUD* hud;

	unsigned int _previous_time;
	unsigned int _prev_event_time;
	unsigned int _seconds;

public:

	void SetSpriteAllocator(iiSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	iiSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	void SetItemAllocator(iiItemAllocator* new_value)
	{ ItemAllocator = new_value; }
	iiItemAllocator* GetItemAllocator() const
	{ return ItemAllocator; }

	void SetEventAllocator(iiEventAllocator* new_value)
	{ EventAllocator = new_value; }
	iiEventAllocator* GetEventAllocator() const
	{ return EventAllocator; }

	void SetScreenTextAllocator(iiScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	iiScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	void SetInstanceAllocator(iiInstanceAllocator* new_value)
	{ InstanceAllocator = new_value; }
	iiInstanceAllocator* GetInstanceAllocator() const
	{ return InstanceAllocator; }

	void SetHUD(HUD* new_value) { hud = new_value; }
	HUD* GetHUD() const { return hud; }

	void Draw();
	void Move();
	void Init();
	void SetWorldMaxX(int value);
	void SetWorldMinX(int value);
	void PrintDebug();


	ii();
	~ii();

};