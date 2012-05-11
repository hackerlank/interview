#include "StdAfx.h"

#include "texture.h"
#include "renderer.h"


#include "../misc.h"

#define BLOCK_INIT_SIZE 1024 // bytes
#define BLOCK_SIZE 512		 // bytes

extern float DBG_PRINT_VAL1;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

render_data* sprites_render_data = NULL;	// Массив списков отрисовки спрайтов
UINT sprites_render_data_size = 0;			// Количество списковотрисовки спрайтов в массиве

render_data* lines_render_data = NULL;		// Массив списков отрисовки спрайтов
UINT lines_render_data_size = 0;			// Количество списковотрисовки спрайтов в массиве

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Такой id текстуры уже есть в списке отрисовки?
render_data* _id_exists(render_data* p_data, UINT data_size, GLuint id)
{
	if(data_size == 0)
		return NULL;

	assert(p_data != NULL);
	//assert(id != 0);


	for(UINT i = 0; i < data_size; i++)
	{
		if(p_data[i].texture_id == id)
			return &p_data[i];
	}

	return NULL;

}


// Освободить дату
void _free_render_data(render_data* p_data)
{
	assert(p_data != NULL);

	if(p_data->coords)
		free(p_data->coords);

	if(p_data->vertices)
		free(p_data->vertices);

	p_data->vert_allocated_size = 0;
	p_data->coord_allocated_size = 0;
	p_data->count = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Добавить список отрисовки к существующему
void r_AddRenderData(render_data* p_data, render_data** render_mas, UINT* mas_size)
{
	if(!p_data)
		return;

	GLuint id = p_data->texture_id;

	// Существующий список
	render_data* x_data = _id_exists(*render_mas, *mas_size, id);

	if(x_data != NULL)
	{
		// Расширяем имеющийся список

		UINT new_vert_size = x_data->count*sizeof(vertex3f_t) + p_data->count*sizeof(vertex3f_t);
		// Если нам не хватает выделенной памяти для добавления новых коодинат вершин
		if(new_vert_size > x_data->vert_allocated_size)
		{
			//UINT d_size = p_data->allocated_size - (x_data->allocated_size - x_data->size + BLOCK_SIZE);
			// Разница между необходимым и выделенным объемом
			UINT d_size = new_vert_size - x_data->vert_allocated_size;

			// Выделение производится блоками по BLOCK_SIZE байт
			// Расчитываем необходимое количество блоков (как минимум 1)
			UINT blocks_to_add = (d_size / BLOCK_SIZE) + 1;
			
			// Новый объем выделеной памяти
			x_data->vert_allocated_size += (blocks_to_add * BLOCK_SIZE);

			x_data->vertices = (vertex3f_t*)realloc(x_data->vertices, x_data->vert_allocated_size);
		}
		// TODO: А что если realloc не сработает?!
		memcpy(x_data->vertices + x_data->count, p_data->vertices, p_data->count * sizeof(vertex3f_t));
		free(p_data->vertices);


		if (p_data->coords)
		{
			UINT new_coord_size = x_data->count*sizeof(coord2f_t) + p_data->count*sizeof(coord2f_t);
			// Если нам не хватает выделенной памяти для добавления новых коодинат текстур
			if(new_coord_size > x_data->coord_allocated_size)
			{
				//UINT d_size = p_data->allocated_size - (x_data->allocated_size - x_data->size + BLOCK_SIZE);
				// Разница между необходимым и выделенным объемом
				UINT d_size = new_vert_size - x_data->coord_allocated_size;

				// Выделение производится блоками по BLOCK_SIZE байт
				// Расчитываем необходимое количество блоков (как минимум 1)
				UINT blocks_to_add = (d_size / BLOCK_SIZE) + 1;

				// Новый объем выделеной памяти
				x_data->coord_allocated_size += (blocks_to_add * BLOCK_SIZE);

				x_data->coords = (coord2f_t*)realloc(x_data->coords, x_data->coord_allocated_size);
			}
			// TODO: А что если realloc не сработает?!
			memcpy(x_data->coords + x_data->count, p_data->coords, p_data->count * sizeof(coord2f_t));
			free(p_data->coords);
		}

		x_data->count += p_data->count;
	}
	else
	{
		// Новый список
		*render_mas = (render_data*)realloc(*render_mas, (*mas_size + 1)*sizeof(render_data));
		memcpy(&( (*render_mas)[*mas_size] ), p_data, sizeof(render_data));
		// TODO: А что если realloc не сработает?!
		// Списки отрисовки, возможн лучше хранить свзанным списком. Тогда отдельные списки легче будет изменять.
		// И новый список добавится с большей вероятностью. А может я и чушь порю.
		*mas_size += 1;
	}
}

// Обнулить отрисовку
void r_ZeroRenderData(void)
{
	for(UINT i = 0; i < sprites_render_data_size; i++)
	{
		sprites_render_data[i].count = 0;
	}

	for(UINT i = 0; i < lines_render_data_size; i++)
	{
		lines_render_data[i].count = 0;
	}


#ifdef _DEBUG_RENDER
	LogToFile("Render data was zeroized");
#endif // _DEBUG_RENDER
}

// Очистить отрисовку (освободить память)
void r_ClearRenderData(void)
{
	for(UINT i = 0; i < sprites_render_data_size; i++)
	{
		_free_render_data(&sprites_render_data[i]);
	}

	free(sprites_render_data);

	sprites_render_data = NULL;
	sprites_render_data_size = 0;



	for(UINT i = 0; i < lines_render_data_size; i++)
	{
		_free_render_data(&lines_render_data[i]);
	}

	free(lines_render_data);

	lines_render_data = NULL;
	lines_render_data_size = 0;


}

void RenderSprites()
{
	if(!sprites_render_data || !sprites_render_data_size)
		return;

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for(UINT i = 0; i < sprites_render_data_size; i++)
	{
		render_data* rdata = &sprites_render_data[i];
		
		if (rdata->texture_id)
		{
			glBindTexture(GL_TEXTURE_2D, rdata->texture_id);
			glTexCoordPointer(2, GL_FLOAT, 0, rdata->coords);
			glVertexPointer(3, GL_FLOAT, 0, rdata->vertices);
			glDrawArrays(GL_QUADS, 0, rdata->count);
		}
		else
		{	
			// Если текстуры нет, то будет нарисован просто закрашенный прямоугольник

			//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glPushAttrib(GL_CURRENT_BIT);
			glColor3f(1.0f, 0.5f, 0.25f);
			glDisable(GL_TEXTURE_2D);

			glVertexPointer(3, GL_FLOAT, 0, rdata->vertices);
			glDrawArrays(GL_QUADS, 0, rdata->count);

			//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			glPopAttrib();
		}
		
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	

	/*glEnable(GL_TEXTURE_2D);
	for (UINT i = 0; i < all_render_data_size; i++)
	{
		render_data* r = &all_render_data[i];
		glBindTexture(GL_TEXTURE_2D, r->texture_id);
		
		for (UINT j = 0; j < r->count; j+=4)
		{
			glBegin(GL_QUADS);
			glTexCoord2f(r->coords[j+0].x, r->coords[j+0].y); glVertex3f(r->vertices[j+0].x, r->vertices[j+0].y, r->vertices[j+0].z);
			glTexCoord2f(r->coords[j+1].x, r->coords[j+1].y); glVertex3f(r->vertices[j+1].x, r->vertices[j+1].y, r->vertices[j+1].z);
			glTexCoord2f(r->coords[j+2].x, r->coords[j+2].y); glVertex3f(r->vertices[j+2].x, r->vertices[j+2].y, r->vertices[j+2].z);
			glTexCoord2f(r->coords[j+3].x, r->coords[j+3].y); glVertex3f(r->vertices[j+3].x, r->vertices[j+3].y, r->vertices[j+3].z);
			glEnd();
		}
	}*/

}

void RenderLines()
{
	if(!lines_render_data || !lines_render_data_size)
		return;

	glEnableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	for(UINT i = 0; i < lines_render_data_size; i++)
	{
		render_data* rdata = &lines_render_data[i];

		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(1.0f, 0.5f, 0.25f);


		glVertexPointer(3, GL_FLOAT, 0, rdata->vertices);
		glDrawArrays(GL_LINES, 0, rdata->count);

		glPopAttrib();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}

// Отрисовать все
void r_RenderAll(void)
{
	RenderSprites();
	RenderLines();
}

//////////////////////////////////////////////////////////////////////////

render_data* MakeSpriteRenderData(float x, float y, float z, float x1, float y1, float x2, float y2, Texture* tex, BOOL mirrorX)
{
	DBG_PRINT_VAL1++;

	render_data* rd = (render_data*)malloc(sizeof(render_data));

	// Прямоугольники - по 4 координаты.
	rd->count = 4;

	rd->vert_allocated_size = sizeof(vertex3f_t) * rd->count;
	rd->vertices = (vertex3f_t*)malloc(rd->vert_allocated_size);

	rd->vertices[0].z = rd->vertices[1].z = rd->vertices[2].z = rd->vertices[3].z = z;

	float dx = x2-x1;
	float dy = y2-y1;

	rd->vertices[0].x = x; 
	rd->vertices[0].y = y;

	rd->vertices[1].x = x + dx; 
	rd->vertices[1].y = y;

	rd->vertices[2].x = x + dx; 
	rd->vertices[2].y = y + dy;

	rd->vertices[3].x = x; 
	rd->vertices[3].y = y + dy;

	if(tex)
	{
		rd->coord_allocated_size = sizeof(coord2f_t) * rd->count;
		rd->coords = (coord2f_t*)malloc(rd->coord_allocated_size);

		rd->texture_id = tex->tex;		

		float CWidth = (float)tex->width;
		float CHeight = (float)tex->height;
		float cx1 = x1 / CWidth;
		float cx2 = x2 / CWidth;
		float cy1 = 1 - y1 / CHeight;
		float cy2 = 1 - y2 / CHeight;

		if(mirrorX) 
		{
			rd->coords[0].x = cx2; 
			rd->coords[0].y = cy1;

			rd->coords[1].x = cx1; 
			rd->coords[1].y = cy1;
			
			rd->coords[2].x = cx1; 
			rd->coords[2].y = cy2;

			rd->coords[3].x = cx2; 
			rd->coords[3].y = cy2;
		}
		else
		{	 
			rd->coords[0].x = cx1; 
			rd->coords[0].y = cy1;

			rd->coords[1].x = cx2; 
			rd->coords[1].y = cy1;

			rd->coords[2].x = cx2; 
			rd->coords[2].y = cy2;

			rd->coords[3].x = cx1; 
			rd->coords[3].y = cy2;
		}
	}
	else
	{
		// Случай, если текстура не загружена.
		rd->coord_allocated_size = 0;
		rd->coords = NULL;
		rd->texture_id = 0;
	}

	return rd;
}


render_data* MakeBoxRenderData(float x, float y, float z, float w, float h)
{
	DBG_PRINT_VAL1++;

	render_data* rd = (render_data*)malloc(sizeof(render_data));

	// Прямоугольник из линий - 8 координат.
	rd->count = 8;

	rd->coords = NULL;
	rd->coord_allocated_size = 0;
	rd->texture_id = 0;

	rd->vert_allocated_size = sizeof(vertex3f_t) * rd->count;
	rd->vertices = (vertex3f_t*)malloc(rd->vert_allocated_size);

	rd->vertices[0].z = rd->vertices[1].z = rd->vertices[2].z = rd->vertices[3].z =
		rd->vertices[4].z = rd->vertices[5].z = rd->vertices[6].z = rd->vertices[7].z = z;
	
	// 1 - - 2
	//glVertex2i(_x, y);
	rd->vertices[0].x = x; 
	rd->vertices[0].y = y;
	//glVertex2i(x + w, y);
	rd->vertices[1].x = x + w; 
	rd->vertices[1].y = y;

	// 2 - - 3
	//glVertex2i(x + w, y);
	rd->vertices[2].x = x + w; 
	rd->vertices[2].y = y;
	//glVertex2i(x + w, y + h);
	rd->vertices[3].x = x + w; 
	rd->vertices[3].y = y + h;

	// 3 - - 4
	//glVertex2i(x + w, y + h);
	rd->vertices[4].x = x + w; 
	rd->vertices[4].y = y + h;
	//glVertex2i(x, y + h);
	rd->vertices[5].x = x; 
	rd->vertices[5].y = y + h;

	// 4 - - 1
	//glVertex2i(_x, y + h);
	rd->vertices[6].x = x; 
	rd->vertices[6].y = y + h;
	//glVertex2i(x, y);
	rd->vertices[7].x = x; 
	rd->vertices[7].y = y;

	return rd;
}

//////////////////////////////////////////////////////////////////////////

void RenderSprite(float x, float y, float z, float x1, float y1, float x2, float y2, Texture* tex, BOOL mirrorX)
{
	render_data* rd = MakeSpriteRenderData(x, y, z, x1, y1, x2, y2, tex, mirrorX);
	r_AddRenderData(rd, &sprites_render_data, &sprites_render_data_size);
	free(rd);
}


void RenderBox(float x, float y, float z, float w, float h)
{
	render_data* rd = MakeBoxRenderData(x, y, z, w, h);
	r_AddRenderData(rd, &lines_render_data, &lines_render_data_size);
	free(rd);
}


/************************************************************************/
/* Primitive Render Section                                             */
/************************************************************************/

int CPrimitiveRender::glListCircleL = 0;
int CPrimitiveRender::glListCircleS = 0;
int CPrimitiveRender::glListRingS = 0;
int CPrimitiveRender::glListHalfCircle = 0;


void CPrimitiveRender::grLine(const Vector2 &v0, const Vector2 &v1)
{
	BeforeRndr();
	glColor4fv(&(plClr->r));
	glBegin(GL_LINES);
	glVertex2fv(&(v0.x));
	glVertex2fv(&(v1.x));
	glEnd();
	AfterRndr();
}

void CPrimitiveRender::grSegment(const Vector2 &v0, const Vector2 &v1)
{
	BeforeRndr();
	glColor4fv(&(plClr->r));
	glBegin(GL_LINES);
	glVertex2fv(&(v0.x));
	glVertex2fv(&(v1.x));
	glEnd();
	if (ppClr)
		glColor4fv(&(ppClr->r));
	glBegin(GL_POINTS);
	glVertex2fv(&(v0.x));
	glVertex2fv(&(v1.x));
	glEnd();
	AfterRndr();
}

void CPrimitiveRender::grSegmentC( const Vector2 &v0, const Vector2 &v1 )
{
	BeforeRndr();
	glColor4fv(&(plClr->r));
	glTranslatef(v0.x, v0.y, 0.0f);
	glRotatef(Angle, 0.0f, 0.0f, -1.0f);
	glTranslatef(-v0.x, -v0.y, 0.0f);
	glBegin(GL_LINES);
	glVertex2fv(&(v0.x));
	glVertex2fv(&(v1.x));
	glEnd();
	if (ppClr)
		glColor4fv(&(ppClr->r));
	glBegin(GL_POINTS);
	glVertex2fv(&(v0.x));
	glVertex2fv(&(v1.x));
	glEnd();
	AfterRndr();
}

void CPrimitiveRender::grCircleL(const Vector2 &p, scalar Radius)
{
	if (!glIsList(glListCircleL))
		return;
	BeforeRndr();
	glColor4fv(&(plClr->r));
	glTranslatef(p.x, p.y, depth);
	glScalef(Radius, Radius, 1.0f);
	glCallList(glListCircleL);
	AfterRndr();
}

void CPrimitiveRender::grCircleS(const Vector2 &p, scalar Radius)
{
	if (!glIsList(glListCircleL))
		return;
	BeforeRndr();
	glColor4fv(&(psClr->r));
	glTranslatef(p.x, p.y, depth);
	glScalef(Radius, Radius, 1.0f);
	glCallList(glListCircleS);
	AfterRndr();
}

void CPrimitiveRender::grCircleC(const Vector2 &p, scalar Radius)
{
	grCircleS(p, Radius);
	grCircleL(p, Radius);
	grSegmentC(p, p + Vector2(Radius, 0.0f));
}

void CPrimitiveRender::grRectL(const Vector2 &v0, const Vector2 &v1)
{
	BeforeRndr();
	glColor4fv(&(plClr->r));
	glBegin(GL_LINE_LOOP);
	glVertex2f(v0.x, v0.y);
	glVertex2f(v1.x, v0.y);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v0.x, v1.y);
	glEnd();
	AfterRndr();
}
void CPrimitiveRender::grRectS(const Vector2 &v0, const Vector2 &v1)
{
	BeforeRndr();
	glColor4fv(&(psClr->r));
	glBegin(GL_QUADS);
	glVertex2f(v0.x, v0.y);
	glVertex2f(v1.x, v0.y);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v0.x, v1.y);
	glEnd();
	AfterRndr();
}

void CPrimitiveRender::grRectC(const Vector2 &v0, const Vector2 &v1)
{
	BeforeRndr();
	float hy = (v1.y+v0.y)*0.5f;
	float hx = (v1.x+v0.x)*0.5f;
	glTranslatef(hx, hy, 0.0f);
	glRotatef(Angle, 0.0f, 0.0f, -1.0f);
	glTranslatef(-hx, -hy, 0.0f);
	glColor4fv(&(psClr->r));
	glBegin(GL_QUADS);
	glVertex2f(v0.x, v0.y);
	glVertex2f(v1.x, v0.y);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v0.x, v1.y);
	glEnd();
	glColor4fv(&(plClr->r));
	glBegin(GL_LINE_LOOP);
	glVertex2f(v0.x, v0.y);
	glVertex2f(v1.x, v0.y);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v0.x, v1.y);
	glEnd();
	glColor4fv(&(ppClr->r));
	glBegin(GL_POINTS);
	glVertex2f(v0.x, v0.y);
	glVertex2f(v1.x, v0.y);
	glVertex2f(v1.x, v1.y);
	glVertex2f(v0.x, v1.y);
	glEnd();
	AfterRndr();
}


