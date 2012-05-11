//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Render
//
// DESCRIPTION:
//	������ ���������
//  ������������ glDrawArrays()...
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "iichan_render.h"

#ifdef _DEBUG_RENDER
	
#include "../misc.h"

#endif // _DEBUG_RENDER



// ������ ��� ���������
render_data*	all_render_data;
uint			all_render_data_size;


// ����� id �������� ��� ���� � ������ ���������?
render_data* _id_exists(render_data* p_data, uint data_size, GLuint id);

// ���������� ����
void _free_render_data(render_data* p_data);




#define BLOCK_INIT_SIZE 1024 // bytes
#define BLOCK_SIZE 512		 // bytes


// TODO:
// �������� ��� � ��������� (10-20-100) ����� ������� ���������, �� ����� �� ��� ���������� ������
// ������ ��� �������� ���: ������ ���������� ������� ������ ��� ����������� �������� � ����������� ���� �� �� �������
// ��� ������� ����� ������ ����������, �� �� �������������, ������� ����� ���������� ��������, ����� ������ ��������
// �� ����� �� ������������ (�������� ��� ������� � ���� ���� ����������)



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






// �������� ������ ��������� � �������������

void r_AddRenderData(render_data* p_data)
{
	if(!p_data)
		return;


	GLuint id = p_data->texture_id;

	render_data* xdata = _id_exists(all_render_data, all_render_data_size, id);
	
	if(xdata != NULL)
	{
		// ��������� ��������� ������

		uint new_size = xdata->size + p_data->size;

		// ����� �� ������������ ������
		bool need_realloc = false;


		// ���� ��� �� ������� ���������� ������ ��� ���������� ������ �����
		if(xdata->size + p_data->allocated_size >= xdata->allocated_size)
		{
			uint d_size = p_data->allocated_size - (xdata->allocated_size - xdata->size + BLOCK_SIZE);

			// d_size - �������������� ������, ���� ���������� ������ BLOCK_SIZE ��� ����� �� ������ �����
			// �������� ����
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
		// ����� ������
		all_render_data = (render_data*)realloc(all_render_data, all_render_data_size + 1);
		memcpy(all_render_data[all_render_data_size], p_data, sizeof(render_data));

		all_render_data_size += 1;
	}

#ifdef _DEBUG_RENDER
	IICHAN_LOG("New render_data");
#endif // _DEBUG_RENDER
}





// �������� ���������

void r_ZeroRenderData(void)
{
	ZeroMemory(all_render_data, sizeof(all_render_data));

#ifdef _DEBUG_RENDER
	IICHAN_LOG("Render data was zeroized");
#endif // _DEBUG_RENDER
}



// �������� ��������� (���������� ������)

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




// ���������� ������ ���������

void r_RenderData(render_data* p_data);



// ���������� ���

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




