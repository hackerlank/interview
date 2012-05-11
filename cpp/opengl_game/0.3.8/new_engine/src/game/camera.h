#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "objects/object.h"

// Точки объекта, на которых может фокусироваться камера
enum CameraFocusObjectPoint { 
	CamFocusCenter,					// Центр
	CamFocusBottomCenter,			// Середина нижнего края
	CamFocusTopCenter,				// Середина верхнего края
	CamFocusLeftCenter,				// Середина левого края
	CamFocusRightCenter,			// Середина правого края
	CamFocusLeftTopCorner,			// Левый верхний угол
	CamFocusLeftBottomCorner,		// Левый нижний угол
	CamFocusRightTopCorner,			// Правый верхний угол
	CamFocusRightBottomCorner		// Правый нижний угол
};

void CameraMoveToPos(float x, float y);

void CameraUpdatePosition();

void SetCameraAttachedObjectOffset(float x, float y);
void CameraAttachToAxis(bool x, bool y);
void CameraAttachToObject(GameObject* obj);
GameObject* GetCameraAttachedObject();

#endif // __CAMERA_H_