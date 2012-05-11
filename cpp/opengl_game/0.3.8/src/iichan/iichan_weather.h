#ifndef __IICHAN_WEATHER_H
#define __IICHAN_WEATHER_H_
#include "../draw/particle.h"

enum IICHAN_WEATHER_TYPE { wNo, wSnow };

struct Weather
{
	unsigned char color_red;
	unsigned char color_green;
	unsigned char color_blue;
	unsigned char color_alpha;
	float life;
	float x_floor_min;
	float x_floor_max;
	float y_floor_min;
	float y_floor_max;
	float gravity;
	float min_size;
	float max_size;

	RECREATION_TYPE recreation_type;
	DEAD_CONDITION dead_condition;
	string name;
	string particle_texture;
};

bool LoadWeather(const char* file_name);
Weather* WeatherByName(const char* name);

string WeatherToString(IICHAN_WEATHER_TYPE type);

void StartWeather(Weather* weather, unsigned int power);
void UpdateWeather(float tick);
void DrawWeather();
void StopWeather();

#endif