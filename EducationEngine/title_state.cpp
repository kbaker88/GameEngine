#include "title_state.h"

static GLuint TextureID[3];
static v3 Position;

void 
Title_Initialize(ProgramState* State)
{
	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;
	 
	State->CameraArray[0].SetPosition(&v3(-HalfScreenWidth, 
		-HalfScreenHeight, 1.0f));
	State->CameraArray[0].SetProjectionMatrix(0);
	
#if DATA_ORIENTED
	State->ShaderHandles[0] =
		Render_CompileShaders(
			VertexShader_Title, FragmentShader_Title);

// TODO: Throw this part into an asset system
#if MEMORY_ON

#else
	State->ModelObjBlocks[0].BlockObjects[0] = new Model;
	State->RenderObjBlocks[0].BlockObjects[0] = new RenderObj;
	State->RenderObjBlocks[0].BlockObjects[1] = new RenderObj;
	State->RenderObjBlocks[0].BlockObjects[2] = new RenderObj;
#endif // MEMORY_ON
	ModelObj_CreateRectangle(
		State->ModelObjBlocks[0].BlockObjects[0], 160.0f, 40.0f);

	Position = v3(0.0f, -160.0f, 0.0f);
	Texture2D* Textures[3];
	Textures[0] = Asset_GetTexture(0);
	Textures[1] = Asset_GetTexture(1);
	Textures[2] = Asset_GetTexture(2);

	for (uint32 Index = 0; Index < 3; Index++)
	{
		RenderObj_CreateRenderObject(
			State->RenderObjBlocks[0].BlockObjects[Index],
			State->ModelObjBlocks[0].BlockObjects[0]);
		State->RenderObjBlocks[0].BlockObjects[Index]->NumVertices = 6;

		Position.y -= 42.0f;
		Collision_FillObject(&State->CollisionObj[Index], 160.0f, 40.0f,
			0.0f, &Position);
		State->CollisionObj[Index].CollisionCode = Index * 10;

		Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
			Textures[Index]->Height, Textures[Index]->Data);
	}

#else // NOT DATA_ORIENTED

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

	Text_CreateObj(&State->TextObjArray[0], 0.3f,
		&v3(-60.0f, 3.0f, 0.1f), 16, &State->FontArr[0]);

	State->TimerArray[0].Start();
#endif // DATA_ORIENTED
}

// TODO: Remove this, temporary testing
static bool TempPing = false;

void 
Title_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(1.0f, 1.0f, 1.0f, 1.0f));

#if DATA_ORIENTED
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
		(float*)State->CameraArray[0].GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)State->CameraArray[0].GetProjectionMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3], 0);

	Position = v3(0.0f, -160.0f, 0.0f);
	m4 ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);

	int32 CollisionResult = 0;
	for (uint32 Index = 0; Index < 3; Index++)
	{
		ModelMatrix = Math_TranslateMatrix(ModelMatrix,
			v3(0.0f, -42.0f, 0.0f));
		CollisionResult = Collision_ButtonClick(&State->CursorPosition,
			&State->CollisionObj[Index]);
		Title_CollisionResolve(State, CollisionResult);

		Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
			&ModelMatrix.Rc[0][0], 1, 0);
		Render_BindTexture(TextureID[Index]);
		Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index],
			State->RenderObjBlocks[0].BlockObjects[Index]->NumVertices);
		Render_BindTexture(0);
	}

	if (State->Status == -1)
	{
		Title_Clean(State);
	}
	else
	{
		Platform_UpdateMouseState(0);
	}
