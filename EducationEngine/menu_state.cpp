#include "menu_state.h"

void Menu_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 32);
	Object_CreateBlock(State->ObjectBlockNum, 32);

	window_properties WindowSize = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowSize.Width;
	float HalfScreenHeight = 0.5f * (float)WindowSize.Height;

	State->CameraArray[0].SetPosition(&v3(-HalfScreenWidth,
		-HalfScreenHeight, 1.0f));
	State->CameraArray[0].SetProjectionMatrix(0);


	State->ShaderHandles[0] = 
		Render_CompileShaders(MenuVertexShaderSource, MenuFragmentShaderSource);
	State->ShaderHandles[1] = 
		Render_CompileShaders(TextVertexShaderSource, TextFragmentShaderSource);

	float ButtonWidth = 160.0f;
	float ButtonHeight = 40.0f;
	float MenuButtonsXPos = HalfScreenWidth - (ButtonWidth * 0.5f) - 100.0f;
	float MenuButtonsYPos = HalfScreenHeight - (ButtonHeight * 0.5f) - 100.0f;
	v3 ButtonPosition = { MenuButtonsXPos, MenuButtonsYPos, 0.0f };

	// Start Button
	Object_Create(new MyRectangle, State->ObjectBlockNum, 0, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 0, State->ObjectBlockNum, 0, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 0, Asset_GetTexture(0));

	// Title Button
	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 1, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 1, State->ObjectBlockNum, 1, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 1, Asset_GetTexture(3));

	// Exit Button
	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 2, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 2, State->ObjectBlockNum, 2, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 2, Asset_GetTexture(2));

	// Exit Button (temp second one)
	ButtonPosition.y -= ButtonHeight;
	Entity_Create(State->EntityBlockNum, 3, State->ObjectBlockNum, 2, ButtonPosition);
}

void Menu_Draw(ProgramState* State)
{
	Render_ClearScreen();

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

	int MouseOver = 0;

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 0),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 0)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 0)->Height))
	{
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			*State->StateOfProgram = 2;
		}
		MouseOver = 1;
	}
	else
	{
		MouseOver = 0;
	}
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], MouseOver);
	Entity_Draw(State->EntityBlockNum, 0, State->GPUShaderVarArray[0]);

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 1),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 1)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 1)->Height))
	{
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			*State->StateOfProgram = 1;
		}
		MouseOver = 1;
	}
	else
	{
		MouseOver = 0;
	}
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], MouseOver);
	Entity_Draw(State->EntityBlockNum, 1, State->GPUShaderVarArray[0]);

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 2),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 2)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 2)->Height))
	{
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			*State->StateOfProgram = 4;
		}
		MouseOver = 1;
	}
	else
	{
		MouseOver = 0;
	}
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], MouseOver);
	Entity_Draw(State->EntityBlockNum, 2, State->GPUShaderVarArray[0]);

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 3),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 3)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 3)->Height))
	{
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			Render_UpdateWindow(800, 600);
		}
		MouseOver = 1;
	}
	else
	{
		MouseOver = 0;
	}
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], MouseOver);
	Entity_Draw(State->EntityBlockNum, 3, State->GPUShaderVarArray[0]);

	Platform_UpdateMouseState(0);

	if (State->Status == -1)
	{
		Menu_Clean(State);
	}
}

void Menu_Clean(ProgramState* State)
{
	Entity_DeleteBlock(State->EntityBlockNum);
	Object_DeleteBlock(State->ObjectBlockNum);
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}