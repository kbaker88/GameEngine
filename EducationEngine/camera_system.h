#ifndef CAMERA_H
#define CAMERA_H

#include "linear_algebra.h" // TODO: fix the linkage here. 
#include "render_layer.h"

struct Camera
{
	m4 ViewMatrix, ProjectionMatrix;
	v3 Position, UpVector, ForwardVector;
	float Yaw, Pitch;
};

void 
Camera_SetPosition(Camera* CameraPtr, v3* NewPosition);

#endif
