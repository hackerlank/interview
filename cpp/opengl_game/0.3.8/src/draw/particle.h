#ifndef __PARTICLE_H_
#define __PARTICLE_H_
#include "vertex.h"

struct Particle {
	float x_pos;
	float y_pos;

	float min_size;
	float max_size;
	float size;

	float x_vec;
	float y_vec;

	float x_floor_min;
	float x_floor_max;
	float y_floor_min;
	float y_floor_max;

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	float life;
	float start_life;
	float gravity;
	bool dead;
	
};

enum RECREATION_TYPE { rtNo, rtScreenTop, rtScreenRandom, rtWorldRandom, rtWorldTop };
enum DEAD_CONDITION { dcNo, dcLifeZero, dcOutOfScreen };

void InitParticles();
void FreeParticles();
void ParticlesTick(Particle* vec, unsigned int len, float tick, RECREATION_TYPE rt, DEAD_CONDITION dc);
void ParticlesDraw(unsigned int len);
#endif

