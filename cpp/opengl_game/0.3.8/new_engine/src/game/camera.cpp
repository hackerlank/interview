#include "StdAfx.h"

#include "../config.h"

#include "camera.h"

//////////////////////////////////////////////////////////////////////////

extern config cfg;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Положение камеры
float CAMERA_X = 0;
float CAMERA_Y = 0;

// Положение границ экрана
float CAMERA_LEFT = 0;
float CAMERA_RIGHT = 0;
float CAMERA_TOP = 0;
float CAMERA_BOTTOM = 0;

// Это смещенеие системы кординат
float CAMERA_OFF_X = 0;			
float CAMERA_OFF_Y = 0;

float CAMERA_OLD_OFF_X = 0;
//float CAMERA_OLD_OFF_Y = 0;

// Смещение камеры относительно объекта
float CAMERA_OBJ_OFF_X = 0;
float CAMERA_OBJ_OFF_Y = 0;

// Точка объекта, на которую нацелена камера
CameraFocusObjectPoint CAMERA_FOCUS_ON_OBJ_POS = CamFocusCenter;

// Объект, на который нацелена камера
GameObject* attached_object = NULL;

bool camera_attach_x = true;
bool camera_attach_y = true;

//////////////////////////////////////////////////////////////////////////
// Расчеты

__INLINE void CalcCamOffX()
{
	CAMERA_OFF_X = -(CAMERA_X - cfg.scr_width * 0.5f);
	CAMERA_LEFT = -CAMERA_OFF_X;
	CAMERA_RIGHT = CAMERA_LEFT + cfg.scr_width;
}

__INLINE void CalcCamOffY()
{
	CAMERA_OFF_Y = -(CAMERA_Y - cfg.scr_height * 0.5f);
	CAMERA_TOP = -CAMERA_OFF_Y;
	CAMERA_BOTTOM = CAMERA_TOP + cfg.scr_height;
}


//////////////////////////////////////////////////////////////////////////
void CameraAttachToAxis(bool x, bool y)
{
	camera_attach_x = x;
	camera_attach_y = y;
}

// Прицепляет камеру к объекту
// Если передать NULL, то отцеплет ее
void CameraAttachToObject(GameObject* obj)
{
	attached_object = obj;
}

void SetCameraAttachedObjectOffset(float x, float y)
{
	CAMERA_OBJ_OFF_X = x;
	CAMERA_OBJ_OFF_Y = y;

	// Смещения будут действовать, даже если камера следит только за одной из осей
	CAMERA_X = x;
	CAMERA_Y = y;
	CalcCamOffX();
	CalcCamOffY();
}

// Возвращает объект
GameObject* GetCameraAttachedObject() 
{
	return attached_object;
}

// Смещает камеру
// x, y - координаты точки, которая будет в центре экрана
void CameraMoveToPos(float x, float y)
{
	attached_object = NULL;

	CAMERA_X = x;
	CAMERA_Y = y;

	CalcCamOffX();
	CalcCamOffY();
}

// Обновляет положение камеры
void CameraUpdatePosition()
{
	CAMERA_OLD_OFF_X = CAMERA_OFF_X;
	//CAMERA_OLD_OFF_Y = CAMERA_OFF_Y;

	if (attached_object)
	{
		if (camera_attach_x)
		{
			CAMERA_X = attached_object->aabb.p.x - CAMERA_OBJ_OFF_X;
			
			switch (CAMERA_FOCUS_ON_OBJ_POS)
			{
			case CamFocusLeftCenter:
				CAMERA_X -= attached_object->aabb.W;
				break;
			case CamFocusRightCenter:
				CAMERA_X += attached_object->aabb.W;
				break;
			case CamFocusLeftTopCorner:
				CAMERA_X -= attached_object->aabb.W;
				break;
			case CamFocusLeftBottomCorner:
				CAMERA_X -= attached_object->aabb.W;
				break;
			case CamFocusRightTopCorner:
				CAMERA_X += attached_object->aabb.W;
				break;
			case CamFocusRightBottomCorner:
				CAMERA_X += attached_object->aabb.W;
				break;
			}

			CalcCamOffX();
		}

		if (camera_attach_y)
		{
			CAMERA_Y = attached_object->aabb.p.y - CAMERA_OBJ_OFF_Y;

			switch (CAMERA_FOCUS_ON_OBJ_POS)
			{
			case CamFocusBottomCenter:
				CAMERA_Y += attached_object->aabb.H;
				break;
			case CamFocusTopCenter:
				CAMERA_Y -= attached_object->aabb.H;
				break;
			case CamFocusLeftTopCorner:
				CAMERA_Y -= attached_object->aabb.H;
				break;
			case CamFocusLeftBottomCorner:
				CAMERA_Y += attached_object->aabb.H;
				break;
			case CamFocusRightTopCorner:
				CAMERA_Y -= attached_object->aabb.H;
				break;
			case CamFocusRightBottomCorner:
				CAMERA_Y += attached_object->aabb.H;
				break;
			}

			CalcCamOffY();
		}
	}

	// Мы сдвигаем систему координат OpenGL - в экран попадает то, что 
	// мы должны увидеть.
	glTranslatef(CAMERA_OFF_X, CAMERA_OFF_Y, 0);
}