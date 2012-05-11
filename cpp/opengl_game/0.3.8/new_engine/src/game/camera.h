#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "objects/object.h"

// ����� �������, �� ������� ����� �������������� ������
enum CameraFocusObjectPoint { 
	CamFocusCenter,					// �����
	CamFocusBottomCenter,			// �������� ������� ����
	CamFocusTopCenter,				// �������� �������� ����
	CamFocusLeftCenter,				// �������� ������ ����
	CamFocusRightCenter,			// �������� ������� ����
	CamFocusLeftTopCorner,			// ����� ������� ����
	CamFocusLeftBottomCorner,		// ����� ������ ����
	CamFocusRightTopCorner,			// ������ ������� ����
	CamFocusRightBottomCorner		// ������ ������ ����
};

void CameraMoveToPos(float x, float y);

void CameraUpdatePosition();

void SetCameraAttachedObjectOffset(float x, float y);
void CameraAttachToAxis(bool x, bool y);
void CameraAttachToObject(GameObject* obj);
GameObject* GetCameraAttachedObject();

#endif // __CAMERA_H_