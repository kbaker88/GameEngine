#include "module_math.h"

void
Module_Math_Load(ProgramState* State)
{
	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;

	State->CameraArray[0].Yaw = -90.0f;
	State->CameraArray[0].Pitch = 0.0f;
	State->CameraArray[0].UpVector = v3(0.0f, 1.0f, 0.0f);
	State->CameraArray[0].ForwardVector = v3(0.0f, 0.0f, -1.0f);
	State->CameraArray[0].ProjectionMatrix =
		Math_OrthographicMarix(0.0f, (float)WindowDimensions.Width,
			0.0f, (float)WindowDimensions.Height,
			0.1f, 100.0f);

	Camera_SetPosition(&State->CameraArray[0], &v3(-HalfScreenWidth,
		-HalfScreenHeight, 1.0f));

	State->ShaderHandles[0] =
		Render_CompileShaders(
			VertexShader_Title, FragmentShader_Title);
	State->ShaderHandles[1] =
		Render_CompileShaders(TextVertexShaderSource,
			TextFragmentShaderSource);
}

void
Module_Math_Run(ProgramState* State)
{
	Render_ClearScreen(&v4(1.0f, 1.0f, 1.0f, 1.0f));

	Platform_GetCursorPosition(&State->CursorPosition.x,
		&State->CursorPosition.y);

	Render_BindShaders(State->ShaderHandles[0]);
	State->GPUShaderVarArray[0] =
		Render_GetShaderVariable(State->ShaderHandles[0], "model");
	State->GPUShaderVarArray[1] =
		Render_GetShaderVariable(State->ShaderHandles[0], "view");
	State->GPUShaderVarArray[2] =
		Render_GetShaderVariable(State->ShaderHandles[0], "projection");
	State->GPUShaderVarArray[3] =
		Render_GetShaderVariable(State->ShaderHandles[0], "myTexture");
	State->GPUShaderVarArray[4] =
		Render_GetShaderVariable(State->ShaderHandles[0], "HoverColor");

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)&State->CameraArray[0].ViewMatrix, 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)&State->CameraArray[0].ProjectionMatrix, 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3], 0);

	m4 ModelMatrix = Math_IdentityMatrix();



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
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}

int64
Module_Math_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	int64 Result = 0;

	return Result;
}