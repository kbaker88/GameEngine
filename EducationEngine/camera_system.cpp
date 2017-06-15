#include "camera_system.h"

void 
Camera_SetPosition(Camera* CameraPtr, v3* NewPosition)
{
	CameraPtr->Position = *NewPosition;

	CameraPtr->ViewMatrix = 
		Math_LookAtMatrix(CameraPtr->Position,
		CameraPtr->Position + CameraPtr->ForwardVector,
		CameraPtr->UpVector);
}

void
Camera_SetDefaultOrtho(Camera* CameraPtr, float Width,
	float Height, v3* Position)
{
	CameraPtr->Yaw = -90.0f;
	CameraPtr->Pitch = 0.0f;
	CameraPtr->UpVector = v3(0.0f, 1.0f, 0.0f);
	CameraPtr->ForwardVector = v3(0.0f, 0.0f, -1.0f);
	CameraPtr->ProjectionMatrix =
		Math_OrthographicMarix(0.0f, Width,
			0.0f, Height, 0.1f, 100.0f);

	Camera_SetPosition(CameraPtr, Position);
}

void
Camera_SetDefaultProjection(Camera* CameraPtr, float Width,
	float Height, v3* Position)
{
	CameraPtr->Yaw = -90.0f;
	CameraPtr->Pitch = 0.0f;
	CameraPtr->UpVector = v3(0.0f, 1.0f, 0.0f);
	CameraPtr->ForwardVector = v3(0.0f, 0.0f, -1.0f);
	CameraPtr->ProjectionMatrix =
		Math_PerspectiveMatrix(45.0f, Width / Height,
			0.01f, 1000.0f);
	Camera_SetPosition(CameraPtr, Position);
}