#include "module_math.h"

void
Module_Math_Load(ProgramState* State)
{
	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;


}

void
Module_Math_Run(ProgramState* State)
{
	Render_ClearScreen(&v4(1.0f, 1.0f, 1.0f, 1.0f));


	if (State->Status == -1)
	{
		// Clean
	}
	else
	{
		Platform_UpdateMouseState(0);
	}
	
}

void
Module_Math_Clean(ProgramState* State)
{
	Platform_UpdateMouseState(0);
	Render_ClearCurrentShaderProgram();
}

int64
Module_Math_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	int64 Result = 0;

	return Result;
}