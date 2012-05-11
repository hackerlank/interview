#include "stdafx.h"
#include "vbo.h"

// Определение расширения VBO, из glext.h
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
#define GL_DYNAMIC_DRAW_ARB 0x88E8
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

// Доступен ли Vertex Buffer Object
bool VBO_ENABLED = false;

// Указатели на функции VBO расширения
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL; // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL; // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL; // VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;

// Имя вершинного буфера
unsigned int VBOVertices = 0;

// Количество вершин
unsigned int VerticesLen = 0;


void InitVBO()
{
	assert(VBO_ENABLED);
	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
	glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
	glGenBuffersARB(1, &VBOVertices);
}

void FreeVBO()
{
	assert(VBO_ENABLED);
	glDeleteBuffersARB(1, &VBOVertices);
}

void BuildVBO(Vertex* vec, unsigned int len)
{
	assert(VBO_ENABLED);
	assert(vec);
	assert(len > 0);

	VerticesLen = len;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOVertices);	
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, len * 2 * sizeof(float), vec, GL_STATIC_DRAW_ARB);
}

void DrawVBO()
{
	assert(VBO_ENABLED);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOVertices);
	glVertexPointer(2, GL_FLOAT, 0, (char*)NULL);
	glDrawArrays(GL_POINTS, 0, VerticesLen);
 
	//glDisableClientState(GL_VERTEX_ARRAY); // запрет вершинных массивов
}