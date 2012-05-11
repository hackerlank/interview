#include "StdAfx.h"

//#include "../render/texture.h"
//#include "animation.h"
#include "sprite.h"

extern DWORD internal_time;

void Sprite::ChangeFrame(Animation* a)
{
	UINT now = internal_time;
	if (now - prevAnimTick >= a->animationSpeed)
	{
		if (currentFrame == a->frameCount + a->startFrame - 1)
		{
			if(IsLooped())
			{
				currentFrame = a->startFrame;
				ClearAnimDone();
			}
			else
			{
				//currentFrame = 0;
				SetAnimDone();
			}
		}
		else
		{
			currentFrame++;
		}
		prevAnimTick = now;
	}
}

// ¬ыставл€ет текущий кадр анимации на последний.
void Sprite::SetLastAnimationFrame()
{
	if (this->anims && this->anims->size())
	{
		AnimationMap::iterator it = (this->anims->find(this->cur_anim));
		if (it != this->anims->end())
		{
			Animation* a = it->second;
			this->currentFrame = a->frameCount + a->startFrame - 1;
		}
	}
}

Animation* Sprite::GetAnimation(string name)
{
	AnimationMap::iterator it = this->anims->find(name);
	if (it != this->anims->end())
		return it->second;
	else
		return NULL;
}