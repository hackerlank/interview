#ifndef __IICHAN_RENDER_H_
#define __IICHAN_RENDER_H_

#include "iichan_types.h"


typedef struct {


	// Идентификатор текстуры OGL

	GLuint			texture_id;



	// Массив вершин

	vertex2f_t*		vertices;



	// Массив координат

	coord2f_t*		coords;



	// Выделенная память

	uint			allocated_size;


	// Действительный размер

	uint			size;


} render_data;






// Добавить список отрисовки к существующему

void r_AddRenderData(render_data* p_data);



// Обнулить отрисовку

void r_ZeroRenderData(void);



// Очистить отрисовку (освободить память)

void r_ClearRenderData(void);



// Отрисовать список отрисовки

void r_RenderData(render_data* p_data);



// Отрисовать все

void r_RenderAll(void);








#endif