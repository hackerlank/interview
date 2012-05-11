#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "texture.h"
#include "../game/phys/phys_misc.h"
#include "../misc.h"

struct  vertex2f_t{
	float x;
	float y;
	vertex2f_t():x(0),y(0){}
};

struct  vertex3f_t{
	float x;
	float y;
	float z;
	vertex3f_t():x(0),y(0),z(0){}
};

struct coord2f_t{
	float x;
	float y;
	coord2f_t():x(0),y(0){}
};


struct render_data 
{	
    GLuint			texture_id;			// Идентификатор текстуры OGL
    vertex3f_t*		vertices;			// Массив вершин
    coord2f_t*		coords;				// Массив координат текстур
    UINT			vert_allocated_size;		// Выделенная память под массив вершин
	UINT			coord_allocated_size;		// Выделенная память под массив кординат тектур
    UINT			count;				// Количесвто элементов
	
	render_data()
	{
		texture_id = -1;
		vertices = NULL;
		coords = NULL;
		vert_allocated_size = 0;
		coord_allocated_size = 0;
		count = 0;
		
	}
};


void r_ZeroRenderData(void);				// Обнулить отрисовку
void r_ClearRenderData(void);				// Очистить отрисовку (освободить память)
//void r_RenderData(render_data* p_data);		// Отрисовать список отрисовки
void r_RenderAll(void);						// Отрисовать все


void RenderSprite(float x, float y, float z, float x1, float y1, float x2, float y2, Texture* tex, BOOL mirrorX);
void RenderBox(float x, float y, float z, float w, float h);

// Satana врывается и в этот тред, потому что как-то неестественно с классом отрисовки примитивов врываться в font.h


union Matrix2
{
	struct
	{
		float e11;
		float e12;
		float e21;
		float e22;
	};

	float e[2][2];

	Matrix2(float _e11, float _e12, float _e21, float _e22) : e11(_e11), e12(_e12), e21(_e21), e22(_e22)
	{}

	Matrix2()
	{}

	Matrix2(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);

		e11 = c; e12 = s;
		e21 =-s; e22 = c;
	}

	float  operator()(int i, int j) const 
	{
		return e[i][j];
	}

	float& operator()(int i, int j)       
	{
		return e[i][j]; 
	}

	const Vector2& operator[](int i) const
	{
		return reinterpret_cast<const Vector2&>(e[i][0]);
	}

	Vector2& operator[](int i)
	{
		return reinterpret_cast<Vector2&>(e[i][0]);
	}		

	static Matrix2 Identity()
	{
		static const Matrix2 T(1.0f, 0.0f, 0.0f, 1.0f);

		return T;
	}

	static Matrix2 Zer0()
	{
		static const Matrix2 T(0.0f, 0.0f, 0.0f, 0.0f);

		return T;
	}


	Matrix2 Tranpose()
	{
		std::swap(e12, e21);		
		return *this;
	}

	Matrix2 Tranposed() const
	{
		Matrix2 T;

		T.e11 = e11;
		T.e21 = e12;
		T.e12 = e21;
		T.e22 = e22;

		return T;
	}

	__INLINE Matrix2 operator - (void) const
	{
		Matrix2 T;
		T.e11 = -e11;
		T.e12 = -e12;
		T.e21 = -e21;
		T.e22 = -e22;
		return T;
	}

	Matrix2 operator * (const Matrix2& M) const 
	{
		Matrix2 T;

		T.e11 = e11 * M.e11 + e12 * M.e21;
		T.e21 = e21 * M.e11 + e22 * M.e21;
		T.e12 = e11 * M.e12 + e12 * M.e22;
		T.e22 = e21 * M.e12 + e22 * M.e22;

		return T;
	}

	Matrix2 operator ^ (const Matrix2& M) const 
	{
		Matrix2 T;

		T.e11 = e11 * M.e11 + e12 * M.e12;
		T.e21 = e21 * M.e11 + e22 * M.e12;
		T.e12 = e11 * M.e21 + e12 * M.e22;
		T.e22 = e21 * M.e21 + e22 * M.e22;

		return T;
	}

	__INLINE Matrix2 operator * (float s) const
	{
		Matrix2 T;

		T.e11 = e11 * s;
		T.e21 = e21 * s;
		T.e12 = e12 * s;
		T.e22 = e22 * s;

		return T;
	}

	__INLINE Matrix2 operator + (const Matrix2 &M) const
	{
		Matrix2 T;
		T.e11 = e11 + M.e11;
		T.e12 = e12 + M.e12;
		T.e21 = e21 + M.e21;
		T.e22 = e22 + M.e22;
		return T;
	}

	__INLINE Matrix2 operator - (const Matrix2 &M) const
	{
		Matrix2 T;
		T.e11 = e11 - M.e11;
		T.e12 = e12 - M.e12;
		T.e21 = e21 - M.e21;
		T.e22 = e22 - M.e22;
		return T;
	}
};


typedef float scalar;

union Vector4
{
public:
	scalar v[4];
	struct {scalar x, y, z, w;};
	struct {scalar r, g, b, a;};

	__INLINE scalar operator[](int i)
	{
		if (i<0 || i> 3)
			return 0;
		return v[i];
	}


