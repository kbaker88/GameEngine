#include "menu_state.h"

void
Menu_Initialize(ProgramState* State)
{
	window_properties WindowSize = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowSize.Width;
	float HalfScreenHeight = 0.5f * (float)WindowSize.Height;


	State->CameraArray[0].Yaw = -90.0f;
	State->CameraArray[0].Pitch = 0.0f;
	State->CameraArray[0].UpVector = v3(0.0f, 1.0f, 0.0f);
	State->CameraArray[0].ForwardVector = v3(0.0f, 0.0f, -1.0f);
	State->CameraArray[0].ProjectionMatrix =
		Math_OrthographicMarix(0.0f, (float)WindowSize.Width,
			0.0f, (float)WindowSize.Height,
			0.1f, 100.0f);

	Camera_SetPosition(&State->CameraArray[0], &v3(-HalfScreenWidth,
		-HalfScreenHeight, 1.0f));
}

void 
Menu_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(1.0f, 1.0f, 1.0f, 1.0f));

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
		Render_GetShaderVariable(State->ShaderHandles[0], "mouseOver");

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)&State->CameraArray[0].ViewMatrix, 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)&State->CameraArray[0].ProjectionMatrix, 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3], (int32)0);

	Platform_GetCursorPosition(&State->CursorPosition.x,
		&State->CursorPosition.y);
}

void 
Menu_CollisionResolve(ProgramState* State, int32 CollisionResult)
{
	switch (CollisionResult)
	{
	case 0:
	{
	} break;
	case 1:
	{
	} break;
	case 2:
	{
		State->Status = -1;
		*State->StateOfProgram = 2;
	} break;
	case 10:
	{
	} break;
	case 11:
	{
	} break;
	case 12:
	{
		State->Status = -1;
		*State->StateOfProgram = 1;
	} break;
	case 20:
	{
	} break;
	case 21:
	{
	} break;
	case 22:
	{
		State->Status = -1;
		*State->StateOfProgram = 4;
	} break;
	case 30:
	{

	} break;
	case 31:
	{

	} break;
	case 32:
	{

	} break;
	default: {} break;
	}
}

void
Menu_Clean(ProgramState* State)
{
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}