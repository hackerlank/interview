#ifndef __SPRITE_H_
#define __SPRITE_H_
#include "texture.h"

typedef struct tagSPRITE {
	texture_t* tex;
	int x;
	int y;
	int patternWidth;
	int patternHeight;
	int imageWidth;
	int imageHeight;
	int realWidth;
	int realHeight;
	int realX;
	int realY;
	bool mirror;
	bool fixed;

	bool GetFixed() const { return fixed; }
	void SetFixed(bool new_value) { fixed = new_value; }

	int GetX() const { return x; }
	void SetX(int new_value) { x = new_value; }
	int GetY() const { return y; }
	void SetY(int new_value) { y = new_value; }

	texture_t* GetTexture() const { return tex; }
	void SetTexture(texture_t* new_value) { tex = new_value; }

	int GetPatternWidth() const { return patternWidth; }
	void SetPatternWidth(int new_value) { patternWidth = new_value; }
	int GetPatternHeight() const { return patternHeight; }
	void SetPatternHeight(int new_value) { patternHeight = new_value; }

	int GetRealX() const { return realX; }
	void SetRealX(int new_value) { realX = new_value; }
	int GetRealY() const { return realY; }
	void SetRealY(int new_value) { realY = new_value; }
	int GetRealWidth() const { return realWidth; }
	void SetRealWidth(int new_value) { realWidth = new_value; }
	int GetRealHeight() const { return realHeight; }
	void SetRealHeight(int new_value) { realHeight = new_value; }

	int GetImageWidth() const { return imageWidth; }
	void SetImageWidth(int new_value) { imageWidth = new_value; }
	int GetImageHeight() const { return imageHeight; }
	void SetImageHeight(int new_value) { imageHeight = new_value; }

	
	bool GetMirror() const { return mirror; }
	void SetMirror(bool new_value) { mirror = new_value; }

	virtual void Draw();

	tagSPRITE();

} SPRITE;
#endif