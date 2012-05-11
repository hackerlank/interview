#ifndef __IICHAN_RENDER_H_
#define __IICHAN_RENDER_H_

#include "iichan_types.h"


typedef struct {


	// ������������� �������� OGL

	GLuint			texture_id;



	// ������ ������

	vertex2f_t*		vertices;



	// ������ ���������

	coord2f_t*		coords;



	// ���������� ������

	uint			allocated_size;


	// �������������� ������

	uint			size;


} render_data;






// �������� ������ ��������� � �������������

void r_AddRenderData(render_data* p_data);



// �������� ���������

void r_ZeroRenderData(void);



// �������� ��������� (���������� ������)

void r_ClearRenderData(void);



// ���������� ������ ���������

void r_RenderData(render_data* p_data);



// ���������� ���

void r_RenderAll(void);








#endif