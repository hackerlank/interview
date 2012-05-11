#include "iichan_player.h"
#include "iichan_small_types.h"
#include "iichan_hud.h"
#include "iichan_eventalloc.h"
#include "iichan_spritealloc.h"
#include "iichan_itemalloc.h"
#include "iichan_scrtextalloc.h"



class IICHAN;
class IICHANSpriteAllocator;
class IICHANItemAllocator;
class IICHANEventAllocator;
class IICHANScreenTextAllocator;



class IICHAN {
private:
	IICHANSpriteAllocator* SpriteAllocator;
	IICHANItemAllocator* ItemAllocator;
	IICHANEventAllocator* EventAllocator;
	IICHANScreenTextAllocator* ScreenTextAllocator;
	HUD* hud;

public:

	void SetSpriteAllocator(IICHANSpriteAllocator* new_value)
	{ SpriteAllocator = new_value; }
	IICHANSpriteAllocator* GetSpriteAllocator() const
	{ return SpriteAllocator; }

	void SetItemAllocator(IICHANItemAllocator* new_value)
	{ ItemAllocator = new_value; }
	IICHANItemAllocator* GetItemAllocator() const
	{ return ItemAllocator; }

	void SetEventAllocator(IICHANEventAllocator* new_value)
	{ EventAllocator = new_value; }
	IICHANEventAllocator* GetEventAllocator() const
	{ return EventAllocator; }

	void SetScreenTextAllocator(IICHANScreenTextAllocator* new_value)
	{ ScreenTextAllocator = new_value; }
	IICHANScreenTextAllocator* GetScreenTextAllocator() const
	{ return ScreenTextAllocator; }

	void SetHUD(HUD* new_value) { hud = new_value; }
	HUD* GetHUD() const { return hud; }

	void Draw();
	void Move();

	void PrintDebug();


	void Init();

	void SetWorldMaxX(int value);
	void SetWorldMinX(int value);

	IICHAN();
	~IICHAN();

};





