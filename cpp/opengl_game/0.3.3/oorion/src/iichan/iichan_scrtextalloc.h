#ifndef __IICHAN_SCRTEXTALLOC_H_
#define __IICHAN_SCRTEXTALLOC_H_

struct SCREEN_TEXT {
	int time_remaining;
	string text;
	int x;
	int y;
	bool floating;
	int id;
};

class IICHANScreenTextAllocator
{
private:
	int _tick;

protected:
	vector<SCREEN_TEXT*> items;
	int _count; // current count of items

public:
	void AddItem(SCREEN_TEXT* scr_text);
	void DeleteItem(SCREEN_TEXT* scr_text);

	int GetIndexById(int id);

	void Process();
	void Draw();



	IICHANScreenTextAllocator();
	~IICHANScreenTextAllocator();
};

#endif