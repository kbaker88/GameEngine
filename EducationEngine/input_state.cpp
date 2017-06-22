#include "input_state.h"

static Cursor_State CursorState;

void 
Input_UpdateMouseState(Camera* PlayerCamera)
{
	//TODO: This MousePosition might be the cause of error in mouse picking
	//TODO: Maybe only do the mouse position calculation once.
	v2 CursorPosition;
	Platform_GetCursorPosition(&CursorPosition.x,
		&CursorPosition.y);

	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float WindowHalfWidth = 
		(float)WindowDimensions.Width * 0.5f;
	float WindowHalfHeight =
		(float)WindowDimensions.Height * 0.5f;
	CursorState.Position.x =
		CursorPosition.x - WindowHalfWidth;
	CursorState.Position.y =
		WindowHalfHeight - CursorPosition.y;

	if (CursorState.Position.x > WindowHalfWidth)
	{
		CursorState.Position.x = WindowHalfWidth;
	}
	else if (CursorState.Position.x < -WindowHalfWidth)
	{
		CursorState.Position.x = -WindowHalfWidth;
	}
	if (CursorState.Position.y > WindowHalfHeight)
	{
		CursorState.Position.y = WindowHalfHeight;
	}
	else if (CursorState.Position.y < -WindowHalfHeight)
	{
		CursorState.Position.y = -WindowHalfHeight;
	}

	CursorState.PositionOffset = CursorState.Position -
		CursorState.PrevPosition;
	CursorState.PrevPosition = CursorState.Position;

	float Sensitivity = 0.05f;
	CursorState.PositionOffset *= Sensitivity;

	float* YawPtr = &PlayerCamera->Yaw;
	float* PitchPtr = &PlayerCamera->Pitch;

	*YawPtr += CursorState.PositionOffset.x;
	*PitchPtr += CursorState.PositionOffset.y;

	if (*PitchPtr > 89.0f)
		*PitchPtr = 89.0f;
	if (*PitchPtr < -89.0f)
		*PitchPtr = -89.0f;

	//TODO: Remove conversion to radians. Make Cos automatically give radians.
	v3 PlayerFront = v3(Math_Cosine(Math_ConvertToRadians
		(*YawPtr)) * Math_Cosine(Math_ConvertToRadians(*PitchPtr)),
		Math_Sine(Math_ConvertToRadians(*PitchPtr)),
		Math_Sine(Math_ConvertToRadians(*YawPtr)) *
		Math_Cosine(Math_ConvertToRadians(*PitchPtr)));

	PlayerCamera->ForwardVector = Math_Normalize(PlayerFront);
}

void 
Input_UpdateKeyStates(Camera* PlayerCamera, 
	v3* Direction)
{
	float* YawPtr = &PlayerCamera->Yaw;
	float* PitchPtr = &PlayerCamera->Pitch;

	v3* DirectionVector = Direction;
	v3* FacingVector = &PlayerCamera->ForwardVector;
	v3* RelativeUpVector = &PlayerCamera->UpVector;
	
	if (Platform_GetStateOfKey('Q') == 1)
	{
		*YawPtr -= 1.0f;
	}
	if (Platform_GetStateOfKey('E') == 1)
	{
		*YawPtr += 1.0f;
	}

	v3 UserFacingVector = 
		v3(Math_Cosine(Math_ConvertToRadians(*YawPtr)) * 
			Math_Cosine(Math_ConvertToRadians(*PitchPtr)),
			Math_Sine(Math_ConvertToRadians(*PitchPtr)),
			Math_Sine(Math_ConvertToRadians(*YawPtr)) *
			Math_Cosine(Math_ConvertToRadians(*PitchPtr)));
	*FacingVector = Math_Normalize(UserFacingVector);

	if ((Platform_GetStateOfKey('W') == 1) && 
		(Platform_GetStateOfKey('A') == 1))
	{
		*DirectionVector =
			-Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) + *FacingVector;
	}
	else if ((Platform_GetStateOfKey('W') == 1) && 
		(Platform_GetStateOfKey('D') == 1))
	{
		*DirectionVector =
			Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) + *FacingVector;
	}
	else if ((Platform_GetStateOfKey('S') == 1) && 
		(Platform_GetStateOfKey('A') == 1))
	{
		*DirectionVector =
			-Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) - *FacingVector;
	}
	else if ((Platform_GetStateOfKey('S') == 1) && 
		(Platform_GetStateOfKey('D') == 1))
	{
		*DirectionVector = 
			Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) - *FacingVector;
	}
	else if (Platform_GetStateOfKey('A') == 1)
	{
		*DirectionVector = 
			-Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector));
	}
	else if (Platform_GetStateOfKey('W') == 1)
	{
		*DirectionVector = *FacingVector;
	}
	else if (Platform_GetStateOfKey('S') == 1)
	{
		*DirectionVector = -*FacingVector;
	}
	else if (Platform_GetStateOfKey('D') == 1)
	{
		*DirectionVector =
			Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector));
	}
	else
	{
		*DirectionVector = v3(0.0f, 0.0f, 0.0f);
	}
}