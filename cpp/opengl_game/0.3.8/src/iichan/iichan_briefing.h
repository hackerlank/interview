#ifndef __ii_BRIEFING_H_
#define __ii_BRIEFING_H_

//struct BriefingLine {
//	int position_x;
//	int position_y;
//	BYTE color_red;
//	BYTE color_green;
//	BYTE color_blue;
//};

struct Briefing {
	vector<string> lines;
	int line_count;
	float speed;

	float position_x;
	float position_y;
	float tick;
	int width;
	int height;
	int max_line_length;
	int line_dy;
	bool done;

	Briefing()
	{
		done = false;
		tick = 0.0f;
		speed = 0.0f;
		position_x = 0.0f;
		position_y = 0.0f;
		width = 0;
		height = 0;
		max_line_length = 25;
		line_dy = 10;
		line_count = 0;
	}
};

bool LoadBriefing(const char* file_name, Briefing* br);

#endif