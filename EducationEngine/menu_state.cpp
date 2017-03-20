#include "menu_state.h"

static uint32 EntityBlockNum = 0; //TODO: Remove This later, temporary

MenuState::MenuState() : initialized(0), NeedToClean(0)
{
}

MenuState::~MenuState()
{
}

void MenuState::Init(uint8 &programState)
{
	Entity_CreateBlock(EntityBlockNum, 32);

	WindowSize = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowSize.Width;
	float HalfScreenHeight = 0.5f * (float)WindowSize.Height;

	TempCamera.SetPosition(&v3(-HalfScreenWidth, -HalfScreenHeight, 1.0f));
	TempCamera.SetProjectionMatrix(0);

	StateOfProgram = &programState;

	ProgramShaderHandle = Render_CompileShaders(VertexShaderSource,
		FragmentShaderSource);
	TextShaderHandle = Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	Button.Init(200.0f, 200.0f);
	Button.InputTexture(Asset_GetTexture(12));

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
	Entity_AddTexture(EntityBlockNum, 2, Asset_GetTexture(9));
	//TODO: Remove all seconds and try to do in the shader
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 3, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 3, Asset_GetTexture(10));

	ButtonPosition.y -= ButtonHeight;
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 4, ObjectID, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 4, Asset_GetTexture(7));
	//TODO: Remove all seconds and try to do in the shader
	uint32 ObjectID2 = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(EntityBlockNum, 5, ObjectID2, ButtonPosition);
	Entity_AddTexture(EntityBlockNum, 5, Asset_GetTexture(8));

	ButtonPosition.y -= ButtonHeight;
	Entity_Create(EntityBlockNum, 6, ObjectID, ButtonPosition);
	Entity_Create(EntityBlockNum, 7, ObjectID2, ButtonPosition);

	initialized = 1;
}

uint8 MenuState::CheckInitialization()
{
	return initialized;
}

void MenuState::Display()
{

	Render_ClearScreen();

	DisplayButtons();
	DisplayText();

	if (NeedToClean == 1)
	{
		CleanUp();
	}
}

void MenuState::DisplayButtons()
{
	Render_BindShaders(ProgramShaderHandle);
	ShaderVars[0] = Render_GetShaderVariable(ProgramShaderHandle, "model");
	ShaderVars[1] = Render_GetShaderVariable(ProgramShaderHandle, "view");
	ShaderVars[2] = Render_GetShaderVariable(ProgramShaderHandle, "projection");

	m4 ModelMatrix = TranslateMatrix(IdentityMatrix(), v3(0.0f, 0.0f, 0.0f));

	Render_UpdateShaderVariable(3, ShaderVars[1], (float*)TempCamera.GetViewMatrix());
	Render_UpdateShaderVariable(3, ShaderVars[2], (float*)TempCamera.GetProjectionMatrix());
	Render_UpdateShaderVariable(3, ShaderVars[0], &ModelMatrix.Rc[0][0]);

	Button.Draw();

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
			*StateOfProgram = 1;
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

	if (Collision_OrthoMouseToRect(Entity_GetPosition(EntityBlockNum, 6),
		Entity_GetCollisionObjPtr(EntityBlockNum, 6)->Width, 
		Entity_GetCollisionObjPtr(EntityBlockNum, 6)->Height))
	{
		Entity_Draw(EntityBlockNum, 7, ShaderVars[0]);
		if (Platform_GetMouseState())
		{
			NeedToClean = true;
			Render_UpdateWindow(800, 600);
		}
	}
	else
	{
		Entity_Draw(EntityBlockNum, 6, ShaderVars[0]);
	}

	Platform_UpdateMouseState(0);
}

void MenuState::DisplayText()
{
	//BindShaders(TextShaderHandle);
	//UniformShaderVariables[0] = GetShaderVariable(ProgramShaderHandle, "model");
	//UniformShaderVariables[1] = GetShaderVariable(ProgramShaderHandle, "view");
	//UniformShaderVariables[2] = GetShaderVariable(ProgramShaderHandle, "projection");
	//
	//UpdateShaderUniformVariableMatrix(UniformShaderVariables[1], &view.Rc[0][0]);
	//UpdateShaderUniformVariableMatrix(UniformShaderVariables[2], &projection.Rc[0][0]);
	//
	//ClearGlobalTextStream();
}

void MenuState::CleanUp()
{
	Entity_DeleteBlock(EntityBlockNum);
	Object_ClearAll();
	Render_DeleteShaderProgram(ProgramShaderHandle);
	NeedToClean = 0;
	initialized = 0;
}