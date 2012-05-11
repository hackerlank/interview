#ifndef __PHYSMISC_H_
#define __PHYSMISC_H_

#include <math.h>
//#include "sap/OPC_ArraySAP.h"
#include "phys/sap/SweepAndPrune.h"
using namespace Opcode;

#pragma warning( disable : 4305 ) 
#pragma warning( disable : 4244 ) 
//#pragma warning( disable : 4996 ) 
#pragma warning( disable : 4172 )

// If enabled then optimized version of 
// Vector2.Length() used. It costs 5% accuracy.
//#define OPTIMIZE_V2L

#define __DEBUG_PHYSICS_
#undef __DEBUG_PHYSICS_

//#define KERNEL_BUG_PHYSICS
//#define SATANA_PHYSICS
#define USING_SAP_PHYSICS
#undef __DEBUG_SAP_PHYSICS_

 


#ifdef WIN32
#define __INLINE __forceinline
#else
#define __INLINE inline
#endif

typedef float scalar;

class Vector2{
public:
	scalar x, y;
	static const Vector2& Blank()
	{ 
		static const Vector2 V(0, 0);
		return V;
	} 
public:
	__INLINE Vector2(void): x(0.0f), y (0.0f){}
	__INLINE Vector2( scalar Ix, scalar Iy) : x(Ix), y(Iy){}
	__INLINE  Vector2 operator + (const Vector2 &V)const
	{ 
		return Vector2(x + V.x, y + V.y);
	}
	__INLINE Vector2 operator - (const Vector2 &V)const
	{ 
		return Vector2(x - V.x, y - V.y);
	}
	__INLINE Vector2 operator * (const scalar a)const
	{ 
		return Vector2(x * a, y * a);
	}
	__INLINE Vector2 operator / (const scalar a)const
	{
		if (a == 0.0f)
			return Vector2().Blank();
		scalar t = 1.0f/a;
		return Vector2(x*t, y*t);
	}
	friend __INLINE  Vector2 operator * (scalar k, const Vector2& V)
	{
		return Vector2(V.x*k, V.y*k);
	}

	__INLINE Vector2 const &operator +=(const Vector2 &V) 
	{
		x += V.x;
		y += V.y;
		return *this;
	}
	__INLINE Vector2 const &operator -=(const Vector2 &V)
	{
		x -= V.x;
		y -= V.y;
		return *this;
	}
	__INLINE Vector2 const  &operator *=(const scalar a)
	{
		x *= a;
		y *= a;
		return *this;
	}
	__INLINE Vector2 const &operator /=(const scalar &a)
	{
		if (a == 0.0f)
			return *this;
		scalar t = 1.0f/a;
		x *= t;
		y *= t;
		return *this;
	}

	__INLINE Vector2 operator -(void) const
	{
		return Vector2( -x, -y);
	}

	// скалярное произведение векторов
	__INLINE scalar operator * (const Vector2 &V) const
	{
		return x * V.x + y * V.y;
	}

	// попытка сделать аналог векторного произведения в двух измеениях
	__INLINE scalar operator ^ (const Vector2 &V) const
	{
		return x * V.y - y * V.x;
	}

	__INLINE scalar Length(void) const
	{ 
#ifdef OPTIMIZE_V2L
		scalar dx, dy;
		if ( x < 0 )
			dx = -x;
		else
			dx = x;

		if ( y < 0 )
			dy = -y;
		else
			dy = y;
		if ( dx < dy )  
			return 0.961f*dy+0.398f*dx;  
		else    
			return 0.961f*dx+0.398f*dy;
#else
		return (scalar)sqrt((double)(x*x + y*y));
#endif
	}

	__INLINE Vector2 GetPerp()
	{
		return Vector2(-y, x);
	}

	__INLINE Vector2 Normalized(void)const
	{
		scalar l = Length();
		if ( l == 0.0f )
			return Vector2();
		Vector2 t = (*this) * ( 1.0f / l );	 
		return t;
	}
	__INLINE scalar Normalize(void) 
	{
		scalar l = Length();
		if ( l == 0.0f )
			return 0.0f;
		(*this) *= ( 1.0f / l );	 
		return l;
	}

};


