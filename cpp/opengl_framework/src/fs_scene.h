//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
//	Main rendering routines
//	and other related stuff
//-----------------------------------------------------------------------------

#ifndef __FS_SCENE_H_
#define __FS_SCENE_H_

// just clear scene

void FS_ClearScene(void);

// render scene using specified render_param_t struct.
// if NULL - render as default

void FS_RenderScene(render_param_t* r);

// flip buffers and actually show rendered scene to user

void FS_FlushScene(void);

//?

void FS_PrintFPS(void);


// free memory, remove all objects, etc..

void FS_FreeScene(void);





#endif // __FS_SCENE_H_