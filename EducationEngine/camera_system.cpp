#include "camera_system.h"

void 
Camera_SetPosition(Camera* CameraPtr, v3* NewPosition)
{
	CameraPtr->Position = *NewPosition;

	CameraPtr->ViewMatrix = Math_LookAtMatrix(CameraPtr->Position,
		CameraPtr->Position + CameraPtr->ForwardVector,
		CameraPtr->UpVector);
}
