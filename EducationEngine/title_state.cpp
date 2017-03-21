#include "title_state.h"

//TODO: Move Shaders into a shader file
const char* VertexShaderSource = "#version 430 core\n"
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

const char* FragmentShaderSource = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"uniform bool isClicked;\n"
"void main()\n"
"{\n"
"FragColor = texture2D(myTexture, TexCoord);\n" //* vec4(Color, 1.0);\n"
"}\0";

////////////////////FOR TEXT ///////////////////////

const char* TextVertexShaderSource = "#version 430 core\n"
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

const char* TextFragmentShaderSource = "#version 430 core\n"
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


void Title_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 32);
	Object_CreateBlock(State->ObjectBlockNum, 32);

	window_properties WindowDimensions = Render_GetWindowProperties();
	float HalfScreenWidth = 0.5f * (float)WindowDimensions.Width;
	float HalfScreenHeight = 0.5f * (float)WindowDimensions.Height;

	State->CameraArray[0].SetPosition(&v3(-HalfScreenWidth, -HalfScreenHeight, 1.0f));
	State->CameraArray[0].SetProjectionMatrix(0);
	
	State->ShaderHandles[0] = Render_CompileShaders(VertexShaderSource,
		FragmentShaderSource);
	State->ShaderHandles[1] = Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

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
	Entity_AddTexture(State->EntityBlockNum, 2, Asset_GetTexture(2));
	//TODO: Remove all seconds and try to do in the shader
	Object_Create(new MyRectangle, State->ObjectBlockNum, 3, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 3, State->ObjectBlockNum, 3, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 3, Asset_GetTexture(6));

	ButtonPosition.y -= ButtonHeight;
	Object_Create(new MyRectangle, State->ObjectBlockNum, 4, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 4, State->ObjectBlockNum, 4, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 4, Asset_GetTexture(7));
	//TODO: Remove all seconds and try to do in the shader
	Object_Create(new MyRectangle, State->ObjectBlockNum, 5, ButtonWidth, ButtonHeight, 0.0f);
	Entity_Create(State->EntityBlockNum, 5, State->ObjectBlockNum, 5, ButtonPosition);
	Entity_AddTexture(State->EntityBlockNum, 5, Asset_GetTexture(8));
}

void Title_Draw(ProgramState* State)
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

	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[1],
		(float*)State->CameraArray[0].GetViewMatrix());
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[2], 
		(float*)State->CameraArray[0].GetProjectionMatrix());
	Render_UpdateShaderVariable(1, State->GPUShaderVarArray[3], 0);

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
			*State->StateOfProgram = 3;
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

	Platform_UpdateMouseState(0); // TODO : Remove this and fix error

	if (State->Status == -1)
	{
		Title_Clean(State);
	}

	Render_BindShaders(State->ShaderHandles[1]);
	State->GPUShaderVarArray[0] = Render_GetShaderVariable(State->ShaderHandles[1], "model");
	State->GPUShaderVarArray[1] = Render_GetShaderVariable(State->ShaderHandles[1], "view");
	State->GPUShaderVarArray[2] = Render_GetShaderVariable(State->ShaderHandles[1], "projection");
	State->GPUShaderVarArray[3] = Render_GetShaderVariable(State->ShaderHandles[1], "myTexture");

	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[1], (float*)State->CameraArray[0].GetViewMatrix());
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[2], (float*)State->CameraArray[0].GetProjectionMatrix());
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
}

void Title_Clean(ProgramState* State)
{
	Entity_DeleteBlock(State->EntityBlockNum);
	Object_DeleteBlock(State->ObjectBlockNum);
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}