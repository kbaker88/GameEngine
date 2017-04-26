#include "menu_state.h"

void Menu_Initialize(ProgramState* State)
{
	window_properties WindowSize = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowSize.Width;
	float HalfScreenHeight = 0.5f * (float)WindowSize.Height;

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
	float MenuButtonsXPos = HalfScreenWidth -
		(ButtonWidth * 0.5f) - 100.0f;
	float MenuButtonsYPos = HalfScreenHeight -
		(ButtonHeight * 0.5f) - 100.0f;
	v3 ButtonPosition = { MenuButtonsXPos, MenuButtonsYPos, 0.0f };

	// Start Button
	Utility_CreateButton(State, ButtonWidth, ButtonHeight,
		&ButtonPosition, Asset_GetTexture(0));

	// Title Button
	ButtonPosition.y -= ButtonHeight;
	Utility_CreateButton(State, ButtonWidth, ButtonHeight,
		&ButtonPosition, Asset_GetTexture(3));

	// Exit Button
	ButtonPosition.y -= ButtonHeight;
	Utility_CreateButton(State, ButtonWidth, ButtonHeight,
		&ButtonPosition, Asset_GetTexture(2));

	// Exit Button (temp second one)
	ButtonPosition.y -= ButtonHeight;
	Entity_Create(&State->EntityBlocks, 3, 
		RenderObj_GetObjectPtr(&State->ObjectBlocks, 2),
		&ButtonPosition, 0x111);
}

void Menu_Draw(ProgramState* State)
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
		Menu_CollisionResolve(State, CollisionResult);

		Render_UpdateShaderVariable(State->GPUShaderVarArray[4],
			Entity_Ptr(&State->EntityBlocks, Index)->State);
		Entity_Draw(&State->EntityBlocks, Index, State->GPUShaderVarArray[0]);
	}

	if (State->Status == -1)
	{
		Menu_Clean(State);
	}
	else
	{
		Platform_UpdateMouseState(0);
	}
}

void Menu_CollisionResolve(ProgramState* State, int32 CollisionResult)
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
		*State->StateOfProgram = 1;
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

void Menu_Clean(ProgramState* State)
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