/**
*	Расстояние от точки до прямой вычисляет эта функция.
*	v1, v2 - координаты любых двух (возможно не совпадающих)
*		точек, принадлежащих прямой.
*	p - та самая точка, расстояние от которой мы ищем.
*	TODO : оптимизировать её нужно.
*/

__INLINE scalar PointLineDistance(Vector2 v1, Vector2 v2, Vector2 p)
{
	scalar dx, dy, D;
	dx = v1.x - v2.x;
	dy = v1.y - v2.y;
	D = dx * (p.y - v1.y) - dy * ( p.x - v1.x );
	return  abs( D / sqrt( dx * dx + dy * dy));
}

class AABB
{
public:
	Vector2 p;
	scalar H;
	scalar W;

	scalar Left() const { return p.x - W; }
	scalar Right() const { return p.x + W; }
	scalar Top() const { return p.y - H; }
	scalar Bottom() const { return p.y + H; }

	scalar GetMin(UINT axis) const { return axis == 0 ? this->Left() : this->Top(); }
	scalar GetMax(UINT axis) const { return axis == 0 ? Right() : Bottom(); }

	ASAP_AABB GetASAP_AABB() 
	{
		ASAP_AABB box;
		box.mMax.x = Right();
		box.mMax.y = Bottom();
		box.mMax.z = 1;
		box.mMin.x = Left();
		box.mMin.y = Top();
		box.mMin.z = 0;
		return box;
	}

	__INLINE AABB(void) : H(0.0f), W(0.0f) { }

	AABB(const AABB& a)
	{
		p = a.p;
		H = a.H;
		W = a.W;
	}

	__INLINE bool IsEmpty ()
	{
		return (H == 0 && W == 0);
	}

	void Intersect(const AABB &a, const AABB &b)
	{
		if (fabs(a.p.x - b.p.x) <= a.W + b.W &&
			fabs(a.p.y - b.p.y) <= a.H + b.H)
		{
			scalar al, bl, ar, br, at, bt, ab, bb;
			scalar cl, cr, cb, ct;

			al = a.p.x - a.W;
			at = a.p.y - a.H;
			bl = b.p.x - b.W;
			bt = b.p.y - b.W;

			ar = a.p.x + a.W;
			ab = a.p.y + a.H;
			br = b.p.x + b.W;
			bb = b.p.y + b.W;

			cl = al > bl ? al : bl;
			cr = ar < br ? ar : br;
			ct = at > bt ? at : bt;
			cb = ab < bb ? ab : bb;

			W = (cl - cr) / 2;
			H = (cb - ct) / 2;
			p.x = cl + W;
			p.y = ct + H;
		}
		else
		{
			H = 0;
			W = 0;
			p.Blank();
		}
	}
};

/**
*	Collide - сталкиваются ли 2 бокса.
*	TODO: слишком дохуя сложений\вычитаний в такой простой ф-ии.
*/

__INLINE bool Collide(const AABB &a, const AABB &b)
{
	if (a.p.x + a.W < b.p.x - b.W || a.p.x - a.W > b.p.x + b.W)
		return false;
	if (a.p.y + a.H < b.p.y - b.H || a.p.y - a.H > b.p.y + b.H)
		return false;
	return true;
}

/**
*	Дефайны для типов пересечений отрезков проекций боксов.
*	Тоесть: нет пересечения - нам надо знать первый отезок перед вторым или после, если есть пересечение, то хрен с ним есть.
*/

#define SEG_PROJ_STATE_BEFORE		0x01
#define SEG_PROJ_STATE_INTERSECT	0x02
#define SEG_PROJ_STATE_AFTER		0x03

/**
*	Собственно определяет тип пересечения отрезков проекций боксов
*	Вообще не проекция боксов блаблабла, а просто тип пересечения отезков. Но мы передаем в неё обычно проекции боксов.
*	a.x, a.y - 1я и 2я координаты первого отрезка. Вообще, то что они в векторе и названы x y может сбить с мысли. Пусть не сбивает.
*/

