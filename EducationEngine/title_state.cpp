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
		//State->RenderObjBlocks[0].BlockObjects[Index]->NumVertices = 6;

		Position.y -= 42.0f;
		Collision_FillObject(&State->CollisionObj[Index], 160.0f, 40.0f,
			0.0f, &Position);
		State->CollisionObj[Index].CollisionCode = Index * 10;

		Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
			Textures[Index]->Height, Textures[Index]->Data);
	}
}

// TODO: Remove this, temporary testing
static bool TempPing = false;

void 
Title_Draw(ProgramState* State)
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
		Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
		Render_BindTexture(0);
	}

	if (State->Status == -1)
	{
		Title_Clean(State);
	}
	else
	{

		//Render_BindShaders(State->ShaderHandles[1]);
		//State->GPUShaderVarArray[0] =
		//	Render_GetShaderVariable(State->ShaderHandles[1], "model");
		//State->GPUShaderVarArray[1] =
		//	Render_GetShaderVariable(State->ShaderHandles[1], "view");
		//State->GPUShaderVarArray[2] =
		//	Render_GetShaderVariable(State->ShaderHandles[1],
		//		"projection");
		//State->GPUShaderVarArray[3] =
		//	Render_GetShaderVariable(State->ShaderHandles[1],
		//		"myTexture");
		//
		//Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		//	(float*)&State->CameraArray[0].ViewMatrix, 1, 0);
		//Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		//	(float*)&State->CameraArray[0].ProjectionMatrix, 1, 0);
		//Render_UpdateShaderVariable(State->GPUShaderVarArray[3], 0);

		Text_DrawCharLine(string("MOUSE COLLISION \0"),
			v3(20.0f, 10.0f, 0.0f), 1.0f,
			State->GPUShaderVarArray[0], State->FontArr);

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
	} break;
	case 1:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 0.0f, 1.0f, 0.0f);
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
	} break;
	case 11:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 0.0f, 1.0f, 0.0f);
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
	} break;
	case 21:
	{
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[4], 0.0f, 1.0f, 0.0f);
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
	//RenderObj_DeleteBlock(&State->RenderObjBlocks[0]);
	//ModelObj_DeleteBlock(&State->ModelObjBlocks[0]);
#if 0
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