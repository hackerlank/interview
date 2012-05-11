#ifndef __SPRITE_H_
#define __SPRITE_H_


#include "animation.h"
#include "../render/texture.h"

typedef map<string, Animation*> AnimationMap;

struct Sprite
{
	Texture* tex;

	// —мещение, с которого будет отсчитыватьс€ спрайт.
	USHORT frameX; 
	USHORT frameY;
	// Ўирина и высота кадра текстуры
	USHORT frameWidth;
	USHORT frameHeight;
	// Ўирина и высота всего массива кадроов текстуры
	USHORT imageWidth;
	USHORT imageHeight;

	UINT currentFrame;

	AnimationMap *anims;
	string cur_anim;
	UINT prevAnimTick;				// ѕредыдущий тик, во врем€ которого сменилс€ кадр

	float z;


	// ‘лаги
	// dead			1		спрайт мертв, не используетс€
	// mirrored		2		спрайт отзеркален
	// fixed		4		используютс€ координаты окна, а не мира
	// visible		8		будет рендеритс€
	// looped		16		анимаци€ будет зациклена
	// animDone		32		анимаци€ завершена
	BYTE flags;

	__INLINE BYTE IsDead()		{ return flags & 1; }
	__INLINE BYTE IsMirrored()	{ return flags & 2; }
	__INLINE BYTE IsFixed()		{ return flags & 4; }
	__INLINE BYTE IsVisible()	{ return flags & 8; }
	__INLINE BYTE IsLooped()	{ return flags & 16; }
	__INLINE BYTE IsAnimDone()	{ return flags & 32; }

	__INLINE void SetDead()			{ flags |= 1; }
	__INLINE void SetMirrored()		{ flags |= 2; }
	__INLINE void SetFixed()		{ flags |= 4; }
	__INLINE void SetVisible()		{ flags |= 8; }
	__INLINE void SetLooped()		{ flags |= 16; }
	__INLINE void SetAnimDone()		{ flags |= 32; }

	__INLINE void ClearDead()		{ flags &= ~1; }
	__INLINE void ClearMirrored()	{ flags &= ~2; }
	__INLINE void ClearFixed()		{ flags &= ~4; }
	__INLINE void ClearVisible()	{ flags &= ~8; }
	__INLINE void ClearLooped()		{ flags &= ~16; }
	__INLINE void ClearAnimDone()	{ flags &= ~32; }

	void ChangeFrame(Animation* a);

	Animation* GetAnimation(string name);
	void SetLastAnimationFrame();

	Sprite()
	{
		tex = NULL;
		frameX = 0;
		frameY = 0;

		z = 0;

		frameHeight = 0;
		frameWidth = 0;
		flags = 0;
		imageHeight = 0;
		imageWidth = 0;
		prevAnimTick = 0;
		currentFrame = 0;
		cur_anim = "";
		anims = NULL;
	}

	~Sprite()
	{
		if (this->anims)
		{
			Animation* a = NULL;
			for(AnimationMap::iterator ait = this->anims->begin();
				ait != this->anims->end();
				ait++)
			{
				a = ait->second;
				DELETESINGLE(a);
			}
			DELETESINGLE(this->anims);
		}
	}
};

#endif // __SPRITE_H_