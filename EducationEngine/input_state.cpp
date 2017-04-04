#include "input_state.h"

static Cursor_State CursorState;

void Input_UpdateMouseState(ProgramState* State)
{
	CursorState.Position = GetOrthoMousePosition();
	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfWidth = (float)WindowDimensions.Width * 0.5f;
	float WindowHalfHeight = (float)WindowDimensions.Height * 0.5f;

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

	float* YawPtr = 
		&Entity_GetCamera(State->EntityBlockNum, 0)->Yaw;
	float* PitchPtr = 
		&Entity_GetCamera(State->EntityBlockNum, 0)->Pitch;

	*YawPtr += CursorState.PositionOffset.x;
	*PitchPtr += CursorState.PositionOffset.y;

	if (*PitchPtr > 89.0f)
		*PitchPtr = 89.0f;
	if (*PitchPtr < -89.0f)
		*PitchPtr = -89.0f;

	v3 PlayerFront = v3(Math_Cos(Math_ConverToRadians
		(*YawPtr)) * Math_Cos(Math_ConverToRadians(*PitchPtr)),
		Math_Sin(Math_ConverToRadians(*PitchPtr)),
		Math_Sin(Math_ConverToRadians(*YawPtr)) * Math_Cos(Math_ConverToRadians(*PitchPtr)));
	Entity_GetCamera(State->EntityBlockNum, 0)->FacingVector =
		Math_Normalize(PlayerFront);
}

void Input_UpdateKeyStates(ProgramState* State)
{
	float* YawPtr =
		&Entity_GetCamera(State->EntityBlockNum, 0)->Yaw;
	float* PitchPtr =
		&Entity_GetCamera(State->EntityBlockNum, 0)->Pitch;

	v3* DirectionVector = 
		&Entity_Ptr(State->EntityBlockNum, 0)->DirectionVector;
	v3* FacingVector =
		&Entity_GetCamera(State->EntityBlockNum, 0)->FacingVector;
	v3* RelativeUpVector =
		&Entity_GetCamera(State->EntityBlockNum, 0)->UpVector;
	
	if (Platform_GetStateOfKey('Q') == 1)
	{
		*YawPtr -= 1.0f;
	}
	if (Platform_GetStateOfKey('E') == 1)
	{
		*YawPtr += 1.0f;
	}

	v3 UserFacingVector = 
		v3(Math_Cos(Math_ConverToRadians(*YawPtr)) * Math_Cos(Math_ConverToRadians(*PitchPtr)),
			Math_Sin(Math_ConverToRadians(*PitchPtr)),
			Math_Sin(Math_ConverToRadians(*YawPtr)) * Math_Cos(Math_ConverToRadians(*PitchPtr)));
	*FacingVector = Math_Normalize(UserFacingVector);

	if ((Platform_GetStateOfKey('W') == 1) && 
		(Platform_GetStateOfKey('A') == 1))
	{
		*DirectionVector = -Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) + *FacingVector;
	}
	else if ((Platform_GetStateOfKey('W') == 1) && 
		(Platform_GetStateOfKey('D') == 1))
	{
		*DirectionVector = Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) + *FacingVector;
	}
	else if ((Platform_GetStateOfKey('S') == 1) && 
		(Platform_GetStateOfKey('A') == 1))
	{
		*DirectionVector = -Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) - *FacingVector;
	}
	else if ((Platform_GetStateOfKey('S') == 1) && 
		(Platform_GetStateOfKey('D') == 1))
	{
		*DirectionVector = Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector)) - *FacingVector;
	}
	else if (Platform_GetStateOfKey('A') == 1)
	{
		*DirectionVector = -Math_Normalize(Math_CrossProduct(*FacingVector,
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
		*DirectionVector = Math_Normalize(Math_CrossProduct(*FacingVector,
			*RelativeUpVector));
	}
	else
	{
		*DirectionVector = v3(0.0f, 0.0f, 0.0f);
	}
}