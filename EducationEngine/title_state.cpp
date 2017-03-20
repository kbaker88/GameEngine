#include "title_state.h"

static uint32 EntityBlockNum = 0; //TODO: Remove This later, temporary

TitleState::TitleState() : initialized(0), mouseoverstart(0), NeedToClean(0), ConsoleOn(0)
{
}

TitleState::~TitleState()
{
}

void TitleState::Init(uint8 &programState)
{
	Entity_CreateBlock(EntityBlockNum, 32);

	WindowProperties = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowProperties.Width;
	float HalfScreenHeight = 0.5f * (float)WindowProperties.Height;

	StateOfProgram = &programState;

	TempCamera.SetPosition(&v3(-HalfScreenWidth, -HalfScreenHeight, 1.0f));
	TempCamera.SetProjectionMatrix(0);

	ProgramShaderHandle = Render_CompileShaders(VertexShaderSource,
		FragmentShaderSource);
	TextShaderHandle = Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	float ButtonWidth = 160.0f;
	float ButtonHeight = 40.0f;
	float MenuButtonsXPos = HalfScreenWidth - (ButtonWidth * 0.5f) - 100.0f;
	float MenuButtonsYPos = HalfScreenHeight - (ButtonHeight * 0.5f) - 100.0f;
	v3 ButtonPosition = { MenuButtonsXPos, MenuButtonsYPos, 0.0f };

	uint32 ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 0, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 0, Asset_GetTexture(0));
	//TODO: Remove all seconds and try to do in the shader
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 1, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 1, Asset_GetTexture(1));

	ButtonPosition.y -= ButtonHeight;
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 2, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 2, Asset_GetTexture(2));
	//TODO: Remove all seconds and try to do in the shader
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 3, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 3, Asset_GetTexture(6));

	ButtonPosition.y -= ButtonHeight;
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 4, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 4, Asset_GetTexture(7));
	//TODO: Remove all seconds and try to do in the shader
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 5, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 5, Asset_GetTexture(8));

	initialized = 1;
}

uint8 TitleState::CheckInitialization()
{
	return initialized;
}

void TitleState::Display()
{
	Render_ClearScreen();

	DisplayButtons();
	DisplayConsole();

	if (NeedToClean == 1)
	{
		CleanUp();
	}
}

void TitleState::DisplayButtons()
{
	Render_BindShaders(ProgramShaderHandle);
	ShaderVars[0] = Render_GetShaderVariable(ProgramShaderHandle, "model");
	ShaderVars[1] = Render_GetShaderVariable(ProgramShaderHandle, "view");
	ShaderVars[2] = Render_GetShaderVariable(ProgramShaderHandle, "projection");
	ShaderVars[3] = Render_GetShaderVariable(ProgramShaderHandle, "myTexture");

	Render_UpdateShaderVariable(3, ShaderVars[1], (float*)TempCamera.GetViewMatrix());
	Render_UpdateShaderVariable(3, ShaderVars[2], (float*)TempCamera.GetProjectionMatrix());
	Render_UpdateShaderVariable(1, ShaderVars[3], 0);

	if (Collision_OrthoMouseToRect(Entity_GetPosition(EntityBlockNum, 0),
		Entity_GetCollisionObjPtr(EntityBlockNum, 0)->Width,
		Entity_GetCollisionObjPtr(EntityBlockNum, 0)->Height))
	{
		Entity_Draw(EntityBlockNum, 1, ShaderVars[0]);
		if (Platform_GetMouseState())
		{
			NeedToClean = true;
			*StateOfProgram = 2;
		}
	}
	else
	{
		Entity_Draw(EntityBlockNum, 0, ShaderVars[0]);
	}

	if (Collision_OrthoMouseToRect(Entity_GetPosition(EntityBlockNum, 2), 
		Entity_GetCollisionObjPtr(EntityBlockNum, 2)->Width,
		Entity_GetCollisionObjPtr(EntityBlockNum, 2)->Height))
	{
		Entity_Draw(EntityBlockNum, 3, ShaderVars[0]);
		if (Platform_GetMouseState())
		{
			NeedToClean = true;
			*StateOfProgram = 3;
		}
	}
	else
	{
		Entity_Draw(EntityBlockNum, 2, ShaderVars[0]);
	}

	if (Collision_OrthoMouseToRect(Entity_GetPosition(EntityBlockNum, 4),
		Entity_GetCollisionObjPtr(EntityBlockNum, 4)->Width,
		Entity_GetCollisionObjPtr(EntityBlockNum, 4)->Height))
	{
		Entity_Draw(EntityBlockNum, 5, ShaderVars[0]);
		if (Platform_GetMouseState())
		{
			NeedToClean = true;
			*StateOfProgram = 4;
		}
	}
	else
	{
		Entity_Draw(EntityBlockNum, 4, ShaderVars[0]);
	}

	Platform_UpdateMouseState(0); // TODO : Remove this and fix error

	if (NeedToClean == 1)
	{
		CleanUp();
	}
}

void TitleState::DisplayConsole()
{
	Render_BindShaders(TextShaderHandle);
	ShaderVars[0] = Render_GetShaderVariable(TextShaderHandle, "model");
	ShaderVars[1] = Render_GetShaderVariable(TextShaderHandle, "view");
	ShaderVars[2] = Render_GetShaderVariable(TextShaderHandle, "projection");
	ShaderVars[3] = Render_GetShaderVariable(TextShaderHandle, "myTexture");

	Render_UpdateShaderVariable(3, ShaderVars[1], (float*)TempCamera.GetViewMatrix());
	Render_UpdateShaderVariable(3, ShaderVars[2], (float*)TempCamera.GetProjectionMatrix());
	Render_UpdateShaderVariable(1, ShaderVars[3], 0);

	v3 TextStartPosition = { (float)(-0.5f * WindowProperties.Width) + 20.0f,
		(float)(-0.5f * WindowProperties.Height) + 100.0f, 0.0f };

	Text_GetFromStream();
	Text_DrawStream(TextStartPosition, 0.3f, ShaderVars[0]);

	Text_ClearGlobalStream();
}

void TitleState::CleanUp()
{
	Entity_DeleteBlock(EntityBlockNum);
	Object_ClearAll();
	Render_DeleteShaderProgram(ProgramShaderHandle);
	NeedToClean = 0;
	initialized = 0; 
}