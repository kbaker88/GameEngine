#include "game_state.h"

v3 LightPosition = { 2.0f, 1.0f, -17.0f }; //TODO: Move into a shader struct?
v3 LightColor = { 1.0f, 1.0f, 1.0f };
static HeightMap HeightMapPlane; //TODO: Move this to entity

static const char* VertShaderForTextureAndLight = "#version 430 core\n"
"layout (location = 0) in vec3 VertexPosition;\n"
"layout (location = 1) in vec3 VertexColor;\n"
"layout (location = 2) in vec3 NormalPosition;\n"
"layout (location = 3) in vec2 TextureCoord;\n"

"out vec3 Color;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec3 LightPos;\n"
"out vec2 TexCoord;\n"

"uniform vec3 lightPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"Color = VertexColor;\n"
"TexCoord = TextureCoord;\n"
"gl_Position = projection * view * model * vec4(VertexPosition, 1.0);\n"
"FragPos = vec3(model * vec4(VertexPosition, 1.0f));\n"
"Normal = mat3(transpose(inverse(view * model))) * NormalPosition;\n"
"LightPos = vec3(view * vec4(lightPos, 1.0));\n"
"}\0";

static const char* FragShaderForTextureAndLight = "#version 430 core\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec3 LightPos;\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"

"out vec4 color;\n"

"uniform vec3 lightColor;\n"
"uniform sampler2D myTexture;\n"
"uniform vec3 viewPos;\n"
"uniform bool isTextured;\n"

"void main()\n"
"{\n"
"float ambientStrength = 0.5f;\n" // Ambient Light
"vec3 ambient = ambientStrength * lightColor;\n" // Ambient Light

"vec3 norm = normalize(Normal);\n" // Diffuse Light
"vec3 lightDir = normalize(LightPos - FragPos);\n" // Diffuse Light
"float diff = max(dot(norm, lightDir), 0.0);\n" // Diffuse Light
"vec3 diffuse = diff * lightColor;\n" // Diffuse Light

"float specularStrength = 1.5f;\n" // Specular Light
"vec3 viewDir = normalize(-FragPos);\n" // Specular Light
"vec3 reflectDir = reflect(-lightDir, norm);\n" // Specular Light
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n" // Specular Light
"vec3 specular = specularStrength * spec * lightColor;\n" // Specular Light
"vec3 result = (ambient + diffuse + specular) * Color;\n"

"if (isTextured)\n"
"{\n"
"color = vec4(result, 1.0f) * texture2D(myTexture, TexCoord);\n"
"}\n"
"else\n"
"{\n"
"color = vec4(result, 1.0f);\n" //* texture2D(myTexture, TexCoord);\n" //vec4(result, 1.0f);\n"
"}\n"
"}\0";

////////////////////////////////////////

static const char* TestVertexShader = "#version 430 core\n"
"layout (location = 0) in vec3 VertexPosition;\n"
"layout (location = 1) in vec3 VertexColor;\n"

"out vec3 Color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"Color = VertexColor;\n"
"gl_Position = projection * view * model * vec4(VertexPosition, 1.0);\n"
"}\0";

static const char* TestFragShader = "#version 430 core\n"
"in vec3 Color;\n"

"out vec4 color;\n"

"void main()\n"
"{\n"
"color = vec4(Color, 1.0f);\n"

"}\n\0";

static const char* TextVertexShaderSource = "#version 430 core\n"
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

static const char* TextFragmentShaderSource = "#version 430 core\n"
	"in vec3 Color;\n"
	"in vec2 TexCoord;\n"
	"out vec4 FragColor;\n"
	"uniform sampler2D myTexture;\n"
	"void main()\n"
	"{\n"
	//"if (texture2D(myTexture, TexCoord).rgb == vec3(1.0,1.0,1.0))\n"
	//	"discard;\n"
	"if (texture2D(myTexture, TexCoord).rgb == vec3(0.0,0.0,0.0))\n"
	"discard;\n"
	"FragColor = texture2D(myTexture, TexCoord) * vec4(0.0, 1.0, 0.0, 1.0);\n" //* vec4(Color, 1.0);\n"
	"}\0";

