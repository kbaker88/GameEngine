#ifndef CAMERA_H
#define CAMERA_H

#include "collision_system.h"

struct Camera
{
	m4 ViewMatrix, ProjectionMatrix;
	v3 Position, UpVector, ForwardVector;
	float Yaw, Pitch;
};

void 
Camera_SetPosition(Camera* CameraPtr, v3* NewPosition);
void
Camera_SetDefaultOrtho(Camera* CameraPtr, float Width,
	float Height, v3* Position);
void
Camera_SetDefaultProjection(Camera* CameraPtr, float Width,
	float Height, v3* Position);
#endif
