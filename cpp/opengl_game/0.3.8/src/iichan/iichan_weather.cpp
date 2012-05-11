#include "stdafx.h"
#include "../osl/osl_types.h"
#include "../osl/osl_lexems.h"
#include "../osl/osl_parse.h"
#include "../osl/osl.h"
#include "../osl/osl_misc.h"
#include "../osl/osl_debug.h"
#include "../misc.h"
#include "../config.h"
#include "iichan_weather.h"

vector<Weather> weathers;

extern unsigned int OSL_CURRENT_LINE;

extern int WORLD_MAX_X;
extern int WORLD_MIN_X;

extern config cfg;


void SetResourceField(Weather* w, string field, string value)
{
	if(field == "name")	w->name = osl_RemoveQuotesStr(value);
	else if(field == "particle_texture") w->particle_texture = osl_RemoveQuotesStr(value);
	else if(field == "color_red") w->color_red = atoi(value.c_str());
	else if(field == "color_green") w->color_green = atoi(value.c_str());
	else if(field == "color_blue") w->color_blue = atoi(value.c_str());
	else if(field == "color_alpha") w->color_alpha = atoi(value.c_str());
	else if(field == "life") w->life = (float)atof(value.c_str());
	else if(field == "x_floor_min") w->x_floor_min = (float)atof(value.c_str());
	else if(field == "x_floor_max") w->x_floor_max = (float)atof(value.c_str());
	else if(field == "y_floor_min") w->y_floor_min = (float)atof(value.c_str());
	else if(field == "y_floor_max") w->y_floor_max = (float)atof(value.c_str());
	else if(field == "gravity") w->gravity = (float)atof(value.c_str());
	else if(field == "min_size") w->min_size = (float)atof(value.c_str());
	else if(field == "max_size") w->max_size = (float)atof(value.c_str());
	else if(field == "recreation_type") w->recreation_type = RECREATION_TYPE(atoi(value.c_str()));
	else if(field == "dead_condition") w->dead_condition = DEAD_CONDITION(atoi(value.c_str()));
	else
		WriteToLog("SetResourceField(): unknown field");
}

bool LoadWeather(const char* file_name)
{
	char* str = NULL;
	vector<OSL_LEXEM> lxm;

	bool ok = false;
	int lxm_count = 0;

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Loading weather file: " + string(file_name));
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Input...");
#endif
	if(osl_InputFile(file_name, &str))
		ok = osl_InputString(str, &str);

	if(ok)
	{
#ifdef _DEBUG_OSL
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input OK");
#endif
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input FAILED");
		return false;
	}

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize...");
#endif
	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

	if(ok)
	{
#ifdef _DEBUG_OSL
		char buffer[64];
		sprintf(buffer, "Lexemize OK (%u lexems token)", lxm_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
#endif
		delete[] str;
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize FAILED");
		return false;
	}
	vector <OSL_VAR> pool;
	extern OSL_TEMPLATE_STORAGE storage;
	ok = osl_Compile(&lxm, &pool, &storage);
	if (!ok)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Compile FAILED");
		return false;
	}

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Making resource...");
#endif
	Weather weather;
	ZeroMemory(&weather, sizeof(Weather));

	vector<OSL_LEXEM> sqm;
	for(vector<OSL_LEXEM>::iterator it = lxm.begin();
		it != lxm.end();
		it++)
	{
		if(!osl_IsSys((*it).token))
			sqm.push_back((*it));
		else
			OSL_CURRENT_LINE++;

		if(osl_IsSemicolon((*it).token))
		{
			if(sqm.size() != 4)
			{
				WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadResource() error: invalid line");
			}
			else
			{
				SetResourceField(&weather, sqm[0].token, sqm[2].token);
				sqm.clear();
			}
		}
	}

	weathers.push_back(weather);
#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Resource successfully loaded");
#endif
	return true;
}

Weather* WeatherByName(const char* name)
{
	for(vector<Weather>::iterator it = weathers.begin();
		it != weathers.end();
		it++)
	{
		Weather* t = &(*it);

		if(strcmp(t->name.c_str(), name) == 0)
			return t;
	}
	return NULL;
}

// ----------------------------------------------------------------


// Частицы
Particle* particles = NULL;
unsigned int particles_len = 0;
RECREATION_TYPE recreation_type = rtNo;
DEAD_CONDITION dead_condition = dcNo;

// Текущее состояние погоды
bool weather_on = false;
unsigned int weather_power = 0;

void _clear_particles()
{
	if(particles)
		delete[] particles;	
	particles = NULL;
	particles_len = 0;
}


void _particle_sort(Particle* vec, unsigned int len)
{
	unsigned int i,j;
	Particle temp;

	for(i = len - 1; i > 0; i--)
	{
		for(j = 0; j < i; j++)
		{
			if(vec[j].x_pos > vec[j+1].x_pos)
			{
				temp = vec[j+1];
				vec[j+1] = vec[j];
				vec[j] = temp;
			}
		}
	}
}

void StartWeather(Weather* weather, unsigned int power)
{
	if(!weather)
		return;
	char buffer[128];
	sprintf(buffer, "Starting weather: %s (power: %u)\0", weather->name.c_str(), power);
	WriteToLog(buffer);
	InitParticles();
	
	// Если погода включена
	if(weather_on)
	{
		// очищаем старую погоду
		WriteToLog("Clearing old weather... ");
		_clear_particles();
	}

	WriteToLog("Creating particles...");
	particles = new Particle[power];
	ZeroMemory(particles, power);
	particles_len = power;
	
	// Создаем частицы
	for(unsigned int i = 0; i < power; i++)
	{
		Particle* p = &particles[i];

		p->r = weather->color_red;
		p->g = weather->color_green;
		p->b = weather->color_blue;
		p->a = weather->color_alpha;

		p->life = weather->life;
		p->start_life = weather->life;
		p->min_size = weather->min_size;
		p->max_size = weather->max_size;
		p->size = Random(weather->min_size, weather->max_size);

		// TODO: Задать нормальные координаты и векторы
		p->x_vec = (float)(rand()%500)/1000.0f;
		p->y_vec = (float)(rand()%500)/10000.0f;
		p->x_pos = Random((float)WORLD_MIN_X, (float)WORLD_MAX_X + cfg.scr_width);
		p->y_pos = Random((float)cfg.scr_height);

		p->gravity = weather->gravity;

		p->x_floor_max = weather->x_floor_max;
		p->y_floor_min = weather->x_floor_min;
		p->x_floor_min = weather->y_floor_min;
		p->y_floor_max = weather->y_floor_max;
	}

	//_particle_sort(particles, particles_len);


	weather_on = true;
	weather_power = power;
	recreation_type = weather->recreation_type;
	dead_condition = weather->dead_condition;

	UpdateWeather(0.0f);
}

void UpdateWeather(float tick)
{
	ParticlesTick(particles, particles_len, tick, recreation_type, dead_condition);
}

void DrawWeather()
{
	ParticlesDraw(particles_len);
}

void StopWeather()
{
	WriteToLog("Stopping weather...");
	_clear_particles();
	weather_on = false;
	weather_power = 0;
	FreeParticles();
}

string WeatherToString(IICHAN_WEATHER_TYPE type)
{
	switch(type)
	{
	case wNo: return "No weather";
	case wSnow: return "Snow";
	default: return "Undefined";
	}
}