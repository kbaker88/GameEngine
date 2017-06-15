#include "menu_state.h"

// TODO: Temporary, delete this
static uint32 TextureIDs[5];

void
Menu_Initialize(ProgramState* State)
{
	State->StateID = 2;

	State_CreateRenderObjectBlocks(State, 1, 256);
	State_CreateModelObjectBlocks(State, 1, 256);
	State_CreateCollisionObjects(State, 12);

	State_CreateTextObjs(State,
		TEXT_OBJECTS_PER_PROGSTATE);
	State_CreateCameras(State, 1);
	State_CreateShaderVariables(State, 5);
	State_CreateShaderHandles(State, 2);

	State->Status = 1;

	window_properties WindowDimensions = 
		Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowDimensions.Width;
	float HalfScreenHeight = 0.5f * (float)WindowDimensions.Height;

	Camera_SetDefaultOrtho(&State->CameraArray[0],
		(float)WindowDimensions.Width, 
		(float)WindowDimensions.Height,
		&v3(-HalfScreenWidth, -HalfScreenHeight, 1.0f));
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
	} break;
	case 10:
	{
	} break;
	case 11:
	{
	} break;
	case 12:
	{
	} break;
	case 20:
	{
	} break;
	case 21:
	{
	} break;
	case 22:
	{
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

int64
Menu_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	int64 Result = 0;

	return Result;
}