__INLINE int SegProjIntersect(const Vector2 &a, const Vector2 &b)
{
	if (a.y <= b.x)
		return SEG_PROJ_STATE_BEFORE;
	if (a.x >= b.y)
		return SEG_PROJ_STATE_AFTER;
	return SEG_PROJ_STATE_INTERSECT;
}

// Хрень. Вроде не используется даже.
// Действительно не используется. Хрень.
__INLINE Vector2 ResolveVector(int x, int y)
{
	if (x == SEG_PROJ_STATE_INTERSECT)
	{
		switch(y)
		{
		case SEG_PROJ_STATE_BEFORE:
			return Vector2(0.0f, -1.0f);
		case SEG_PROJ_STATE_AFTER:
			return Vector2(0.0f, 1.0f);
		}
	}

	if (y == SEG_PROJ_STATE_INTERSECT)
	{
		switch(x)
		{
		case SEG_PROJ_STATE_BEFORE:
			return Vector2(-1.0f, 0.0f);
		case SEG_PROJ_STATE_AFTER:
			return Vector2(1.0f, 0.0f);
		}
	}
	return Vector2(0.0f, 0.0f);

}

/**
*	Это дефайны углов. C - Corner.
*	TL - Top Left
*	BR - Bottom right 
*	Ну и по аналогии
*/

#define C_TL 0x00
#define C_TR 0x01
#define C_BL 0x02
#define C_BR 0x03

// Очевидно.

__INLINE void Sort2f(float &f1, float &f2)
{
	if (f1 > f2)
		swap(f1, f2);
}

/**
*	Опередляет тип угла, по типам пересецчения проекций боксов.
*	sX - тип проекции по оси Х, для У аналогично.
*/

__INLINE int DetectCorner(const int sX, const int sY)
{
	if (sY == SEG_PROJ_STATE_BEFORE)
	{
		if (sX == SEG_PROJ_STATE_BEFORE )
			return C_TL;
		if (sX == SEG_PROJ_STATE_AFTER )
			return C_TR;
	}

	if (sY == SEG_PROJ_STATE_AFTER)
	{
		if (sX == SEG_PROJ_STATE_BEFORE )
			return C_BL;
		if (sX == SEG_PROJ_STATE_AFTER )
			return C_BR;
	}
}

/*
*	Ф-я CornerProblemSolve(...) должна решать ту проблему с углами. Она пересекает отрезки и всё такое.
*	Ящитаю больше нечего комментировать, потому что и так в ней хуй разберёшься. Лучше не трогать.
*/

