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
//	// ��������, ��� ���������� ������, 
//	// ��������� ���������� dx, dy � pattern_width, pattern_height
//	UINT pattern_dx; 
//	UINT pattern_dy;
//
//} render_data;
//
//typedef struct {
//
//	int range;	// ��������� �����, � ��������
//	int damage; // ����
//
//
//};







//typedef struct {
//    int x;
//    int y;
//
//    char* resource_file; // ��������� �� ���� ��������, ������
//
//    texture_t* texture; // ��������
//
//    object_data data; // ������ �������
//
//    object_handler* handler; // �������-���������� �������
//
//    int id; //
//    int type; // ��� �������, ��� iichan_process_objects
//
//} object_t;

#endif