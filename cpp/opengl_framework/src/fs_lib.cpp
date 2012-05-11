//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
// Main game logic.
// 
//
//-----------------------------------------------------------------------------

#include "stdafx.h"


#include "fs_data.h"

#include "fs_lib.h"

#include "r_objects.h"








// creates cube in specified position
//    |
//    |_ _ _ _ size
// (x,y,z)
//  /
//
//  @return int object id

int FS_CreateCube(int ix, int iy, int iz, int isize)
{
	object_t cube;

	ZeroMemory(&cube, sizeof(object_t));

	cube.pos_x = ix;
	cube.pos_y = iy;
	cube.pos_z = iz;

	cube.vertices = new vertex3f_t[4 * 6];

	vertex3f_t* v = cube.vertices;

	float x = (float)ix / 100;
	float y = (float)iy / 100;
	float z = (float)iz / 100;
	float size = (float)isize / 100;

	// back plane

	v[0].x = x;
	v[0].y = y;
	v[0].z = z;
	v[1].x = x;
	v[1].y = y - size;
	v[1].z = z;
	v[2].x = x + size;
	v[2].y = y - size;
	v[2].z = z;
	v[3].x = x + size;
	v[3].y = y;
	v[3].z = z;

	// right plane

	v[4].x = x + size;
	v[4].y = y;
	v[4].z = z;
	v[5].x = x + size;
	v[5].y = y;
	v[5].z = z + size;
	v[6].x = x + size;
	v[6].y = y - size;
	v[6].z = z + size;
	v[7].x = x + size;
	v[7].y = y - size;
	v[7].z = z;

	// front plane

	v[8].x = x;
	v[8].y = y;
	v[8].z = z + size;
	v[9].x = x;
	v[9].y = y - size;
	v[9].z = z + size;
	v[10].x = x + size;
	v[10].y = y - size;
	v[10].z = z + size;
	v[11].x = x + size;
	v[11].y = y;
	v[11].z = z;

	// left plane

	v[12].x = x;
	v[12].y = y;
	v[12].z = z + size;
	v[13].x = x;
	v[13].y = y - size;
	v[13].z = z + size;
	v[14].x = x;
	v[14].y = y - size;
	v[14].z = z;
	v[15].x = x;
	v[15].y = y;
	v[15].z = z;

	// top plane

	v[16].x = x;
	v[16].y = y - size;
	v[16].z = z;
	v[17].x = x + size;
	v[17].y = y - size;
	v[17].z = z;
	v[18].x = x + size;
	v[18].y = y - size;
	v[18].z = z + size;
	v[19].x = x;
	v[19].y = y - size;
	v[19].z = z + size;

	// down plane

	v[20].x = x;
	v[20].y = y;
	v[20].z = z;
	v[21].x = x + size;
	v[21].y = y;
	v[21].z = z;
	v[22].x = x + size;
	v[22].y = y;
	v[22].z = z + size;
	v[23].x = x;
	v[23].y = y;
	v[23].z = z + size;

	cube.length = 4 * 6;

	sprintf(cube.desc, "cube(%d)\0", isize); 

	int id = R_AddObject(cube);

	return id;
}


void FS_SetObjectLabel(int object_id, const font_t* font, const char* text)
{
	object_t* object = R_GetObjectById(object_id);
	
	if(!object)
		return;

	object->label.font = (font_t*)font;
	object->label.text = (char*)text;
	//object->label.color = color;
	object->label.length = strlen(text);

}






#define SECTOR_SIZE 25


void FS_CreateMapFloor(uint width, uint height)
{
	//object_t floor;

	//ZeroMemory(&floor, sizeof(object_t));

	//uint sector_count_x = width / SECTOR_SIZE;
	//uint sector_count_y = height / SECTOR_SIZE;

	//floor.vertices = new vertex3f_t[sector_count_x * sector_count_y * 4];

	//for(uint i = 0; i < sector_count_x; i++)
	//	for(uint j = 0; j < sector_count_y; j++)
	//	{
	//		









}







void fs_CreateSky(void)
{

}





//?

void fs_UpdateSky(void)
{

}