__INLINE bool CornerProblemSolve(int &Sx, int &Sy, const AABB &dyn, const AABB &stat, const Vector2 &vel)
{
	float hy0 = 0.0f, hy1 = 0.0f;
	float vx0 = 0.0f, vx1 = 0.0f;
	Vector2 v0;	
	Vector2 v1;
	Vector2 x0, x1, p;
	int corner = DetectCorner(Sx, Sy);

	switch(corner)
	{
	case C_TL:
		{
			v0.x = dyn.p.x + dyn.W;
			v0.y = dyn.p.y + dyn.H;
			hy0 = stat.p.y - stat.H;
			vx0 = stat.p.x - stat.W;
			hy1 = stat.p.y + stat.H;
			vx1 = stat.p.x + stat.W;
		}
		break;
	case C_TR:
		{
			v0.x = dyn.p.x - dyn.W;
			v0.y = dyn.p.y + dyn.H;
			hy0 = stat.p.y - stat.H;
			vx0 = stat.p.x + stat.W;
			hy1 = stat.p.y + stat.H;
			vx1 = stat.p.x - stat.W;
		}
		break;
	case C_BL:
		{
			v0.x = dyn.p.x + dyn.W;
			v0.y = dyn.p.y - dyn.H;
			hy0 = stat.p.y + stat.H;
			vx0 = stat.p.x - stat.W;
			hy1 = stat.p.y - stat.H;
			vx1 = stat.p.x + stat.W;
		}
		break;
	case C_BR:
		{
			v0.x = dyn.p.x - dyn.W;
			v0.y = dyn.p.y - dyn.H;
			hy0 = stat.p.y + stat.H;
			vx0 = stat.p.x + stat.W;
			hy1 = stat.p.y - stat.H;
			vx1 = stat.p.x - stat.W;
		}
		break;
	}
	p = Vector2(vx0, hy0);
	v0 -= p;
	v1 = v0 + vel;
	float k = (v1.y - v0.y)/(v1.x - v0.x);
	float b = v1.x*k - v1.y;
	x0.x = -b/k;
	x0.y = 0.0f;
	x1.x = 0.0f;
	x1.y = b;
	x0 += p;
	x1 += p;
 
  	if (abs(x0.x - vx0) < 0.001f && abs(x1.y - hy0) < 0.001f)
	{
		return false;
	}
	Sort2f(hy0, hy1);
	if (hy0 < x1.y && x1.y < hy1)
	{
		Sy = SEG_PROJ_STATE_INTERSECT;
		if (corner == C_TL || corner == C_BL)
			Sx = SEG_PROJ_STATE_BEFORE;
		else
			Sx = SEG_PROJ_STATE_AFTER;
		return true;
	}
	Sort2f(vx0, vx1);
	if (vx0 < x0.x && x0.x < vx1)
	{
		Sx = SEG_PROJ_STATE_INTERSECT;
		if (corner == C_TL || corner == C_TR)
			Sy = SEG_PROJ_STATE_BEFORE;
		else
			Sy = SEG_PROJ_STATE_AFTER;
		return true;
	}
	return false;
}

__INLINE scalar Minf(const scalar a, const scalar b)
{
	return (a < b)?a:b;
}

__INLINE scalar Maxf(const scalar a, const scalar b)
{
	return (a > b)?a:b;
}


#define SEG_INTERS_B	0x00	// no intersection, first segment before second
#define SEG_INTERS_L	0x01	// left intersection
#define SEG_INTERS_AI	0x02	// a inside b
#define SEG_INTERS_BI	0x03	// b inside a
#define SEG_INTERS_R	0x04	// right intersection
#define SEG_INTERS_A	0x05	// no intersection, first segment after second
#define	SEG_INTERS_ERR	0x06	// Error!

__INLINE scalar Minfex(const scalar a, const scalar b)
{
	return (a < b)?a:-b;
}

__INLINE scalar Maxfex(const scalar a, const scalar b)
{
	return (a > b)?a:-b;
}

/**
*	Эта функция (SegIntersect(...)) определяет тип пересечения 2х отрезков
*	a.x < a.y - первый отрезок
*	аналогично для второго (b)
*	В delta мы возвращаем минимальную дистанцию, на которую надо распихнуть отрезки, чтобы они того...
*	Ф-я возвращает тип пересечение: есть или нет, если есть, то слева, справа или внутри, если нет, то перед и или после.
*	Результат рассматривается положение 1го отрезка относительно второго. Так-то.
*/	

__INLINE int SegIntersect(const Vector2 &a, const Vector2 &b, scalar &delta)
{
#ifdef _DEBUG_
	if (a.x > a.y || b.x > b.y)
		return SEG_INTERS_ERR;
#endif
	delta = 0.0f;
	if (a.y < b.x)
		return SEG_INTERS_B;
	if (a.x > b.y)
		return SEG_INTERS_A;
	if (a.x > b.x && a.y < b.y)
	{
		delta = Minfex(b.y-a.x, a.y-b.x);
		return SEG_INTERS_AI;
	}
	if (b.x > a.x && b.y < a.y)
	{
		delta = Minfex(a.y-b.x, b.y-a.x);
		return SEG_INTERS_BI;
	}
	if (a.x > b.x)
	{
		delta = (b.y - a.x);
		return SEG_INTERS_R;
	}
	else
	{
		delta = -(a.y - b.x);
		return SEG_INTERS_L;
	}
	return SEG_INTERS_ERR;
}







#endif