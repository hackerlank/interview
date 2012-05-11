//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Render
//
// DESCRIPTION:
//	Логика отрисовки
//  Используются glDrawArrays()...
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "iichan_render.h"

#ifdef _DEBUG_RENDER
	
#include "../misc.h"

#endif // _DEBUG_RENDER



// Данные для отрисовки
render_data*	all_render_data;
uint			all_render_data_size;


// Такой id текстуры уже есть в списке отрисовки?
render_data* _id_exists(render_data* p_data, uint data_size, GLuint id);

// Освободить дату
void _free_render_data(render_data* p_data);




#define BLOCK_INIT_SIZE 1024 // bytes
#define BLOCK_SIZE 512		 // bytes


// TODO:
// Возможно раз в несколько (10-20-100) тиков следует проверять, не стоит ли нам освободить память
// Сейчас это работает так: память выделяется блоками только при добавлениях объектов и расширяется если ее не хватает
// При очистке сцены память обнуляется, но не освобождается, поэтому может возникнуть ситуация, когда память выделена
// Но никак не используется (например все объекты в игре были уничтожены)



//-----------------------------------------------------------------------------

render_data* _id_exists(render_data* p_data, uint data_size, GLuint id)
{
	if(data_size == 0)
		return NULL;

	assert(p_data != NULL);
	assert(id != 0);


	for(uint i = 0; i < data_size; i++)
	{
		if(p_data[i].texture_id == id)
			return p_data[i];

	}

	return NULL;

}



void _free_render_data(render_data* p_data)
{
	assert(p_data != NULL);

	if(p_data->coords)
		free(p_data->coords);

	if(p_data->vertices)
		free(p_data->vertices);

}






// Добавить список отрисовки к существующему

void r_AddRenderData(render_data* p_data)
{
	if(!p_data)
		return;


	GLuint id = p_data->texture_id;

	render_data* xdata = _id_exists(all_render_data, all_render_data_size, id);
	
	if(xdata != NULL)
	{
		// Расширяем имеющийся список

		uint new_size = xdata->size + p_data->size;

		// Нужно ли перевыделять память
		bool need_realloc = false;


		// Если нам не хватает выделенной памяти для добавления нового блока
		if(xdata->size + p_data->allocated_size >= xdata->allocated_size)
		{
			uint d_size = p_data->allocated_size - (xdata->allocated_size - xdata->size + BLOCK_SIZE);

			// d_size - дополнительная память, если добавления одного BLOCK_SIZE все равно не хватит чтобы
			// добавить блок
			uint blocks_to_add = (d_size / BLOCK_SIZE) + 1;

			//x_data->allocated_size += (d_size > 0 ? (BLOCK_SIZE + d_size) : BLOCK_SIZE);
			x_data->allocated_size += (blocks_to_add * BLOCK_SIZE);

			need_realloc = true;
		}
		
		if(need_realloc)
		{
			xdata->coords = (coord2f_t*)realloc(xdata->coords, xdata->allocated_size);
			xdata->vertices = (vertex2f_t*)realloc(xdata->vertices, xdata->allocated_size);
		}
		
		memcpy(xdata->coords[xdata->size], p_data->coords, p_data->size);		
		memcpy(xdata->vertices[xdata->size], p_data->vertices, p_data->size);

		xdata->size = new_size;
	}
	else
	{
		// Новый список
		all_render_data = (render_data*)realloc(all_render_data, all_render_data_size + 1);
		memcpy(all_render_data[all_render_data_size], p_data, sizeof(render_data));

		all_render_data_size += 1;
	}

#ifdef _DEBUG_RENDER
	IICHAN_LOG("New render_data");
#endif // _DEBUG_RENDER
}





// Обнулить отрисовку

void r_ZeroRenderData(void)
{
	ZeroMemory(all_render_data, sizeof(all_render_data));

#ifdef _DEBUG_RENDER
	IICHAN_LOG("Render data was zeroized");
#endif // _DEBUG_RENDER
}



// Очистить отрисовку (освободить память)

void r_ClearRenderData(void)
{
	for(uint i = 0; i < all_render_data_size; i++)
	{
		_free_render_data(all_render_data[i]);
	}

	free(all_render_data);

	all_render_data = NULL;
	all_render_data_size = 0;

#ifdef _DEBUG_RENDER
	IICHAN_LOG("Render data cleared");
#endif // _DEBUG_RENDER
}




// Отрисовать список отрисовки

void r_RenderData(render_data* p_data);



// Отрисовать все

void r_RenderAll(void)
{
	if(!all_render_data || !all_render_data_size)
		return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	for(uint i = 0; i < all_render_data_size; i++)
	{
		render_data* rdata = all_render_data[i];

		glBindTexture(GL_TEXTURE_2D, rdata->texture_id);

		glVertexPointer(2, GL_FLOAT, 0, rdata->vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, rdata->coords);

		glDrawArrays(GL_QUADS, 0, rdata->size);

	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}