void CPrimitiveRender::grArrowL(const Vector2& v0, const Vector2& v1)
{
	BeforeRndr();
	float fAngle = atan2(v1.y-v0.y, v1.x-v0.x);
	glTranslatef(v0.x, v0.y, 0.0f);
	glRotatef(RadToDeg(fAngle), 0.0f, 0.0f, 1.0f);
	glScalef((v1-v0).Length(), (v1-v0).Length(), 1.0f);

	glColor4fv(&(plClr->r));
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.75f, 0.2f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.75f,-0.2f);
	glEnd();
	glColor4fv(&(ppClr->r));

	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.75f, 0.2f);
	glVertex2f(0.75f, -0.2f);
	glEnd();

	AfterRndr();
}

void CPrimitiveRender::grArrowC(const Vector2 &v0,const Vector2 &v1)
{
	// TODO: complete function
	//grCircleS()
	grRingS(v0, 20);
	grCircleL(v0, 20);
	grCircleL(v0, 20*0.7f); // MAGIC NUMBERS ARRR!
	grArrowL(v0, v1);
}

void CPrimitiveRender::grPolyC(const Vector2 &p, scalar angle, CPolygon *poly)
{
	BeforeRndr();
	glTranslatef(p.x, p.y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glColor4fv(&(psClr->r));

// #ifdef G_POLY_TEXTURE_ENABLE
// 	glEnable(GL_TEXTURE_2D);
// 	CTextureManager *Tman = CTextureManager::Instance();
// 	CTexture *cells = dynamic_cast<CTexture*>(Tman->GetObjectByName("cells"));
// 	Tman->FreeInst();
// 	cells->Bind();
// #endif 

	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < poly->numV; i++)
	{
#ifdef G_POLY_TEXTURE_ENABLE
		glTexCoord2f(poly->V[i].x/G_POLY_TEX_CELL_SIZE, poly->V[i].y/G_POLY_TEX_CELL_SIZE);
#endif 
		glVertex2fv(&(poly->V[i].x));
	}
	glEnd();

#ifdef G_POLY_OUTLINE_ENABLE

	glEnable(GL_LINE_WIDTH);
	glLineWidth(1.0f);
	glColor4fv(&(plClr->r));
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < poly->numV; i++)
	{
		glVertex2fv(&(poly->V[i].x));
	}
	glEnd();

