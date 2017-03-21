#include "player.h"

Player::Player() : yaw(-90.0f), pitch(0.0f), PlayerUpVect(0.0f, 1.0f, 0.0f)
{
	window_properties WindowDimensions = Render_GetWindowProperties();
	PrevCursorPosition.x = (float)WindowDimensions.Width * 0.5f;
	PrevCursorPosition.y = (float)WindowDimensions.Height * 0.5f;
}

Player::~Player()
{}

void Player::UpdateRotation()
{
	v2 CursorPosition = GetOrthoMousePosition();
	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfWidth = (float)WindowDimensions.Width * 0.5f;
	float WindowHalfHeight = (float)WindowDimensions.Height * 0.5f;

	if (CursorPosition.x > WindowHalfWidth)
	{
		CursorPosition.x = WindowHalfWidth;
	}
	else if (CursorPosition.x < -WindowHalfWidth)
	{
		CursorPosition.x = -WindowHalfWidth;
	}
	if (CursorPosition.y > WindowHalfHeight)
	{
		CursorPosition.y = WindowHalfHeight;
	}
	else if (CursorPosition.y < -WindowHalfHeight)
	{
		CursorPosition.y = -WindowHalfHeight;
	}

	CursorOffset = CursorPosition - PrevCursorPosition;
	PrevCursorPosition = CursorPosition;

	float Sensitivity = 0.05f;
	CursorOffset *= Sensitivity;

	yaw += CursorOffset.x;
	pitch += CursorOffset.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	if (Platform_GetStateOfKey('Q') == 1)
	{
		yaw -= 1.0f;
	}
	if (Platform_GetStateOfKey('E') == 1)
	{
		yaw += 1.0f;
	}

	v3 PlayerFront = v3(cos(Radians(yaw)) * cos(Radians(pitch)),
		sin(Radians(pitch)),
		sin(Radians(yaw)) * cos(Radians(pitch)));
	PlayerFrontVect = Normalize(PlayerFront);
}

void Player::UpdateTranslation()
{
	if ((Platform_GetStateOfKey('W') == 1) && (Platform_GetStateOfKey('A') == 1))
	{
		PlayerDirectVect = -Normalize(CrossProduct(PlayerFrontVect, 
			PlayerUpVect)) + (PlayerFrontVect);
	}
	else if ((Platform_GetStateOfKey('W') == 1) && (Platform_GetStateOfKey('D') == 1))
	{
		PlayerDirectVect = Normalize(CrossProduct(PlayerFrontVect,
			PlayerUpVect)) + (PlayerFrontVect);
	}
	else if ((Platform_GetStateOfKey('S') == 1) && (Platform_GetStateOfKey('A') == 1))
	{
		PlayerDirectVect = -Normalize(CrossProduct(PlayerFrontVect,
			PlayerUpVect)) - (PlayerFrontVect);
	}
	else if ((Platform_GetStateOfKey('S') == 1) && (Platform_GetStateOfKey('D') == 1))
	{
		PlayerDirectVect = Normalize(CrossProduct(PlayerFrontVect,
			PlayerUpVect)) - (PlayerFrontVect);
	}
	else if (Platform_GetStateOfKey('A') == 1)
	{
		PlayerDirectVect = -Normalize(CrossProduct(PlayerFrontVect,
			PlayerUpVect));
	}
	else if (Platform_GetStateOfKey('W') == 1)
	{
		PlayerDirectVect = (PlayerFrontVect);
	}
	else if (Platform_GetStateOfKey('S') == 1)
	{
		PlayerDirectVect = -(PlayerFrontVect);
	}
	else if (Platform_GetStateOfKey('D') == 1)
	{
		PlayerDirectVect = Normalize(CrossProduct(PlayerFrontVect,
			PlayerUpVect));
	}
	else
	{
		PlayerDirectVect = v3(0.0f, 0.0f, 0.0f);
	}
}

v3* Player::GetDirection()
{
	return &PlayerDirectVect;
}

v3* Player::GetFrontDirection()
{
	return &PlayerFrontVect;
}