//GameState::GameState() : initialized(0)
//{
//}
//
//GameState::~GameState()
//{
//}
//
//void GameState::Init(uint8 &programState)
//{
//	StateOfProgram = &programState;
//
//	InitializeWorld();
//
//	UI.Init();
//
//	initialized = 1;
//}

//uint8 GameState::CheckInitialization()
//{
//	return initialized;
//}

//void GameState::Display()
//{
//	Render_ClearScreen();
//
//	DrawWorld();
//	UI.Draw();
//
//	Text_ClearGlobalStream();
//}

//void GameState::CleanUp()
//{
//	WorldCleanUp();
//	initialized = 0;
//}

void Game_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 256);

	State->ShaderHandles[0] = Render_CompileShaders(VertShaderForTextureAndLight,
		FragShaderForTextureAndLight);
	State->ShaderHandles[1] = Render_CompileShaders(TestVertexShader,
		TestFragShader);

	if (!GetPlayer(0)->InitPlayer(State->EntityBlockNum, 0, v3(0.0f, 6.0f, 20.0f)))
	{
		Platform_TemporaryError("\nGetPlayer Problem\n");
	}
	State->CameraArray[0].SetPosition(&v3(0.0f, 6.0f, 20.0f));//TODO: Link in the manager player1camera to player 1
	State->CameraArray[0].SetFrontDirection(&v3(0.0f, 0.0f, -1.0f));
	State->CameraArray[0].SetProjectionMatrix(1);

	Pysc_SetAccelerationRate(Entity_GetPhysObjPtr(State->EntityBlockNum, 
		GetPlayer(0)->PlayerEntityID), 1000.0f);

	//NOTE: Init Terrain
	HeightMapPlane.Init(Asset_GetTexture(15));
	HeightMapPlane.InputTexture(Asset_GetTexture(3));

	//NOTE: Init Objects
	uint32 ObjectID = Object_Load(new Box, 0.25f, 0.25f, 0.25f);
	Entity_Create(State->EntityBlockNum, 1, ObjectID, v3(3.0f, 0.0f, 15.0f));

	ObjectID = Object_Load(new Box, 0.1f, 0.1f, 0.1f);
	Entity_Create(State->EntityBlockNum, 2, ObjectID, v3(2.0f, 1.0f, 17.0f)); // light

	ObjectID = Object_Load(new Plane2D, 10, 10);
	Entity_Create(State->EntityBlockNum, 3, ObjectID, v3(0.0f, -0.5f, 20.0f));
	Entity_AddTexture(State->EntityBlockNum, 3, Asset_GetTexture(11));

	//ActiveWorldChunks = 1;

	//NOTE: User Inferface Below

	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfHeight = 0.5f * (float)WindowDimensions.Height;
	float WindowHalfWidth = 0.5f * (float)WindowDimensions.Width;

	State->CameraArray[1].SetPosition(&v3(-WindowHalfWidth,
		-WindowHalfHeight, 1.0f));
	State->CameraArray[1].SetProjectionMatrix(0);


	State->ShaderHandles[2] = Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	State->TimerArray[0].Start(); // TODO: Remove, testing only.
}

