#include "StdAfx.h"

#include "../config.h"

#include "../render/renderer.h"

#include "objects/object_sprite.h"

#include "ribbon.h"

//////////////////////////////////////////////////////////////////////////

extern config cfg;

extern float CAMERA_OFF_X;
extern float CAMERA_OLD_OFF_X;
extern float CAMERA_X;
extern float CAMERA_LEFT;
extern float CAMERA_RIGHT;

//////////////////////////////////////////////////////////////////////////

void AddRibbon(Ribbon* r);

//////////////////////////////////////////////////////////////////////////

list<Ribbon*> ribbons;

//////////////////////////////////////////////////////////////////////////
// Ribbon (�����) - ������ ����, ���������� �� ���������, �������� �� �������� �������� ������.
// ����� ���� ����������� ��� ����� ������������� �������, �� ��������� ������� �� ������������.
// ������, ������������ ��� ����� ������ �� ������ ��������� ��� ������������ ������ ������.

// ������� ��������� � ��������� �����. ������ ���������� ����� ��������� ��������� ������.
void Ribbon::Draw()
{
	float dx = CAMERA_OFF_X - CAMERA_OLD_OFF_X;		// �������� ������.
	this->aabb.p.x -= dx*speed_factor;				// �������� �����.

	if (use_bounds && (br < CAMERA_LEFT || bl > CAMERA_RIGHT))
		return;			// ����� �� �������, ������ ��������.

	float left = this->aabb.Left();
	float right = this->aabb.Right();

	Sprite* s = this->sprite;

	// ���� ����� �������� ������ �� �����������, ������� ������� ���������� Y.
	float y = this->aabb.p.y - this->aabb.H;	// ���� �������� �� ������.	
	float y1 = s->frameY;						// ������ � ��������.
	float y2 = y1 + s->frameHeight;				// ������ � ��������.

	// ������ ���������� X
	if ((left <= CAMERA_LEFT) && 
		(right >= CAMERA_RIGHT))
	{
		// ����� ��������� ���������� �� ������. ����� ���� ���������.
		float x = CAMERA_LEFT;						// ���� �������� �� ������.	
		float x1 = s->frameX + (CAMERA_LEFT - left);// ������ � ��������.
		float x2 = x1 + cfg.scr_width;				// ������ � ��������. 

		if (use_bounds)
		{
			// ���������� �� ������
			if (bl > x)
			{
				// ����� ����� �������
				x1 += bl - x;
				x = bl;
			}

			if (br < CAMERA_RIGHT)
			{
				// ����� ������ �������
				x2 -= CAMERA_RIGHT - br;
			}
		}

		RenderSprite(x, y, s->z, x1, y1, x2, y2, s->tex, s->IsMirrored());
	}
	else
	{
		// ����� �� ��������� ����� �� ������. ���������� ���������� "�����" ������� � "������".
		bool rend1 = true;			// �������� �����
		bool rend2 = true;			// �������� ������

		float x1 = CAMERA_LEFT;
		float x11 = s->frameX + CAMERA_LEFT;
		float x12 = s->frameX + s->frameWidth; 

		float x2 = 0;
		float x21 = s->frameX;

		if (dx >= 0)
		{
			// ������ �������� �����
			// ����� ������� ������� ������ ����� �� ������� �����, � 
			// ����� ������� ������ �� ������.
			if (left > CAMERA_LEFT)
			{
				// �������� ������ ����� �� ��� ������.
				aabb.p.x -= 2*aabb.W;
				left = aabb.Left();
				right = aabb.Right();
			}
			
			x11 -= left;
			x2 = aabb.Right();
		}
		else
		{
			// ������ �������� ������
			// ����� ������� ������� ������ ����� �� ������, � ����� ������� ������ ��
			// ������� ������.
			if (right < CAMERA_RIGHT)
			{
				// �������� ������ ������ �� ��� ������.
				aabb.p.x += 2*aabb.W;
				left = aabb.Left();
				right = aabb.Right();
			}

			x11 += s->frameWidth - left;
			x2 = left;
		}

		float x22 = cfg.scr_width - (x12 - x11);

		if (use_bounds)
		{
			// ���������� �� ������.
			if (bl > x1 && bl < x2)
			{
				// ����� ������� �� ������.
				x11 += bl - x1;
				x1 = bl;
			}
			else if (bl > x2)
			{	
				// ����� ������� �� ������, ����� �� ��������.
				x21 += bl - x2;
				x2 = bl;
				rend1 = false;
			}

			if (br < CAMERA_RIGHT && br > x2)
			{
				// ������ ������� �� ������.
				x22 -= (x22 - x21) - (br - x2);
			}
			else if (br < x2)
			{
				// ������ ������� �� ������, ������ �� ��������.
				x12 -= x2 - br;
				rend2 = false;
			}
		}

		if (rend1)
		{
			// ������ �����
			RenderSprite(x1, y, s->z, x11, y1, x12, y2, s->tex, s->IsMirrored());
		}

		if (rend2)
		{
			// ������ ������
			RenderSprite(x2, y, s->z, x21, y1, x22, y2, s->tex, s->IsMirrored());
		}
	}
}

