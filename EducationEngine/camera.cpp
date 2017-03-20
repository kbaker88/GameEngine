#include "camera.h"

Camera::Camera()
{
	Position = v3(0.0f, 0.0f, 1.0f);
	UpDirection = v3(0.0f, 1.0f, 0.0f);
	FrontDirection = v3(0.0f, 0.0f, -1.0f);
}

void Camera::SetPosition(v3* NewPosition) //TODO: Make sure this is only called once per frame
{
	Position = *NewPosition;

	ViewMatrix = LookAtMatrix(Position,
		Position + FrontDirection,
		UpDirection);
}

v3* Camera::GetPosition()
{
	return &Position;
}

void Camera::SetFrontDirection(v3* Direction)
{
	FrontDirection = *Direction;
}

v3* Camera::GetFrontDirection()
{
	return &FrontDirection;
}

void Camera::SetUpDirection(v3* Direction)
{
	UpDirection = *Direction;
}

v3* Camera::GetUpDirection()
{
	return &UpDirection;
}

m4* Camera::GetViewMatrix()
{
	return &ViewMatrix;
}

void Camera::SetProjectionMatrix(uint8 Type)
{
	window_properties WindowProperties = Render_GetWindowProperties(); //TODO: Fix this, get it out of here
	switch (Type)
	{
	case 0:
	{
		ProjectionMatrix = OrthographicMarix(0.0f, (float)WindowProperties.Width,
			0.0f, (float)WindowProperties.Height,
			0.1f, 100.0f);
	} break;

	case 1:
	{
		ProjectionMatrix = PerspectiveMatrix(45.0f,
			(float)WindowProperties.Width / (float)WindowProperties.Height, // TODO: calculate the ratio once only on screen size changes and save value.
			0.01f, 1000.0f);
	} break;

	default:
	{

	} break;
	}
}

m4* Camera::GetProjectionMatrix()
{
	return &ProjectionMatrix;
}