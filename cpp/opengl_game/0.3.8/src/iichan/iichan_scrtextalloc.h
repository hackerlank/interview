#ifndef __ii_SCRTEXTALLOC_H_
#define __ii_SCRTEXTALLOC_H_

struct SCREEN_TEXT {
	int time_remaining;
	string text;
	int x;
	int y;
	bool floating;
	int id;
	int _prev;
	int _prev_float;
	int _d_red;
	int _d_green;
	int _d_blue;

	SCREEN_TEXT()
	{
		_prev = 0;
		_prev_float = 0;
		_d_red = 0;
		_d_green = 0;
		_d_blue = 0;
		floating = false;
	}
};

class iiScreenTextAllocator
{
protected:
	vector<SCREEN_TEXT*> items;
	int _count; // current count of items

public:
	void AddItem(SCREEN_TEXT* scr_text);
	void DeleteItem(SCREEN_TEXT* scr_text);

	int GetIndexById(int id);
	SCREEN_TEXT* GetItemByText(string str);

	void Process();
	void Draw();

	iiScreenTextAllocator();
	~iiScreenTextAllocator();
};

#endif