void Game_Draw(ProgramState* State)
{
	Render_ClearScreen();

	void* Player1 = GetPlayer(0);
	static_cast<Player*>(Player1)->UpdateRotation();
	State->CameraArray[0].SetFrontDirection(
		static_cast<Player*>(Player1)->GetFrontDirection());
	static_cast<Player*>(Player1)->UpdateTranslation();

	Phys_SetMoveDirection(Entity_GetPhysObjPtr(State->EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID),
		*static_cast<Player*>(Player1)->GetDirection());
	Phys_CalculatePosition(Entity_GetPhysObjPtr(State->EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID));
	State->CameraArray[0].SetPosition(
		&Entity_GetPosition(State->EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID));

	Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID), &Gravity);

	// Bind New Shaders
	Render_BindShaders(State->ShaderHandles[0]);
	State->GPUShaderVarArray[0] = Render_GetShaderVariable(State->ShaderHandles[0], "model");
	State->GPUShaderVarArray[1] = Render_GetShaderVariable(State->ShaderHandles[0], "view");
	State->GPUShaderVarArray[2] = Render_GetShaderVariable(State->ShaderHandles[0], "projection");
	State->GPUShaderVarArray[3] = Render_GetShaderVariable(State->ShaderHandles[0], "lightColor");
	State->GPUShaderVarArray[4] = Render_GetShaderVariable(State->ShaderHandles[0], "lightPos");
	State->GPUShaderVarArray[5] = Render_GetShaderVariable(State->ShaderHandles[0], "myTexture");
	State->GPUShaderVarArray[6] = Render_GetShaderVariable(State->ShaderHandles[0], "isTextured");

	// Update Shader Variables for Player 1
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[1],
		(float*)State->CameraArray[0].GetViewMatrix());
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[2],
		(float*)State->CameraArray[0].GetProjectionMatrix());

	// Update Shader Variables for World
	Render_UpdateShaderVariable(2, State->GPUShaderVarArray[3], LightColor.Arr);
	Render_UpdateShaderVariable(2, State->GPUShaderVarArray[4], LightPosition.Arr);
	Render_UpdateShaderVariable(1, State->GPUShaderVarArray[5], 0);

	float Choice = 0;
	Render_UpdateShaderVariable(4, State->GPUShaderVarArray[6], &Choice);

	// NOTE: Draw terrain below
	m4 ModelMatrix = IdentityMatrix();
	//void* Player1 = GetPlayer(0);

	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[0],
		(float*)&ModelMatrix);
	HeightMapPlane.DrawIndices();

	v3 Position =
		Entity_GetPosition(State->EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID);

	if ((Position.x < 1.0f) ||
		(Position.x >(float)HeightMapPlane.TerrainMaxWidth - 1) ||
		(Position.z > -1.0f) ||
		(Position.z < -(float)HeightMapPlane.TerrainMaxDepth + 1))
	{
		// outside of terrain map
		if (Position.y < 0)
		{
			Position.y = 0;
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 
				static_cast<Player*>(Player1)->PlayerEntityID), &(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 
				static_cast<Player*>(Player1)->PlayerEntityID), &v3(0.0f,
					-Entity_GetPhysObjPtr(State->EntityBlockNum, 
						static_cast<Player*>(Player1)->PlayerEntityID)->Acceleration.y,
					0.0f));
			Entity_SetPosition(State->EntityBlockNum, 
				static_cast<Player*>(Player1)->PlayerEntityID, Position);
		}
	}
	else
	{
		int32 x, z;
		x = (int32)round(Position.x) - 1; // because negative is backwards?
		z = (int32)round(Position.z);

		// Test Code
		uint32 PlayerPosition = ((x * HeightMapPlane.TerrainMaxDepth) +
			(HeightMapPlane.TerrainMaxDepth - z)) * 3 * sizeof(float);

		HeightMapPlane.UpdateColorVertice(HeightMapPlane.ObjectDescription,
			PlayerPosition,
			v3(0.0f, 0.0f, 1.0f).Arr);
		Render_UnmapShaderDataPtr();

		if (Collision_HeightMap(&HeightMapPlane, Position))
		{
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 
				static_cast<Player*>(Player1)->PlayerEntityID), &(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 
				static_cast<Player*>(Player1)->PlayerEntityID), &v3(0.0f,
					-Entity_GetPhysObjPtr(State->EntityBlockNum, 
						static_cast<Player*>(Player1)->PlayerEntityID)->Acceleration.y, 
					0.0f));
			Entity_SetPosition(State->EntityBlockNum, 
				static_cast<Player*>(Player1)->PlayerEntityID, Position);
		}
	}

	//NOTE: Draw Objects below
	//DrawWorldObjects(State->GPUShaderVarArray[0], State->GPUShaderVarArray[6]);
	//for (uint32 i = 0; i < 4; i++)
	//{
	//	//if (GJK(GetObjectMapPtr(GetPlayer(0)->PlayerMapID), GetObjectMapPtr(ObjectMaps[i])))
	//	//{
	//	//	GetPlayer(100)->RemoveForce();
	//	//	GetPlayer(100)->AddForce(&ReflectVector(GetPlayer(0)->GetDirectionVector(), v3(1.0f, 0.0f, 0.0f)));
	//	//	GetPlayer(100)->ApplyForces();
	//	//}
	//	DrawObjectMap(ObjectMaps[i], ShaderVariableID);
	//}
	Choice = 0.0f;
	for (uint32 Index = 1; Index < 3; Index++) // not textured
	{
		Render_UpdateShaderVariable(4, State->GPUShaderVarArray[6], &Choice);
		Entity_Draw(State->EntityBlockNum, Index, State->GPUShaderVarArray[0]);
	}
	Choice = 1.0f;
	for (uint32 Index = 3; Index < 4; Index++) // textured
	{
		Render_UpdateShaderVariable(4, State->GPUShaderVarArray[6], &Choice);
		Entity_Draw(State->EntityBlockNum, Index, State->GPUShaderVarArray[0]);
	}
	//NOTE: Draw UI Below
	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfHeight = 0.5f * (float)WindowDimensions.Height;
	float WindowHalfWidth = 0.5f * (float)WindowDimensions.Width;
	float Top = WindowHalfHeight;
	float Bottom = -WindowHalfHeight;
	float Left = -WindowHalfWidth;
	float Right = WindowHalfWidth;

	Render_BindShaders(State->ShaderHandles[2]);
	State->GPUShaderVarArray[0] = Render_GetShaderVariable(State->ShaderHandles[2], "model");
	State->GPUShaderVarArray[1] = Render_GetShaderVariable(State->ShaderHandles[2], "view");
	State->GPUShaderVarArray[2] = Render_GetShaderVariable(State->ShaderHandles[2], "projection");
	State->GPUShaderVarArray[3] = Render_GetShaderVariable(State->ShaderHandles[2], "myTexture");

	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[1], &(*State->CameraArray[1].GetViewMatrix()).Rc[0][0]); // TODO: clean this up
	Render_UpdateShaderVariable(3, State->GPUShaderVarArray[2], &(*State->CameraArray[1].GetProjectionMatrix()).Rc[0][0]);
	Render_UpdateShaderVariable(1, State->GPUShaderVarArray[3], 0);

	// TODO: Remove, Test timer and clock features
	Text_DrawCharLine(string("Elapsed Time: ") + 
		string(Platform_FloatToChar(State->TimerArray[0].GetTime(), 1)),
		v3(Right - 200.0f, Top - 40.0f, 0.0f), 0.15f, State->GPUShaderVarArray[0]);

	// Display FPS
	Text_DrawCharLine(string("Frames Per Second: ") +
		string(Platform_FloatToChar(DEBUG_GetFPS(), 1)),
		v3(Right - 200.0f, Top - 20.0f,
			0.0f), 0.15f, State->GPUShaderVarArray[0]);

	v3 PlayerPosition = Entity_GetPosition(State->EntityBlockNum, GetPlayer(0)->PlayerEntityID);

	Text_DrawCharLine(string("Player Position: \0") +
		string(Platform_FloatToChar(PlayerPosition.x)) + string(" ") +
		string(Platform_FloatToChar(PlayerPosition.y)) + string(" ") +
		string(Platform_FloatToChar(PlayerPosition.z)),
		v3(Left + 20.0f, Top - 20.0f, 0.0f), 0.15f, State->GPUShaderVarArray[0]);

	v2 CursorPosition = GetOrthoMousePosition();

	Text_DrawCharLine(string("Cursor Position: \0") +
		string(Platform_FloatToChar(CursorPosition.x)) + string(" ") +
		string(Platform_FloatToChar(CursorPosition.y)),
		v3(Left + 20.0f, Top - 40.0f, 0.0f), 0.15f, State->GPUShaderVarArray[0]);
	//Text_StepDown();
	//Text_DrawCharLine(string("Cursor Position: \0") +
	//	string(Platform_FloatToChar(CursorPosition.x)) + string(" ") +
	//	string(Platform_FloatToChar(CursorPosition.y)));

	//Text_ClearGlobalStream();

	Text_ClearGlobalStream();
}

void Game_Clean(ProgramState* State)
{
	Entity_DeleteBlock(State->EntityBlockNum);
	Object_ClearAll();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
	Render_DeleteShaderProgram(State->ShaderHandles[2]);
}