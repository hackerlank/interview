//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Render
//
// DESCRIPTION:
//	Drawing objects
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "r_objects.h"


#include <gl/gl.h>

#include "r_text.h"

#include "m_tools.h"



// this is our pointer to all objects

vector <object_t> objects;

// this is data to render objects in one call
// all vertices, coords, colors, etc..

//vector <vertex3f_t>		vertices;
//vector <coord2f_t>		coords;
//vector <color4b_t>		colors;

vertex3f_t*		vertices = NULL;
coord2f_t*		coords = NULL;
color4b_t*		colors = NULL;


uint	length	=	0;
uint	alloc_length = 0;
uint	object_count = 0;


// cached object (last created)
object_t* last_object		=	NULL;


#define BLOCK_SIZE 512


void R_InitObjectSystem(void)
{
	if(!objects.empty())
		objects.clear();

	object_count = 0;

	if(vertices)
		free(vertices);

	if(coords)
		free(coords);

	if(colors)
		free(colors);

	vertices = NULL;
	coords = NULL;
	colors = NULL;

#ifdef DEBUG_LOGGING
	M_Echo(S_LOG, "Object system initialized");
#endif	

}



void R_FreeObjectSystem(void)
{
	if(!objects.empty())
		objects.clear();

	object_count = 0;

	if(vertices)
		free(vertices);

	if(coords)
		free(coords);

	if(colors)
		free(colors);

	vertices = NULL;
	coords = NULL;
	colors = NULL;

#ifdef DEBUG_LOGGING
	M_Echo(S_LOG, "Object system finalized");
#endif	
}


//void _fill_render_vectors(object_t object)
//{
//	if(object.vertices)
//	{
//		vertices.reserve(vertices.size() + object.length);	
//		memcpy(&vertices[vertices.size()], object.vertices, object.length);
//	}
//
//	if(object.colors)
//	{
//		colors.reserve(colors.size() + object.length);
//		memcpy(&colors[colors.size()], object.colors, object.length);
//	}
//
//	if(object.coords)
//	{
//		coords.reserve(coords.size() + object.length);
//		memcpy(&coords[coords.size()], object.coords, object.length);
//	}
//}



void R_DrawObject(object_t* object)
{
	if(!object)
		return;

	if(!object->vertices)
		return;

	uint required_free_mem = object->length;

	if(alloc_length - length <= required_free_mem)
	{
		vertices = (vertex3f_t*)realloc(vertices, alloc_length + required_free_mem + BLOCK_SIZE);

		if(object->colors)
			colors = (color4b_t*)realloc(colors, alloc_length + required_free_mem + BLOCK_SIZE);
		
		if(object->coords)
			coords = (coord2f_t*)realloc(coords, alloc_length + required_free_mem + BLOCK_SIZE);
	}

	memcpy(&vertices[length], object->vertices, object->length);

	if(object->colors)
		memcpy(&colors[length], object->colors, object->length);

	if(object->coords)
		memcpy(&coords[length], object->coords, object->length);
}





// flush and actually draw all objects in one call

void R_RenderAllObjects(void)
{
	if(length == 0)
		return;

	if(!vertices)
		return;

	glPushAttrib(GL_CURRENT_BIT);


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

	if(colors)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0]);
	}

	if(coords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &coords[0]);
	}

	glDrawArrays(GL_QUADS, 0, length);

	glDisableClientState(GL_VERTEX_ARRAY);


	if(colors)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if(coords)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glPopAttrib();


	for(vector<object_t>::iterator it = objects.begin();
		it != objects.end();
		it++)
	{
		if((*it).label.text)
			R_DrawText(
				(*it).label.text,
				(*it).label.length,
				(*it).label.font,
				(*it).pos_x + 50,
				(*it).pos_y + 50);
	}

}



// render specified object

void R_RenderObject(object_t* object)
{
	// not done
}

void R_RenderObject(uint tag)
{
	// not done
}



/**
	add object to scene

	@return int object id
*/

int R_AddObject(object_t object)
{
	static int __id_counter = 0;

	__id_counter++;

	object.id = __id_counter;

	objects.push_back(object);

	//_fill_render_vectors(object);

	// cache this object

	last_object = &objects[objects.size()-1];

	// finally increment the counters
	
	object_count++;

	length += object.length;

#ifdef DEBUG_LOGGING

	M_Echo(S_LOG,
		"New object: '%s' at (%d, %d, %d) with id %d",
		object.desc,
		object.pos_x,
		object.pos_y,
		object.pos_z,
		object.id);

#endif	

	return object.id;

}



// remove object from scene

void R_RemoveObject(object_t* object)
{
	if(!object)
		return;

	if(last_object)
		if(object->id == last_object->id)
			// we've just removed cached object
			// so set its pointer to NULL
			last_object = NULL;

	uint index = 0;
	for(vector<object_t>::iterator it = objects.begin();
		it != objects.end();
		it++)
	{
		index++;

		object_t* p_obj = &(*it);

		if(p_obj->id = object->id)
			objects.erase(objects.begin() + index);
	}

	object_count--;
}



//void R_RemoveObject(uint index)
//{




// remove all objects from scene

void R_RemoveAllObjects(void)
{
	// clean up render data

	if(vertices)
		free(vertices);
	if(coords)
		free(coords);
	if(colors)
		free(colors);

	vertices = 0;
	coords = 0;
	colors = 0;
	length = 0;

	// clean up objects data

	for(;;)
	{
		if(object_count > 0)
		{
			R_RemoveObject(&objects[object_count-1]);
		}
		else
			break;
	}

	objects.clear();
}




// get object by id

object_t* R_GetObjectById(int id)
{
	// cache check
	if(last_object->id == id)
		return last_object;

	for(uint i = 0; i < object_count; i++)
	{
		object_t* tmp_object = &objects[i];
		if(tmp_object->id == id)
			return tmp_object;
	}

	return NULL;
}