#else

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
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3],
		(int32)0);

	Platform_GetCursorPosition(&State->CursorPosition.x,
		&State->CursorPosition.y);

	int32 CollisionResult = 0;
	for (uint32 Index = 0; Index < State->EntityCount; Index++)
	{
		CollisionResult = Collision_ButtonClick(&State->CursorPosition,
			Entity_GetCollisionObjPtr(&State->EntityBlocks[0],
				Index, 0));
		Title_CollisionResolve(State, CollisionResult);

		Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 
			Entity_Ptr(&State->EntityBlocks[0], Index)->State);
		Entity_Draw(&State->EntityBlocks[0], Index,
			State->GPUShaderVarArray[0]);
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
			Render_GetShaderVariable(State->ShaderHandles[1],
				"projection");
		State->GPUShaderVarArray[3] =
			Render_GetShaderVariable(State->ShaderHandles[1], 
				"myTexture");

		Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
			(float*)State->CameraArray[0].GetViewMatrix(), 1, 0);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
			(float*)State->CameraArray[0].GetProjectionMatrix(), 1, 0);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[3], 0);

		window_properties WindowDimensions = 
			Render_GetWindowProperties();
		v3 TextStartPosition = 
		{ -0.5f * (float)WindowDimensions.Width + 10.0f,
			-0.5f * (float)WindowDimensions.Height + 20.0f, 0.0f };

		if (State->TimerArray[0].GetTime() > 0.5f)
		{
			if (TempPing)
			{
				TempPing = false;
			}
			else
			{
				TempPing = true;
			}
			State->TimerArray[0].Start();
		}
		Text_InputBoxUpdate(&State->TextObjArray[0],
			(uint16)State->LastKeyPress, TempPing);
		Text_Draw(&State->TextObjArray[0], 
			State->GPUShaderVarArray[0]);

		if (State->ConsoleState)
		{
			Text_DrawConsole(&TextStartPosition, 0.2f,
				State->GPUShaderVarArray[0], State->FontArr, 
				State->ConsoleGlyph, CONSOLE_BUFFER_LENGTH);
		}

		State->LastKeyPress = 0;
		if (State->Status == -1)
		{
			Title_Clean(State);
		}
		else
		{
			Platform_UpdateMouseState(0);
		}
	}
#endif
}

void 
Title_CollisionResolve(ProgramState* State, 
	int32 CollisionResult)
{
	switch (CollisionResult)
	{
	case 0:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 1.0f, 1.0f, 1.0f);
		Entity_Ptr(&State->EntityBlocks[0], 0)->State = 0;
	} break;
	case 1:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 0.0f, 1.0f, 0.0f);
		Entity_Ptr(&State->EntityBlocks[0], 0)->State = 1;
	} break;
	case 2:
	{
		State->Status = -1;
		*State->StateOfProgram = 2;
	} break;
	case 10:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 1.0f, 1.0f, 1.0f);
		Entity_Ptr(&State->EntityBlocks[0], 1)->State = 0;
	} break;
	case 11:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 0.0f, 1.0f, 0.0f);
		Entity_Ptr(&State->EntityBlocks[0], 1)->State = 1;
	} break;
	case 12:
	{
		State->Status = -1;
		*State->StateOfProgram = 3;
	} break;
	case 20:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 1.0f, 1.0f, 1.0f);
		Entity_Ptr(&State->EntityBlocks[0], 2)->State = 0;
	} break;
	case 21:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 0.0f, 1.0f, 0.0f);
		Entity_Ptr(&State->EntityBlocks[0], 2)->State = 1;
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
		if (State->TextObjArray[0].CollisionResult)
		{
			State->TextObjArray[0].CollisionResult = 0;
		}
		else
		{
			State->TextObjArray[0].CollisionResult = 1;
		}
	} break;
	default: {} break;
	}
}

void
Title_Clean(ProgramState* State)
{
#if DATA_ORIENTED
	RenderObj_DeleteBlock(&State->RenderObjBlocks[0]);
	ModelObj_DeleteBlock(&State->ModelObjBlocks[0]);
#else
	Entity_DeleteBlock(&State->EntityBlocks[0]);
	RenderObj_DeleteBlock(&State->RenderObjBlocks[0]);
	State->ObjectCount = 0;
	State->EntityCount = 0;
#endif
	Platform_UpdateMouseState(0);
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}