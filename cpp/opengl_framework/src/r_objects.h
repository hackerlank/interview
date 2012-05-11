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

#ifndef __R_OBJECTS_H_
#define __R_OBJECTS_H_

#include "fs_def.h"

#include "fs_texture.h"
#include "fs_font.h"

// object label type

typedef struct {

	font_t* font;		// font, used to draw text over object

	color4b_t color;	// font color

	char* text;			// text (thanks captain)

	uint length;		// text length

} object_label_t;


// object type

typedef struct {

	object_label_t	label;			// label (text over object)

	texture_t*		texture;		//

	vertex3f_t*		vertices;		// vertices of object

	coord2f_t*		coords;			// texture coord of each vertex

	color4b_t*		colors;			// object color

	uint			length;			//

	uint			tag;			// tag of the object.
									// every object has unique tag
									// unique guaranteed by game logic
									// (not renderer's AddObject)

	uint			id;				// unique id (inc every object)

	int				pos_x;			//?
	int				pos_y;			//?
	int				pos_z;			//?

	char			desc[64];		// description (e.g. 'cube')

} object_t;


void R_InitObjectSystem(void);
void R_FreeObjectSystem(void);



// render all objects in the scene

void R_RenderAllObjects(void);



// render specified object

void R_RenderObject(object_t* object);
void R_RenderObject(uint index);



// add object to scene

int R_AddObject(object_t object);



// remove object from scene

void R_RemoveObject(object_t* object);
void R_RemoveObject(uint tag);



// remove all objects from scene

void R_RemoveAllObjects(void);



// get object by id

object_t* R_GetObjectById(int id);



//

void R_DrawObject(object_t* object);














#endif // __R_OBJECT_H_