#endif 
	AfterRndr();
}

void CPrimitiveRender::grRingS(const Vector2 &p, scalar Radius)
{
	BeforeRndr();
	glTranslatef(p.x, p.y, 0.0f);
	glColor4fv(&(psClr->r));
	glScalef(Radius, Radius, 0.0f);
	glCallList(glListRingS);
	AfterRndr();
}

void CPrimitiveRender::grRingC( const Vector2 &p, scalar Radius )
{
	BeforeRndr();
	glTranslatef(p.x, p.y, 0.0f);
	glScalef(Radius, Radius, 0.0f);
	glColor4fv(&(psClr->r));
	glCallList(glListRingS);
	glScalef(1.0f/Radius, 1.0f/Radius, 0.0f);
	glTranslatef(-p.x, -p.y, 0.0f);
	grCircleL(p, Radius);
	grCircleL(p, Radius*0.7f); // MAGIC NUMBER!
	AfterRndr();
}

void CPrimitiveRender::BeforeRndr()
{
	glPushMatrix();
	glPushAttrib(GL_TEXTURE_BIT | GL_DEPTH_BUFFER_BIT | GL_BLEND | GL_LINE_BIT | GL_POINT_BIT);
	glEnable(GL_POINT_SIZE);
	glLineWidth(lwidth);
	glPointSize(psize);
	glEnable(GL_LINE_WIDTH);
	glDisable(GL_TEXTURE_2D);
	if (LineStippleEnabled)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(lwidth*4, 0xAAAA);
	}
	else
		glDisable(GL_LINE_STIPPLE);
	CheckBlend();
	if (doUseCurrentCoordSystem)
		glLoadIdentity();
}

