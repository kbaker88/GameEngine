#ifndef CAMERA_H
#define CAMERA_H

#include "linear_algebra.h" // TODO: fix the linkage here. 
#include "render_layer.h"

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

//void Camera_LinkToEntity();
//void Camera_Link(Camera* Camera1, Camera* Camera2);

#endif
