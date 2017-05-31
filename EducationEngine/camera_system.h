#ifndef CAMERA_H
#define CAMERA_H

#include "linear_algebra.h" // TODO: fix the linkage here. 
#include "render_layer.h"

#if DATA_ORIENTED	

struct Camera
{
	m4 ViewMatrix, ProjectionMatrix;
	v3 Position, UpVector, ForwardVector;
	float Yaw, Pitch;
};

void 
Camera_SetPosition(Camera* CameraPtr, v3* NewPosition);

#else
struct Camera
{
	Camera() : Yaw(-90.0f), Pitch(0.0f), Position(0.0f, 0.0f, 0.0f),
		UpVector(0.0f, 1.0f, 0.0f), FacingVector(0.0f, 0.0f, -1.0f),
		DirectionVector(0.0f, 0.0f, 0.0f) {}

	~Camera() {}

	void SetPosition(v3* NewPosition);
	v3* GetPosition();
	void SetFrontDirection(v3* Direction);
	v3* GetFrontDirection();
	void SetUpDirection(v3* Direction);
	v3* GetUpDirection();

	m4* GetViewMatrix();
	void SetProjectionMatrix(uint8 Type);
	m4* GetProjectionMatrix();

	float Yaw, Pitch;
	v3 Position, UpVector, FacingVector, DirectionVector;
	m4 ViewMatrix, ProjectionMatrix;
	uint32 ID;

};
#endif // DATA_ORIENTED

#endif
