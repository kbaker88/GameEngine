#include "camera_system.h"

void 
Camera::SetPosition(v3* NewPosition) //TODO: Make sure this is only called once per frame
{
	Position = *NewPosition;

	ViewMatrix = Math_LookAtMatrix(Position,
		Position + FacingVector,
		UpVector);
}

v3*
Camera::GetPosition()
{
	return &Position;
}

void
Camera::SetFrontDirection(v3* Direction)
{
	FacingVector = *Direction;
}

v3* 
Camera::GetFrontDirection()
{
	return &FacingVector;
}

void 
Camera::SetUpDirection(v3* Direction)
{
	UpVector = *Direction;
}

v3* 
Camera::GetUpDirection()
{
	return &UpVector;
}

m4* 
Camera::GetViewMatrix()
{
	return &ViewMatrix;
}

void 
Camera::SetProjectionMatrix(uint8 Type)
{
	window_properties WindowProperties = Render_GetWindowProperties(); //TODO: Fix this, get it out of here
	switch (Type)
	{
	case 0:
	{
		ProjectionMatrix = Math_OrthographicMarix(0.0f, (float)WindowProperties.Width,
			0.0f, (float)WindowProperties.Height,
			0.1f, 100.0f);
	} break;

	case 1:
	{
		ProjectionMatrix = Math_PerspectiveMatrix(45.0f,
			(float)WindowProperties.Width / (float)WindowProperties.Height, // TODO: calculate the ratio once only on screen size changes and save value.
			0.01f, 1000.0f);
	} break;

	default:
	{

	} break;
	}
}

m4*
Camera::GetProjectionMatrix()
{
	return &ProjectionMatrix;
}

//void Camera_Link(Camera* Camera1, Camera* Camera2)
//{
//	Camera2 = Camera1;
//}
