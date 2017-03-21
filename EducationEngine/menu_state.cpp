#include "menu_state.h"

const char* VertexShaderSource1 = "#version 430 core\n"
"layout (location = 0) in vec3 VertexPosition;\n"
"layout (location = 1) in vec3 VertexColor;\n"
"layout (location = 2) in vec2 TextureCoord;\n"
"out vec3 Color;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"Color = VertexColor;\n"
"gl_Position = projection * view * model * vec4(VertexPosition, 1.0);\n"
"TexCoord = TextureCoord;\n"
"}\0";

const char* FragmentShaderSource1 = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"void main()\n"
"{\n"
//"FragColor = vec4(Color, 1.0);\n"
"FragColor = texture2D(myTexture, TexCoord);\n" //* vec4(Color, 1.0);\n"
"}\n\0";

////////////////////FOR TEXT ///////////////////////

const char* TextVertexShaderSource1 = "#version 430 core\n"
"layout (location = 0) in vec3 VertexPosition;\n"
"layout (location = 1) in vec3 VertexColor;\n"
"layout (location = 2) in vec2 TextureCoord;\n"
"out vec3 Color;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"Color = VertexColor;\n"
"gl_Position = projection * view * model * vec4(VertexPosition, 1.0);\n"
"TexCoord = TextureCoord;\n"
"}\0";

const char* TextFragmentShaderSource1 = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"void main()\n"
"{\n"
"if (texture2D(myTexture, TexCoord).rgb == vec3(0.0,0.0,0.0))\n"
"discard;\n"
"FragColor = texture2D(myTexture, TexCoord) * vec4(0.0, 1.0, 0.0, 1.0);\n" //* vec4(Color, 1.0);\n"
"}\0";

void Menu_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 32);

	window_properties WindowSize = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowSize.Width;
	float HalfScreenHeight = 0.5f * (float)WindowSize.Height;

	State->CameraArray[0].SetPosition(&v3(-HalfScreenWidth,
		-HalfScreenHeight, 1.0f));
	State->CameraArray[0].SetProjectionMatrix(0);


	State->ShaderHandles[0] = 
		Render_CompileShaders(VertexShaderSource1, FragmentShaderSource1);
	State->ShaderHandles[1] = 
		Render_CompileShaders(TextVertexShaderSource1, TextFragmentShaderSource1);

	float ButtonWidth = 160.0f;
	float ButtonHeight = 40.0f;
	float MenuButtonsXPos = HalfScreenWidth - (ButtonWidth * 0.5f) - 100.0f;
	float MenuButtonsYPos = HalfScreenHeight - (ButtonHeight * 0.5f) - 100.0f;
	v3 ButtonPosition = { MenuButtonsXPos, MenuButtonsYPos, 0.0f };

	uint32 ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 0, ObjectID, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 0, Asset_GetTexture(0));
	//TODO: Remove all seconds and try to do in the shader
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 1, ObjectID, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 1, Asset_GetTexture(1));

	ButtonPosition.y -= ButtonHeight;
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 2, ObjectID, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 2, Asset_GetTexture(9));
	//TODO: Remove all seconds and try to do in the shader
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 3, ObjectID, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 3, Asset_GetTexture(10));

	ButtonPosition.y -= ButtonHeight;
	ObjectID = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 4, ObjectID, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 4, Asset_GetTexture(7));
	//TODO: Remove all seconds and try to do in the shader
	uint32 ObjectID2 = Object_Load(new MyRectangle, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 5, ObjectID2, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 5, Asset_GetTexture(8));

	ButtonPosition.y -= ButtonHeight;
	Entity_Create(State->EntityBlockNum, 6, ObjectID, ButtonPosition);
	Entity_Create(State->EntityBlockNum, 7, ObjectID2, ButtonPosition);
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
	Object_ClearAll();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}