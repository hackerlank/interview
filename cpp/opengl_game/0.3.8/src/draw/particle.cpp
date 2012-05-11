#include "stdafx.h"
#include "particle.h"
#include "texture.h"
#include "../glext.h"
#include "../misc.h"
#include "../config.h"

PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;

Vertex* vertices = NULL;
Color* colors = NULL;
Coord* coords = NULL;
unsigned int vertex_length = 0;

Texture* particle_texture = NULL;

extern int WORLD_X;
extern int WORLD_Y;
extern int WORLD_MIN_X;
extern int WORLD_MAX_X;

extern config cfg;

#define PARTICLE_TEXTURE_NAME "particle"

// TODO: оптимизировать частицы, сейчас они рисуютс€ по всей карте


void InitParticles()
{
	particle_texture = TextureByName(PARTICLE_TEXTURE_NAME);
	assert(particle_texture);
	glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffers");
	vertices = NULL;
}

void FreeParticles()
{
	if(vertices)
		delete[] vertices;
	vertices = NULL;
	if(colors)
		delete[] colors;
	colors = NULL;
	if(coords)
		delete[] coords;
	coords = NULL;

	vertex_length = 0;

}

void ParticlesTick(Particle* vec, unsigned int len, float tick, RECREATION_TYPE rt, DEAD_CONDITION dc)
{
	assert(vec);

	unsigned int l = len * 4;

	if(len != (vertex_length / 4))
	{
		if(vertices)
			delete[] vertices;
	
		vertices = new Vertex[l];
		ZeroMemory(vertices, l);

		if(colors)
			delete[] colors;

		colors = new Color[len];
		ZeroMemory(colors, len);

		if(coords)
			delete[] coords;

		coords = new Coord[l];
		ZeroMemory(coords, l);
	}

	vertex_length = l;

	for(unsigned int i = 0; i < len; i++)
	{
		Particle* p = &vec[i];

		//if(p->x_pos > WORLD_X + cfg.scr_width)
			//break;
		p->x_pos += (p->x_vec * tick);
		p->y_pos += (p->y_vec * tick);
		p->y_vec += (p->gravity * tick);

		//if(p->x_pos > p->x_floor_max) p->x_pos -= p->size;
		//else if(p->x_pos < p->x_floor_min) p->x_pos += p->size;

		//if(p->x_pos > p->x_floor_min && p->x_pos < p->x_floor_max &&
		//	p->y_pos > p->y_floor_min && p->y_pos < p->y_floor_max)
		//{
		//	if(p->y_pos < p->size)
		//	{
		//		p->y_pos = p->size;
		//		p->y_vec *= -0.6f;
		//	}
		//}
		p->life -= 1.0f * tick;


		switch(dc)
		{
		case dcLifeZero: p->dead = (p->life <= 0); break;
		case dcOutOfScreen: p->dead = (p->y_pos - WORLD_Y >= cfg.scr_height); break;
		default: break;
		}

		// ≈сли частица мертва - пересоздаем 
		if(p->dead)
		{
			switch(rt)
			{
			case rtScreenTop:
				p->x_pos = Random((float)cfg.scr_width);
				p->y_pos = 0;
				break;
			case rtWorldTop:
				p->x_pos = Random((float)WORLD_MIN_X, (float)WORLD_MAX_X + cfg.scr_width);
				p->y_pos = 0;
				break;
			case rtScreenRandom:
				p->x_pos = Random((float)cfg.scr_width);
				p->y_pos = Random((float)cfg.scr_height);
				break;
			case rtWorldRandom:
				p->x_pos = Random((float)WORLD_MIN_X, (float)WORLD_MAX_X + cfg.scr_width);
				p->y_pos = Random((float)cfg.scr_height);
				break;
			default:
				break;
			}

			p->x_vec = (float)(rand()%500)/1000.0f;
			p->y_vec = 0.5f;

			p->life = p->start_life;
			p->size = Random(p->min_size, p->max_size);
			p->dead = false;
		}

		Vertex* v1 = &vertices[(i+1) * 4 - 1];
		Vertex* v2 = &vertices[(i+1) * 4 - 2];
		Vertex* v3 = &vertices[(i+1) * 4 - 3];
		Vertex* v4 = &vertices[(i+1) * 4 - 4];

		v1->x = p->x_pos - WORLD_X;
		v1->y = p->y_pos - WORLD_Y;

		v2->x = p->x_pos + p->size - WORLD_X;
		v2->y = p->y_pos - WORLD_Y;

		v3->x = p->x_pos + p->size - WORLD_X;
		v3->y = p->y_pos + p->size - WORLD_Y;

		v4->x = p->x_pos - WORLD_X;
		v4->y = p->y_pos + p->size - WORLD_Y;

		Coord* c1 = &coords[(i+1) * 4 - 1];
		Coord* c2 = &coords[(i+1) * 4 - 2];
		Coord* c3 = &coords[(i+1) * 4 - 3];
		Coord* c4 = &coords[(i+1) * 4 - 4];

		c1->x = 0.0f;
		c1->y = 0.0f;
		c2->x = 1.0f;
		c2->y = 0.0f;
		c3->x = 1.0f;
		c3->y = 1.0f;
		c4->x = 0.0f;
		c4->y = 1.0f;

		Color* c = &colors[i];
		c->r = p->r;
		c->g = p->g;
		c->b = p->b;
		c->a = p->a;
	}
}

void ParticlesDraw(unsigned int len)
{
	glPushAttrib(GL_CURRENT_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, particle_texture->tex);
	glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &coords[0]);
	glDrawArrays(GL_QUADS, 0, len);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopAttrib();
}