void CPrimitiveRender::AfterRndr()
{
	glPopAttrib();
	glPopMatrix();
}

void CPrimitiveRender::CheckBlend()
{
	switch(BlendingOption)
	{
	case PRM_RNDR_OPT_BLEND_ONE:
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);		
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
	case PRM_RNDR_OPT_BLEND_OTHER:
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case PRM_RNDR_OPT_BLEND_NOONE:
		glDisable(GL_BLEND);
		break;
	}
}

void CPrimitiveRender::CheckTexture()
{
	glEnable(GL_TEXTURE_2D);
// 	CTextureManager *Tman = CTextureManager::Instance();
// 	CTexture *cells = dynamic_cast<CTexture*>(Tman->GetObjectByName("cells"));
// 	Tman->FreeInst();
//	cells->Bind();
}

void CPrimitiveRender::Init()
{
	if (!glIsList(glListCircleL))
	{
		glListCircleL = glGenLists(1);
		glNewList(glListCircleL, GL_COMPILE);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 64 + 1; i ++)
		{
			Vector2 P(cos(PI * (i / 32.0f)), sin(PI * (i / 32.0f)));
			glVertex2f(P.x, P.y);
		}
		glEnd();
		glEndList();
	}

	if (!glIsList(glListCircleS))
	{
		glListCircleS = glGenLists(1);
		glNewList(glListCircleS, GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < 64 + 1; i ++)
		{
			Vector2 P(cos(PI * (i / 32.0f)), sin(PI * (i / 32.0f)));
			glVertex2f(P.x, P.y);
		}
		glEnd();
		glEndList();
	}

	if (!glIsList(glListRingS))
	{
		glListRingS = glGenLists(1);
		glNewList(glListRingS, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < 64 + 1; i ++)
		{
			Vector2 P(cos(PI * (i / 32.0f)), sin(PI * (i / 32.0f)));
			glVertex2f(P.x, P.y);
			P = Vector2(cos(PI * (i / 32.0f)), sin(PI * (i / 32.0f)));
			glVertex2f(P.x*0.7f, P.y*0.7f);
		}
		glEnd();
		glEndList();
	}

	if (!glIsList(glListHalfCircle))
	{
		glListHalfCircle = glGenLists(1);
		glNewList(glListHalfCircle, GL_COMPILE);
		glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < 32 + 1; i ++)
		{
			Vector2 P(cos(PI * (i / 32.0f)), sin(PI * (i / 32.0f)));
			glVertex2f(P.x, P.y);
		}
		glEnd();
		glEndList();
	}


}

