#ifndef CAMERA_H
#define CAMERA_H

#include "linear_algebra.h" // TODO: fix the linkage here. 
#include "render_layer.h"

struct Camera
{
	Camera();
	void SetPosition(v3* NewPosition);
	v3* GetPosition();
	void SetFrontDirection(v3* Direction);
	v3* GetFrontDirection();
	void SetUpDirection(v3* Direction);
	v3* GetUpDirection();

	m4* GetViewMatrix();
	void SetProjectionMatrix(uint8 Type);
	m4* GetProjectionMatrix();

private:
	v3 Position;
	v3 UpDirection;
	v3 FrontDirection;
	m4 ViewMatrix, ProjectionMatrix;
	uint32 ID;

};

#endif
