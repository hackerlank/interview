#ifndef __IICHAN_TYPES_H_
#define __IICHAN_TYPES_H_

typedef unsigned int uint;


typedef enum {
	S_NULL		=	0,
	S_LOG		=	1,
	S_SCREEN	=	2,
	S_CONSOLE	=	4,
	S_STDERR	=	8

} stream_t;

typedef struct {

	int x;
	int y;

} coord_2i;








//typedef struct {
//	
//	UINT pattern_width;
//	UINT pattern_height;
//
//	UINT pattern_count;
//
//	UINT image_width;
//	UINT image_height;
//
//	UINT current_pattern;
//
//	// смещение, для спрайтшита тайлов, 
//	// указывает конкретное dx, dy и pattern_width, pattern_height
//	UINT pattern_dx; 
//	UINT pattern_dy;
//
//} render_data;
//
//typedef struct {
//
//	int range;	// дальность удара, в пикселях
//	int damage; // урон
//
//
//};







//typedef struct {
//    int x;
//    int y;
//
//    char* resource_file; // указатель на файл описания, ресурс
//
//    texture_t* texture; // текстура
//
//    object_data data; // данные объекта
//
//    object_handler* handler; // функция-обработчик объекта
//
//    int id; //
//    int type; // тип объекта, для iichan_process_objects
//
//} object_t;

#endif