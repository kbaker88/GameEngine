#include "menu_state.h"

// TODO: Temporary, delete this
static uint32 TextureIDs[5];
static uint32 ShaderVars[5];
static uint32 ShaderHandles[2];

static Camera MainCamera;
static v2 CursorPos;

void
Menu_Initialize(ProgramState* State)
{
	State->StateID = 2;

	State->Status = 1;

	window_properties WindowDimensions = 
		Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowDimensions.Width;
	float HalfScreenHeight = 0.5f * (float)WindowDimensions.Height;

	Camera_SetDefaultOrtho(&MainCamera,
		(float)WindowDimensions.Width, 
		(float)WindowDimensions.Height,
		&v3(-HalfScreenWidth, -HalfScreenHeight, 1.0f));

	Render_BindShaders(ShaderHandles[0]);
	ShaderVars[0] =
		Render_GetShaderVariable(ShaderHandles[0], "model");
	ShaderVars[1] =
		Render_GetShaderVariable(ShaderHandles[0], "view");
	ShaderVars[2] =
		Render_GetShaderVariable(ShaderHandles[0], "projection");
	ShaderVars[3] =
		Render_GetShaderVariable(ShaderHandles[0], "myTexture");
	ShaderVars[4] =
		Render_GetShaderVariable(ShaderHandles[0], "mouseOver");
}

void 
Menu_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(1.0f, 1.0f, 1.0f, 1.0f));

	Render_BindShaders(ShaderHandles[0]);

	Render_UpdateShaderVariable(ShaderVars[1], 44,
		(float*)&MainCamera.ViewMatrix, 1, 0);
	Render_UpdateShaderVariable(ShaderVars[2], 44,
		(float*)&MainCamera.ProjectionMatrix, 1, 0);
	Render_UpdateShaderVariable(ShaderVars[3], (int32)0);

	Platform_GetCursorPosition(&CursorPos.x,
		&CursorPos.y);
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
	Render_DeleteShaderProgram(ShaderHandles[0]);
	Render_DeleteShaderProgram(ShaderHandles[1]);
}

int64
Menu_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	int64 Result = 0;

	return Result;
}