void CPrimitiveRender::grInYan(const Vector2 &p, scalar Radius)
{
	BeforeRndr();
	glTranslatef(p.x, p.y, 0.0f);
	glRotatef(Angle, 0.0f, 0.0f, -1.0f);
	sClr = lClr = RGBAf(0.0f, 0.0f, 0.0f, 1.0f);
	glColor4fv(&(psClr->r));
	glScalef(Radius, Radius, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.01f);
	glCallList(glListHalfCircle);
	glRotatef(180.0f, 0.0f, 0.0f, -1.0f);
	sClr = lClr = RGBAf(1.0f, 1.0f, 1.0f, 1.0f);
	glColor4fv(&(psClr->r));
	glCallList(glListHalfCircle);
	AfterRndr();

	glRotatef(Angle, 0.0f, 0.0f, -1.0f);
	sClr = lClr = RGBAf(0.0f, 0.0f, 0.0f, 1.0f);
	grCircleS(Vector2(p.x - Radius*0.5f, p.y), Radius*0.5f);
	sClr = lClr = RGBAf(1.0f, 1.0f, 1.0f, 1.0f);
	grCircleS(Vector2(p.x - Radius*0.5f, p.y), Radius*0.125f);

	sClr = lClr = RGBAf(1.0f, 1.0f, 1.0f, 1.0f);
	grCircleS(Vector2(p.x + Radius*0.5f, p.y), Radius*0.5f);
	sClr = lClr = RGBAf(0.0f, 0.0f, 0.0f, 1.0f);
	grCircleS(Vector2(p.x + Radius*0.5f, p.y), Radius*0.125f);

	//lClr = RGBAf(0.3f, 0.4f, 0.6f, 0.9f);
	lClr = RGBAf(0.0f, 0.0f, 0.0f, 1.0f);
	depth = 0.001f;
	grCircleL(p, Radius/*+lwidth/2.0f*/);
}
