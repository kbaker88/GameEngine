#include "title_state.h"

void Title_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 32);
	Object_CreateBlock(State->ObjectBlockNum, 32);

	window_properties WindowDimensions = Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;

	State->CameraArray[0].SetPosition(&v3(-HalfScreenWidth, 
		-HalfScreenHeight, 1.0f));
	State->CameraArray[0].SetProjectionMatrix(0);
	
	State->ShaderHandles[0] = Render_CompileShaders(MenuVertexShaderSource, 
		MenuFragmentShaderSource);
	State->ShaderHandles[1] = Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	float ButtonWidth = 160.0f;
	float ButtonHeight = 40.0f;
	float MenuButtonsXPos = HalfScreenWidth - 
		(ButtonWidth * 0.5f) - 100.0f;
	float MenuButtonsYPos = HalfScreenHeight -
		(ButtonHeight * 0.5f) - 100.0f;
	v3 ButtonPosition = { MenuButtonsXPos, MenuButtonsYPos, 0.0f };

	// Start Button
	Object_Create(new MyRectangle, State->ObjectBlockNum, 0, 
		ButtonWidth, ButtonHeight, 0.0f);
	Object_SetTexture(State->ObjectBlockNum, 0, Asset_GetTexture(0));
	Entity_Create(State->EntityBlockNum, 0, State->ObjectBlockNum, 0, 
		&ButtonPosition);
	Entity_GetCollisionObjPtr(State->EntityBlockNum, 0)->CollisionCode = 0;

	// Menu Button
	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 1, 
		ButtonWidth, ButtonHeight, 0.0f);
	Object_SetTexture(State->ObjectBlockNum, 1, Asset_GetTexture(1));
	Entity_Create(State->EntityBlockNum, 1, State->ObjectBlockNum, 1, 
		&ButtonPosition);
	Entity_GetCollisionObjPtr(State->EntityBlockNum, 1)->CollisionCode = 10;

	// Exit Button
	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 2,
		ButtonWidth, ButtonHeight, 0.0f);
	Object_SetTexture(State->ObjectBlockNum, 2, Asset_GetTexture(2));
	Entity_Create(State->EntityBlockNum, 2, State->ObjectBlockNum, 2,
		&ButtonPosition);
	Entity_GetCollisionObjPtr(State->EntityBlockNum, 2)->CollisionCode = 20;
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

	uint32 ButtonCount = 3;
	int CollisionResult = 0;
	for (uint32 i = 0; i < ButtonCount; i++)
	{
		CollisionResult = Collision_ButtonClick(&State->CursorPosition,
			Entity_GetCollisionObjPtr(State->EntityBlockNum, i));
		Title_CollisionResolve(State, CollisionResult);
	}

	if (State->Status == -1)
	{
		Title_Clean(State);
	}

	if (State->Status != -1)
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
		Render_UpdateShaderVariable(1, State->GPUShaderVarArray[3], 0);

		v3 TextStartPosition = { (float)(-0.5f * WindowProperties.Width) + 20.0f,
			(float)(-0.5f * WindowProperties.Height) + 100.0f, 0.0f };

		Text_GetFromStream();
		Text_DrawStream(TextStartPosition, 0.3f, State->GPUShaderVarArray[0]);

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

void Title_CollisionResolve(ProgramState* State, int CollisionResult)
{
	switch (CollisionResult)
	{
	case 0:
	{
		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 0);
		Entity_Draw(State->EntityBlockNum, 0, State->GPUShaderVarArray[0]);
	} break;
	case 1:
	{
		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 1);
		Entity_Draw(State->EntityBlockNum, 0, State->GPUShaderVarArray[0]);
	} break;
	case 2:
	{
		State->Status = -1;
		*State->StateOfProgram = 2;
	} break;
	case 10:
	{
		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 0);
		Entity_Draw(State->EntityBlockNum, 1, State->GPUShaderVarArray[0]);
	} break;
	case 11:
	{
		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 1);
		Entity_Draw(State->EntityBlockNum, 1, State->GPUShaderVarArray[0]);
	} break;
	case 12:
	{
		State->Status = -1;
		*State->StateOfProgram = 3;
	} break;
	case 20:
	{
		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 0);
		Entity_Draw(State->EntityBlockNum, 2, State->GPUShaderVarArray[0]);
	} break;
	case 21:
	{
		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 1);
		Entity_Draw(State->EntityBlockNum, 2, State->GPUShaderVarArray[0]);
	} break;
	case 22:
	{
		State->Status = -1;
		*State->StateOfProgram = 4;
	} break;
	default: {} break;
	}
}

void Title_Clean(ProgramState* State)
{
	Platform_UpdateMouseState(0);
	Entity_DeleteBlock(State->EntityBlockNum);
	Object_DeleteBlock(State->ObjectBlockNum);
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}