//////////////////////////////////////////////////////////////////////////

Ribbon* CreateRibbon(Proto* proto, Vector2 coord, bool overr_z, float z, float speed_factor, float x1, float y1, float x2, float y2)
{
	if (!proto)
		return NULL;

	Ribbon* r = new Ribbon();

	// ������� GameObject ������ ����� �������� ��� � LoadObjectFromProto.
	// ����� �� ��� ������. ���� �� LoadObjectFromProto ����������.
	GameObject* obj = new GameObject();

	LoadObjectFromProto(proto, obj);

	r->sprite = obj->sprite;
	obj->aabb.p = coord;

	SetAnimation(obj, "idle", false, true);
	r->aabb.W = obj->aabb.W;
	r->aabb.H = obj->aabb.H;
	r->aabb.p = obj->aabb.p;
	DELETESINGLE(obj);

	r->speed_factor = speed_factor;

	if (overr_z)
		r->sprite->z = z;

	if (x1 != x2 && y1 != y2)
	{
		// ����� �������������� �������.
		r->use_bounds = true;
		r->bl = min(x1, x2);
		r->br = max(x1, x2);
		r->bt = min(x1, x2);
		r->bb = max(x1, x2);
	}
	AddRibbon(r);

	return r;
}

Ribbon* CreateRibbon(const char* proto_name, Vector2 coord, float speed_factor, float x, float y, float w, float h)
{
	if (!proto_name)
		return NULL;

	return CreateRibbon(GetProto(proto_name), coord, false, 0, speed_factor, x, y, w, h);
}

Ribbon* CreateRibbon(const char* proto_name, Vector2 coord, float z, float speed_factor, float x, float y, float w, float h)
{
	if (!proto_name)
		return NULL;

	return CreateRibbon(GetProto(proto_name), coord, true, z, speed_factor, x, y, w, h);
}

void AddRibbon(Ribbon* r)
{
	ribbons.push_back(r);
}

void DeleteRibbon(Ribbon* r)
{
	DELETESINGLE(r->sprite);
	DELETESINGLE(r);
}

void DeleteAllRibbons()
{
	for(list<Ribbon*>::iterator it = ribbons.begin();
		it != ribbons.end();
		it++)
	{
		DeleteRibbon(*it);
	}
	ribbons.clear();
}

// ������� ��������� � ��������� ���� ����. ������ ���������� ����� ��������� ��������� ������.
void DrawRibbons()
{
	for(list<Ribbon*>::iterator it = ribbons.begin();
		it != ribbons.end();
		it++)
	{
		Ribbon* r = *it;
		r->Draw();
	}
}