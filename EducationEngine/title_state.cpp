#include "title_state.h"

// NOTE: Temporary
static GLuint TextureID[3];
Font *ThisFont;
static uint32 TextureIDs[5];
static uint32 FontID;
uint32 AssetID[5];

// TODO: Create asset control for shaders
uint32 ShaderVarArray1[5];
uint32 ShaderVarArray2[5];
// ENDNOTE

void 
Title_Initialize(ProgramState* State)
{
	State->StateID = 0;

	TextureIDs[0] = Asset_LoadBMP("Images/startbutton.bmp"); 
	TextureIDs[1] = Asset_LoadBMP("Images/menubutton.bmp"); 
	TextureIDs[2] = Asset_LoadBMP("Images/exitbutton.bmp"); 
	TextureIDs[3] = Asset_LoadBMP("Images/titlebutton.bmp");
	TextureIDs[4] = Asset_LoadBMP("Images/inputbar.bmp"); 

	FontID = Asset_LoadFont("arial\0",
		"c:/Windows/Fonts/arial.ttf\0");
	Text_SetFont(FontID);
	Text_SetFontSize(0.2f);

	State->NumRenderObjBlocks = 1;
	State->NumModelObjBlocks = 1;

	State_CreateRenderObjectBlocks(State,
		State->NumRenderObjBlocks, 256);
	State_CreateModelObjectBlocks(State,
		State->NumModelObjBlocks, 256);
	State_CreateCollisionObjects(State, 12);
	State_CreateTextObjs(State,
		TEXT_OBJECTS_PER_PROGSTATE);
	State_CreateCameras(State, 1);
	State_CreateShaderVariables(State, 5);
	State_CreateShaderHandles(State, 2);
	State_CreateTimers(State, 2);

	State->Status = 1;

	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;

	Camera_SetDefaultOrtho(&State->CameraArray[0],
		(float)WindowDimensions.Width,
		(float)WindowDimensions.Height,
		&v3(-HalfScreenWidth, -HalfScreenHeight, 1.0f));

	Texture2D* Textures[3];

	for (uint32 Index = 0; Index < 3; Index++)
	{
		Textures[Index] = Asset_GetTexture(TextureIDs[Index]);
		Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
			Textures[Index]->Height, Textures[Index]->Data);
	}
	AssetID[0] = Asset_CreateRect(160.0f, 40.0f);
	AssetID[1] = Asset_CreateRect(200.0f, 60.0f);

	State->ShaderHandles[0] =
		Render_CompileShaders(
			VertexShader_Title, FragmentShader_Title);
	State->ShaderHandles[1] =
		Render_CompileShaders(VertexShader_Text,
			FragmentShader_Text);

	Render_BindShaders(State->ShaderHandles[0]);
	ShaderVarArray1[0] =
		Render_GetShaderVariable(State->ShaderHandles[0], "model");
	ShaderVarArray1[1] =
		Render_GetShaderVariable(State->ShaderHandles[0], "view");
	ShaderVarArray1[2] =
		Render_GetShaderVariable(State->ShaderHandles[0], "projection");
	ShaderVarArray1[3] =
		Render_GetShaderVariable(State->ShaderHandles[0], "myTexture");
	ShaderVarArray1[4] =
		Render_GetShaderVariable(State->ShaderHandles[0], "HoverColor");

	Render_BindShaders(State->ShaderHandles[1]);
	ShaderVarArray2[0] =
		Render_GetShaderVariable(State->ShaderHandles[1], "model");
	ShaderVarArray2[1] =
		Render_GetShaderVariable(State->ShaderHandles[1], "view");
	ShaderVarArray2[2] =
		Render_GetShaderVariable(State->ShaderHandles[1],
			"projection");
	ShaderVarArray2[3] =
		Render_GetShaderVariable(State->ShaderHandles[1],
			"myTexture");
	ShaderVarArray2[4] =
		Render_GetShaderVariable(State->ShaderHandles[1],
			"TextColor");
}

