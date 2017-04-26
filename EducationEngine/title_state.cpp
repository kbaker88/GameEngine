#include "title_state.h"

void Title_Initialize(ProgramState* State)
{
	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;

	State->CameraArray[0].SetPosition(&v3(-HalfScreenWidth, 
		-HalfScreenHeight, 1.0f));
	State->CameraArray[0].SetProjectionMatrix(0);
	
	State->ShaderHandles[0] = 
		Render_CompileShaders(MenuVertexShaderSource, 
		MenuFragmentShaderSource);
	State->ShaderHandles[1] = 
		Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	float ButtonWidth = 160.0f;
	float ButtonHeight = 40.0f;

	// NOTE: Start Button
	Utility_CreateButton(State, ButtonWidth, ButtonHeight,
		&v3(0.0f, -120.0f, 0.0f), Asset_GetTexture(0));

	// NOTE: Menu Button
	Utility_CreateButton(State, ButtonWidth, ButtonHeight,
		&v3(0.0f, -165.0f, 0.0f), Asset_GetTexture(1));

	// NOTE: Exit Button
	Utility_CreateButton(State, ButtonWidth, ButtonHeight,
		&v3(HalfScreenWidth - (160.0f * 0.5f), -HalfScreenHeight +
		(50.0f * 0.5f), 0.0f), Asset_GetTexture(2));

	Utility_CreateButton(State, 400, 40, &v3(0.0f, 0.0f, 0.0f),
		Asset_GetTexture(8));
}

void Title_Draw(ProgramState* State)
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
		(float*)State->CameraArray[0].GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)State->CameraArray[0].GetProjectionMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3], (int32)0);

	Platform_GetCursorPosition(&State->CursorPosition.x,
		&State->CursorPosition.y);

	int32 CollisionResult = 0;
	for (uint32 Index = 0; Index < State->EntityCount; Index++)
	{
		CollisionResult = Collision_ButtonClick(&State->CursorPosition,
			Entity_GetCollisionObjPtr(&State->EntityBlocks, Index, 0));
		Title_CollisionResolve(State, CollisionResult);

		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 
			Entity_Ptr(&State->EntityBlocks, Index)->State);
		Entity_Draw(&State->EntityBlocks, Index, State->GPUShaderVarArray[0]);
	}

	if (State->Status == -1)
	{
		Title_Clean(State);
	} 
	else 
	{
		Render_BindShaders(State->ShaderHandles[1]);
		State->GPUShaderVarArray[0] =
			Render_GetShaderVariable(State->ShaderHandles[1], "model");
		State->GPUShaderVarArray[1] =
			Render_GetShaderVariable(State->ShaderHandles[1], "view");
		State->GPUShaderVarArray[2] =
			Render_GetShaderVariable(State->ShaderHandles[1], "projection");
		State->GPUShaderVarArray[3] =
			Render_GetShaderVariable(State->ShaderHandles[1], "myTexture");

		Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
			(float*)State->CameraArray[0].GetViewMatrix(), 1, 0);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
			(float*)State->CameraArray[0].GetProjectionMatrix(), 1, 0);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[3], 0);

		v3 TextStartPosition = { (float)(-0.5f * WindowProperties.Width) + 20.0f,
			(float)(-0.5f * WindowProperties.Height) + 100.0f, 0.0f };

		Text_GetFromStream();
		Text_DrawStream(TextStartPosition, 0.3f, State->GPUShaderVarArray[0], State->Fonts);

		Text_ClearGlobalStream();
		if (State->Status == -1)
		{
			Title_Clean(State);
		}
		else
		{
			Platform_UpdateMouseState(0);
		}
	}
}

void Title_CollisionResolve(ProgramState* State, 
	int32 CollisionResult)
{
	switch (CollisionResult)
	{
	case 0:
	{
		Entity_Ptr(&State->EntityBlocks, 0)->State = 0;
	} break;
	case 1:
	{
		Entity_Ptr(&State->EntityBlocks, 0)->State = 1;
	} break;
	case 2:
	{
		State->Status = -1;
		*State->StateOfProgram = 2;
	} break;
	case 10:
	{
		Entity_Ptr(&State->EntityBlocks, 1)->State = 0;
	} break;
	case 11:
	{
		Entity_Ptr(&State->EntityBlocks, 1)->State = 1;
	} break;
	case 12:
	{
		State->Status = -1;
		*State->StateOfProgram = 3;
	} break;
	case 20:
	{
		Entity_Ptr(&State->EntityBlocks, 2)->State = 0;
	} break;
	case 21:
	{
		Entity_Ptr(&State->EntityBlocks, 2)->State = 1;
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

void Title_Clean(ProgramState* State)
{
	Platform_UpdateMouseState(0);
	Entity_DeleteBlock(&State->EntityBlocks);
	RenderObj_DeleteBlock(&State->ObjectBlocks);
	State->ObjectCount = 0;
	State->EntityCount = 0;
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}