#ifndef __SPRITE_H_
#define __SPRITE_H_
#include "../draw/texture.h"
#include "iichan_defines.h"
#include "iichan_resource.h"


class SPRITE {
private:
	int id;
	texture_t* tex;
	int patternWidth;
	int patternHeight;
	int patternX;
	int patternY;
	int imageWidth;
	int imageHeight;
	int realWidth;
	int realHeight;
	int realX;
	int realY;
	int realMirrorX;
	int realMirrorY;
	bool mirror;
	bool fixed;
	bool dead;
	bool collidable;
	int z;
	int x;
	int y;
	int shadowX;
	int shadowY;
	int shadowMirrorX;
	int shadowMirrorY;
	bool shadowed;
	float blend;
	RECT collideRect;
	RECT mirrorCollideRect;
	bool visible;
	bool slowpoke;

public:
	bool IsSlowpoke() const { return slowpoke; }
	void SetSlowpoke(bool new_value) { slowpoke = new_value; }

	RECT GetCollideRect() const { return collideRect; }
	void SetCollideRect(RECT new_value) { collideRect = new_value;	}
	RECT GetMirrorCollideRect() const { return mirrorCollideRect; }
	void SetMirrorCollideRect(RECT new_value) {	mirrorCollideRect = new_value; }

	bool GetVisible() const { return visible; }
	void SetVisible(bool new_value) { visible = new_value; }

	virtual void DrawCollideRect();
	virtual void DrawMirrorCollideRect();

	int GetX() const { return x; }
	void SetX(int new_value) { x = new_value; }
	int GetY() const { return y; }
	void SetY(int new_value) { y = new_value; }

	int GetShadowX() const { return shadowX; }
	void SetShadowX(int new_value) { shadowX = new_value; }

	int GetShadowY() const { return shadowY; }
	void SetShadowY(int new_value) { shadowY = new_value; }

	int GetShadowMirrorX() const { return shadowMirrorX; }
	void SetShadowMirrorX(int new_value) { shadowMirrorX = new_value; }

	int GetShadowMirrorY() const { return shadowMirrorY; }
	void SetShadowMirrorY(int new_value) { shadowMirrorY = new_value; }

	bool IsShadowed() const { return shadowed; }
	void SetShadowed(bool new_value) { shadowed = new_value; }

	float GetBlend() const { return blend; }
	void SetBlend(float new_value) { blend = new_value; }

	void SetDead(bool new_value) { dead = new_value; }
	bool IsDead() const { return dead; }

	void SetId(int new_value) { id = new_value; }
	int GetId() const { return id; }

	bool IsFixed() const { return fixed; }
	void SetFixed(bool new_value) { fixed = new_value; }

	int GetZ() const { return z; }
	void SetZ(int new_value) { z = new_value; }

	bool IsCollidable() const { return collidable; }
	void SetCollidable(bool new_value) { collidable = new_value; }

	int GetPatternX() const { return patternX; }
	void SetPatternX(int new_value) { patternX = new_value; }

	int GetPatternY() const { return patternY; }
	void SetPatternY(int new_value) { patternY = new_value; }

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
	int GetRealMirrorX() const { return realMirrorX; }
	void SetRealMirrorX(int new_value) { realMirrorX = new_value; }
	int GetRealMirrorY() const { return realMirrorY; }
	void SetRealMirrorY(int new_value) { realMirrorY = new_value; }
	int GetRealWidth() const { return realWidth; }
	void SetRealWidth(int new_value) { realWidth = new_value; }
	int GetRealHeight() const { return realHeight; }
	void SetRealHeight(int new_value) { realHeight = new_value; }

	int GetImageWidth() const { return imageWidth; }
	void SetImageWidth(int new_value) { imageWidth = new_value; }
	int GetImageHeight() const { return imageHeight; }
	void SetImageHeight(int new_value) { imageHeight = new_value; }

	
	bool GetMirror() const { return mirror; }
	virtual void SetMirror(bool new_value) { mirror = new_value; }

	virtual void Draw();
	virtual void Move();


	virtual void LoadFromResource(RESOURCE* res);

	SPRITE();

};
#endif