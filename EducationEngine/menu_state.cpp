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
		Render_CompileShaders(VertexShaderSource, FragmentShaderSource);
	State->ShaderHandles[1] = 
		Render_CompileShaders(TextVertexShaderSource, TextFragmentShaderSource);

	float ButtonWidth = 160.0f;
	float ButtonHeight = 40.0f;
	float MenuButtonsXPos = HalfScreenWidth - (ButtonWidth * 0.5f) - 100.0f;
	float MenuButtonsYPos = HalfScreenHeight - (ButtonHeight * 0.5f) - 100.0f;
	v3 ButtonPosition = { MenuButtonsXPos, MenuButtonsYPos, 0.0f };

	Object_Create(new MyRectangle, State->ObjectBlockNum, 0, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 0, State->ObjectBlockNum, 0, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 0, Asset_GetTexture(0));
	//TODO: Remove all seconds and try to do in the shader
	Object_Create(new MyRectangle, State->ObjectBlockNum, 1, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 1, State->ObjectBlockNum, 1, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 1, Asset_GetTexture(1));

	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 2, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 2, State->ObjectBlockNum, 2, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 2, Asset_GetTexture(9));
	//TODO: Remove all seconds and try to do in the shader
	Object_Create(new MyRectangle, State->ObjectBlockNum, 3, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 3, State->ObjectBlockNum, 3, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 3, Asset_GetTexture(10));

	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 4, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 4, State->ObjectBlockNum, 4, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 4, Asset_GetTexture(7));
	//TODO: Remove all seconds and try to do in the shader
	Object_Create(new MyRectangle, State->ObjectBlockNum, 5, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 5, State->ObjectBlockNum, 5, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 5, Asset_GetTexture(8));

	ButtonPosition.y -= ButtonHeight;
	Entity_Create(State->EntityBlockNum, 6, State->ObjectBlockNum, 4, ButtonPosition);
	Entity_Create(State->EntityBlockNum, 7, State->ObjectBlockNum, 5, ButtonPosition);
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

	m4 ModelMatrix = TranslateMatrix(IdentityMatrix(), v3(0.0f, 0.0f, 0.0f));

	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[1],
		(float*)State->CameraArray[0].GetViewMatrix());
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[2],
		(float*)State->CameraArray[0].GetProjectionMatrix());
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[0],
		&ModelMatrix.Rc[0][0]);

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 0),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 0)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 0)->Height))
	{
		Entity_Draw(State->EntityBlockNum, 1, State->GPUShaderVarArray[0]);
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			*State->StateOfProgram = 2;
		}
	}
	else
	{
		Entity_Draw(State->EntityBlockNum, 0, State->GPUShaderVarArray[0]);
	}

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 2),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 2)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 2)->Height))
	{
		Entity_Draw(State->EntityBlockNum, 3, State->GPUShaderVarArray[0]);
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			*State->StateOfProgram = 1;
		}
	}
	else
	{
		Entity_Draw(State->EntityBlockNum, 2, State->GPUShaderVarArray[0]);
	}

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 4),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 4)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 4)->Height))
	{
		Entity_Draw(State->EntityBlockNum, 5, State->GPUShaderVarArray[0]);
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			*State->StateOfProgram = 4;
		}
	}
	else
	{
		Entity_Draw(State->EntityBlockNum, 4, State->GPUShaderVarArray[0]);
	}

	if (Collision_OrthoMouseToRect(Entity_GetPosition(State->EntityBlockNum, 6),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 6)->Width,
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 6)->Height))
	{
		Entity_Draw(State->EntityBlockNum, 7, State->GPUShaderVarArray[0]);
		if (Platform_GetMouseState())
		{
			State->Status = -1;
			Render_UpdateWindow(800, 600);
		}
	}
	else
	{
		Entity_Draw(State->EntityBlockNum, 6, State->GPUShaderVarArray[0]);
	}

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
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}