void 
Title_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(1.0f, 1.0f, 1.0f, 1.0f));
	Render_BindShaders(State->ShaderHandles[0]);

	Platform_GetCursorPosition(&State->CursorPosition.x,
		&State->CursorPosition.y);

	Render_UpdateShaderVariable(ShaderVarArray1[1], 44,
		(float*)&State->CameraArray[0].ViewMatrix, 1, 0);
	Render_UpdateShaderVariable(ShaderVarArray1[2], 44,
		(float*)&State->CameraArray[0].ProjectionMatrix, 1, 0);
	Render_UpdateShaderVariable(ShaderVarArray1[3], 0);

	for (uint32 Index = 0; Index < 3; Index++)
	{
		m4 ModelMatrix = *Asset_GetModelMatrix(AssetID[0]);
		ModelMatrix = Math_IdentityMatrix();
		*Asset_GetModelMatrix(AssetID[0]) =
			Math_TranslateMatrix(ModelMatrix,
				v3(0.0f, Index * -42.0f, 0.0f));

		Asset_GetCollisionObj(AssetID[0])->Position =
			v3(0.0f, Index * -42.0f, 0.0f);
		uint32 CollisionResult =
			Collision_ButtonClick(&State->CursorPosition,
			Asset_GetCollisionObj(AssetID[0]));

		float ShaderVarMultiplier = (1 - CollisionResult) * 1.0f;
		Render_UpdateShaderVariable(ShaderVarArray1[4],
			ShaderVarMultiplier, 1.0f, ShaderVarMultiplier);

		if (CollisionResult == 2)
		{
			State->Status = -1;
			State->StateID = Index + 1;
		}

		Render_BindTexture(TextureID[Index]);
		Render_UpdateShaderVariable(ShaderVarArray1[0], 44,
			&Asset_GetModelMatrix(AssetID[0])->Rc[0][0], 1, 0);
		Render_Draw(Asset_GetRenderObj(AssetID[0]));
		Render_BindTexture(0);
	}
	Render_UpdateShaderVariable(ShaderVarArray1[4],
		1.0F, 1.0f, 1.0F);

	m4 ModelMatrix = *Asset_GetModelMatrix(AssetID[1]);
	ModelMatrix = Math_IdentityMatrix();
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, v3(-200.0f,
		0.0f, 0.0f));

	Render_UpdateShaderVariable(ShaderVarArray1[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_Draw(Asset_GetRenderObj(AssetID[1]));

	if (State->Status == -1)
	{
		Title_Clean(State);
	}
	else
	{
		Render_BindShaders(State->ShaderHandles[1]);
		
		Render_UpdateShaderVariable(ShaderVarArray2[1], 44,
			(float*)&State->CameraArray[0].ViewMatrix, 1, 0);
		Render_UpdateShaderVariable(ShaderVarArray2[2], 44,
			(float*)&State->CameraArray[0].ProjectionMatrix, 1, 0);
		Render_UpdateShaderVariable(ShaderVarArray2[3], 0);
		Render_UpdateShaderVariable(ShaderVarArray2[4],
			0.0f, 1.0f, 0.0f);

		Text_SetShaderVarID(ShaderVarArray2[0]);

		Text_DrawCharLine(string("Game\0"),
			v3(-300.0f, 10.0f, 0.1f));

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
Title_Clean(ProgramState* State)
{
	Asset_Delete(TextureIDs[0]);
	Asset_Delete(TextureIDs[1]);
	Asset_Delete(TextureIDs[2]);
	Asset_Delete(TextureIDs[3]);
	Asset_Delete(TextureIDs[4]);
	Asset_DeleteFont(FontID);

	Asset_DeleteObj(AssetID[0]);

	Platform_UpdateMouseState(0);
	Render_ClearCurrentShaderProgram(); 
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);

	ModelObj_DeleteBlock(&State->ModelObjBlocks[0]);
	RenderObj_DeleteBlock(&State->RenderObjBlocks[0]);
	State->NumModelObjBlocks = 0;
	State->NumRenderObjBlocks = 0;
}

int64
Title_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	int64 Result = 0;

	return Result;
}