	Vector4(){x=y=z=0.0f;w=1.0f;}
	Vector4(scalar xv, scalar yv, scalar zv, scalar wv){x=xv;y=yv;z=zv;w=wv;}
	//void operator=(const Vector4 &q){x=q.x;y=q.y;z=q.z;w=q.w;}
	__INLINE Vector4 operator+(Vector4 q){return Vector4( x + q.x , y + q.y, z + q.z, w + q.w );}
	__INLINE Vector4 operator-(Vector4 q){return Vector4( x - q.x , y - q.y, z - q.z, w - q.w );}
	__INLINE Vector4 operator*(scalar s){return Vector4(x * s, y * s, z * s, w * s);}
	__INLINE Vector4 operator=(const Vector4& V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
		w = V.w;
		return *this;
	}


	__INLINE Vector4 operator/(scalar s)
	{
		if (s == 0.0f)
			return Vector4();
		scalar is = 1.0f/s; 
		return (*this)*is;
	}
	__INLINE Vector4 operator+=(Vector4 q){(*this) = (*this)+q; return *this;}
	scalar Norm()const{return x * x + y * y + z * z + w * w;}
	scalar Length( void ){return sqrt(x * x + y * y + z * z + w * w );}	
	Vector4 Conjugate(){ return Vector4(-x,-y,-z,w);}
	Vector4 Identity()
	{
		//TODO: Danger chek division by zero
		return (*this).Conjugate()/(*this).Norm();
	};
	scalar InnerProduct(const Vector4& q){return x*q.x+y*q.y+z*q.z+w*q.w;}

	__INLINE Vector4 operator*(const Vector4 &q)
	{
		Vector4 result;

		result.x = w * q.x + x * q.w + y * q.z - z * q.y;
		result.y = w * q.y - x * q.z + y * q.w + z * q.x;
		result.z = w * q.z + x * q.y - y * q.x + z * q.w;
		result.w = w * q.w - x * q.x - y * q.y - z * q.z;

		return result;
	}

#ifdef USING_OPENGL
	__INLINE void glSet()
	{
		glColor4fv(&r);
	}
#endif

};

typedef Vector4 Quaternion;
typedef Vector4 RGBAf;

class CGeometry
{
public:
	int type;
	CAABB box;
	CGeometry()
	{
		box  = CAABB(0.0f, 0.0f, 0.0f, 0.0f);
		type = 0;
	}
	virtual void CalcBBOX(){}
};


class CPolygon : public CGeometry
{
public:
	int numV;		// кол-во вершин
	Vector2 *V;		// указатель на массив вершин
	CPolygon(): numV(0), V(NULL){ }
	CPolygon(int _numV):numV(_numV)
	{
		V = new Vector2 [numV];
		memset(V, 0, sizeof(V[0])*numV);
	}
	void Reset(int _numV);
	void CalcBBOX();

	static bool	Collide	(const CPolygon* A, const Vector2& Apos, const Vector2& Avel, const Matrix2& Aorient,
		const CPolygon* B, const Vector2& Bpos, const Vector2& Bvel, const Matrix2& Borient,
		Vector2& n, float& depth);

};



#define G_PRIM_BLEND_OPT
#define G_POLY_TEX_CELL_SIZE 20
#define G_POLY_OUTLINE_ENABLE
#define G_POLY_TEXTURE_ENABLE

#define PRM_RNDR_OPT_BLEND_ONE		0x01
#define PRM_RNDR_OPT_BLEND_OTHER	0x02
#define PRM_RNDR_OPT_BLEND_NOONE	0x03

class CPrimitiveRender
{
public:
	int BlendingOption, CheckerCellSize;
	bool OutlineEnabled, TextureEnabled, DashedLines, doUseCurrentCoordSystem, LineStippleEnabled;
	static int glListCircleL;
	static int glListCircleS;
	static int glListRingS;
	static int glListHalfCircle;
	scalar Angle, lwidth, depth, psize;
	int dash;
	RGBAf *plClr, *psClr, *ppClr;
	RGBAf lClr, sClr, pClr; 
	CPrimitiveRender()
	{
		lClr = sClr = pClr = RGBAf(1.0f, 1.0f, 1.0f, 1.0f);
		plClr = &lClr;
		psClr = &sClr;
		ppClr = &pClr;
		Angle = depth = 0.0f;
		lwidth = 1.0f;
		psize = 8.0f;
		LineStippleEnabled = doUseCurrentCoordSystem = false;			
	}

	void Init();

	/**
	*	Внизу функции для отображения примитивов.
	*	Суффикс L - примитив рисуется линиями
	*	S - solid
	*	C - complete
	*	Последнее - по разному для каждого примитива.
	*	Обычно - вращение + линии + заливка + ещё что-то
	*/

	void grLine(const Vector2 &v0, const Vector2 &v1);
	void grSegment(const Vector2 &v0, const Vector2 &v1);
	void grSegmentC(const Vector2 &v0, const Vector2 &v1);

	void grRectL(const Vector2 &v0, const Vector2 &v1);
	void grRectS(const Vector2 &v0, const Vector2 &v1);
	void grRectC(const Vector2 &v0, const Vector2 &v1);

	void grCircleL(const Vector2 &p, scalar Radius);
	void grCircleS(const Vector2 &p, scalar Radius);
	void grCircleC(const Vector2 &p, scalar Radius);

	void grPolyC(const Vector2 &p, scalar angle, CPolygon *poly);

	void grRingS(const Vector2 &p, scalar Radius);
	void grRingC(const Vector2 &p, scalar Radius);

	void grArrowL(const Vector2& v0, const Vector2& v1);
	void grArrowC(const Vector2 &v0,const Vector2 &v1);

	void gDrawAABB(CAABB box);
	void grInYan(const Vector2 &p, scalar Radius);
private:
	void BeforeRndr();
	void AfterRndr();
	void CheckBlend();
	void CheckTexture();
};

#endif // __RENDERER_H_