#include "game_state.h"

static GLuint TextureID[1]; 
static v3 Position, HeightMapPos;
static v3 PlayerCameraFacingVect;
static PhysicsObject PlayerPhysObj;
static m4 PlayerMatrix;
static CollisionObject PlayerCollObj;
static m4 BoxA, BoxB;
static v3 BoxAPos, BoxBPos;
static AABB BoxAColl, BoxBColl;

// TODO: Temporary
static uint32 TextureIDs[4];

static Model* Models[3];
static RenderObject* RenderObjs[3];

static uint32 ShaderVars[5];
static uint32 ShaderHandles[2];

static Camera MainCamera;
static CollisionObject CollisionObjs[8];
static PhysicsObject PhysicsObjs[8];

void 
Game_Initialize(ProgramState* State)
{
	State->StateID = 1;
	TextureIDs[0] = Asset_LoadBMP("Images/grass2.bmp"); 
	TextureIDs[1] = Asset_LoadBMP("Images/container.bmp"); 
	TextureIDs[2] = Asset_LoadBMP("Images/woodfloor.bmp"); 
	TextureIDs[3] = Asset_LoadPNG("Images/StartHeightMap.png"); 

	State->Status = 1;

#if MEMORY_ON

#else
	Models[0] = new Model;
	RenderObjs[0] = new RenderObject;

	Models[1] = new Model;
	RenderObjs[1] = new RenderObject;

	Models[2] = new Model;
	RenderObjs[2] = new RenderObject;
#endif // MEMORY_ON

	window_properties WindowDimensions = 
		Render_GetWindowProperties();

	Camera_SetDefaultProjection(&MainCamera,
		(float)WindowDimensions.Width,
		(float)WindowDimensions.Height,
		&v3(0.0f, 0.0f, 0.0f));

	ShaderHandles[0] =
		Render_CompileShaders(VertexShader_Game,
			FragmentShader_Game);

	ModelObj_CreateBox(Models[0], 1.0f, 1.0f, 1.0f);
	ModelObj_CreatePlane(Models[1], 20, 20);
	ModelObj_CreateHeightmap( Models[2],
		Asset_GetTexture(TextureIDs[3]));
	
	Position = v3(0.0f, 0.0f, 0.0f);
	Texture2D* Textures[2];
	Textures[0] = Asset_GetTexture(TextureIDs[1]);
	Textures[1] = Asset_GetTexture(TextureIDs[0]);

	// NOTE: Box
	uint32 Index = 0;
	RenderObj_CreateRenderObject(
		RenderObjs[Index], Models[Index]);
	Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
		Textures[Index]->Height, Textures[Index]->Data);
	
	// NOTE: Grass
	Index = 1;
	RenderObj_CreateRenderObject(
		RenderObjs[Index], Models[Index]);
	Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
		Textures[Index]->Height, Textures[Index]->Data);

	// NOTE: HeightMap
	Index = 2;
	RenderObj_CreateRenderObject(
		RenderObjs[Index], Models[Index]);
	HeightMapPos = v3(20.0f, 0.0f, -20.0f);
	Collision_FillObject(&CollisionObjs[Index],
		(float)Asset_GetTexture(TextureIDs[3])->Width, 0.0f,
		(float)Asset_GetTexture(TextureIDs[3])->Height, 
		&HeightMapPos);
	CollisionObjs[Index].VerticesPtr =
		Models[Index]->Data[0];

	Index = 3;

	PlayerMatrix = Math_IdentityMatrix();
	PlayerPhysObj.AccelerationRate = 10.0f;
	PlayerPhysObj.Position = v3(2.0f, 3.0f, -2.0f);
	PlayerPhysObj.ModelMatrix = &PlayerMatrix;

	Collision_FillObject(&PlayerCollObj, 0.5f, 0.5f, 0.5f,
		&PlayerPhysObj.Position);

	BoxAPos = v3(1.0f, 0.5f, -10.0);
	BoxBPos = v3(6.0f, 0.5f, -10.0f);
	PhysicsObjs[0].AccelerationRate = 10.0f;
	PhysicsObjs[0].Position = BoxAPos;
	PhysicsObjs[0].ModelMatrix = &BoxA;
	Collision_FillObject(&CollisionObjs[0], 1.0f, 1.0f, 1.0f,
		&BoxAPos);
	CollisionObjs[0].VerticesPtr = Models[0]->Data[0];
	CollisionObjs[0].NumVertices = Models[0]->NumVertices;

	PhysicsObjs[1].AccelerationRate = 10.0f;
	PhysicsObjs[1].Position = BoxBPos;
	PhysicsObjs[1].ModelMatrix = &BoxB;
	Collision_FillObject(&CollisionObjs[1], 1.0f, 1.0f, 1.0f,
		&BoxBPos);
	CollisionObjs[1].VerticesPtr = Models[0]->Data[0];
	CollisionObjs[1].NumVertices = Models[0]->NumVertices;

	Phys_AddForce(&PlayerPhysObj, &Gravity);
	Phys_AddForce(&PhysicsObjs[0], &v3(1.0f, 0.0f, 0.0f));
	Phys_AddForce(&PhysicsObjs[1], &v3(-1.0f, 0.0f, 0.0f));

	BoxAColl.Height = 1.0f;
	BoxAColl.Width = 1.0f;
	BoxAColl.Depth = 1.0f;
	BoxAColl.Position = BoxAPos;

	BoxBColl.Height = 1.0f;
	BoxBColl.Width = 1.0f;
	BoxBColl.Depth = 1.0f;
	BoxBColl.Position = BoxBPos;

	Render_BindShaders(ShaderHandles[0]);
	ShaderVars[0] = Render_GetShaderVariable(ShaderHandles[0],
			"model");
	ShaderVars[1] = Render_GetShaderVariable(ShaderHandles[0],
			"view");
	ShaderVars[2] = Render_GetShaderVariable(ShaderHandles[0],
			"projection");
	ShaderVars[3] = Render_GetShaderVariable(ShaderHandles[0],
			"myTexture");
	ShaderVars[4] = Render_GetShaderVariable(ShaderHandles[0],
			"TextureFlag");
}

void 
Game_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(0.2f, 0.3f, 0.3f, 1.0f));

	Input_UpdateMouseState(&MainCamera);
	Input_UpdateKeyStates(&MainCamera,
		&PlayerPhysObj.MoveDirection);
	Phys_CalculatePosition(&PlayerPhysObj);
	Camera_SetPosition(&MainCamera,
		&PlayerPhysObj.Position);

	Render_BindShaders(ShaderHandles[0]);

	Render_UpdateShaderVariable(ShaderVars[1], 44,
		(float*)&MainCamera.ViewMatrix, 1, 0);
	Render_UpdateShaderVariable(ShaderVars[2], 44,
		(float*)&MainCamera.ProjectionMatrix, 1, 0);
	Render_UpdateShaderVariable(ShaderVars[4], 1.0f);


	m4 ModelMatrix = Math_IdentityMatrix();

	// NOTE: Box
	uint32 Index = 0;
	ModelMatrix = Math_TranslateMatrix(&ModelMatrix, &Position);
	Render_UpdateShaderVariable(ShaderVars[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_BindTexture(TextureID[Index]);
	Render_Draw(RenderObjs[Index]);

	Phys_CalculatePosition(&PhysicsObjs[0]);
	Phys_CalculatePosition(&PhysicsObjs[1]);
	CollisionObjs[0].Position = PhysicsObjs[0].Position;
	CollisionObjs[1].Position = PhysicsObjs[1].Position;

#if 0
	if (Collision_GJK(&State->CollisionObj[0], 
		&State->CollisionObj[1]))
	{
		v3 Normal = Collision_EPA(&State->CollisionObj[0],
			&State->CollisionObj[1]);

		Phys_BounceBack(&State->PhysicsObj[0], 
			&-Normal);
		Phys_BounceBack(&State->PhysicsObj[1],
			&Normal);
		State->CollisionObj[0].Position =
			State->PhysicsObj[0].Position;
		State->CollisionObj[1].Position =
			State->PhysicsObj[1].Position;

		BoxAPos = State->PhysicsObj[0].Position;
		BoxA = Math_IdentityMatrix();
		BoxA = Math_TranslateMatrix(&BoxA, &BoxAPos);
		Render_UpdateShaderVariable(
			ShaderVars[0], 44, &BoxA.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);

		BoxBPos = State->PhysicsObj[1].Position;
		BoxB = Math_IdentityMatrix();
		BoxB = Math_TranslateMatrix(&BoxB, &BoxBPos);
		Render_UpdateShaderVariable(ShaderVars[0], 44,
			&BoxB.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);
	}
	else
	{
		//Phys_CalculatePosition(&State->PhysicsObj[0]);
		BoxAPos = State->PhysicsObj[0].Position;
		BoxA = Math_IdentityMatrix();
		BoxA = Math_TranslateMatrix(&BoxA, &BoxAPos);
		Render_UpdateShaderVariable(ShaderVars[0], 44,
			&BoxA.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);

		//Phys_CalculatePosition(&State->PhysicsObj[1]);
		BoxBPos = State->PhysicsObj[1].Position;
		BoxB = Math_IdentityMatrix();
		BoxB = Math_TranslateMatrix(&BoxB, &BoxBPos);
		Render_UpdateShaderVariable(ShaderVars[0], 44,
			&BoxB.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);
	}
	Render_BindTexture(0);
#else
	BoxAColl.Position = PhysicsObjs[0].Position;
	BoxBColl.Position = PhysicsObjs[1].Position;

	v3 TestNormal(1.0f, 0.0f, 0.0f);
	if (Collision_AABB(&BoxAColl, &BoxBColl))
	{
		Phys_BounceBack(&PhysicsObjs[0],
			&-TestNormal);
		Phys_BounceBack(&PhysicsObjs[1],
			&TestNormal);
		BoxAColl.Position = PhysicsObjs[0].Position;
		BoxBColl.Position = PhysicsObjs[1].Position;

		BoxAPos = PhysicsObjs[0].Position;
		BoxA = Math_IdentityMatrix();
		BoxA = Math_TranslateMatrix(&BoxA, &BoxAPos);
		Render_UpdateShaderVariable(
			ShaderVars[0], 44, &BoxA.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);

		BoxBPos = PhysicsObjs[1].Position;
		BoxB = Math_IdentityMatrix();
		BoxB = Math_TranslateMatrix(&BoxB, &BoxBPos);
		Render_UpdateShaderVariable(ShaderVars[0], 44,
			&BoxB.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);
	}
	else
	{
		BoxAPos = PhysicsObjs[0].Position;
		BoxA = Math_IdentityMatrix();
		BoxA = Math_TranslateMatrix(&BoxA, &BoxAPos);
		Render_UpdateShaderVariable( ShaderVars[0], 44,
			&BoxA.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);

		BoxBPos = PhysicsObjs[1].Position;
		BoxB = Math_IdentityMatrix();
		BoxB = Math_TranslateMatrix(&BoxB, &BoxBPos);
		Render_UpdateShaderVariable(ShaderVars[0], 44,
			&BoxB.Rc[0][0], 1, 0);
		Render_Draw(RenderObjs[Index]);
	}
#endif
	// NOTE: Grass
	Index = 1;
	Render_UpdateShaderVariable(ShaderVars[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_BindTexture(TextureID[Index]);
	Render_Draw(RenderObjs[Index]);
	Render_BindTexture(0);

	// NOTE: Heightmap
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Index = 2;

	PlayerCollObj.Position = PlayerPhysObj.Position;
	if (Collision_HeightMap(&CollisionObjs[Index],
		Models[2]->Data[3],
		&PlayerCollObj))
	{
		Phys_WallSlide(&PlayerPhysObj, 
			&CollisionObjs[Index].CollideNormal);
		Camera_SetPosition(&MainCamera,
			&PlayerPhysObj.Position);
	}

	ModelMatrix = Math_TranslateMatrix(&ModelMatrix, &HeightMapPos);
	Render_UpdateShaderVariable(ShaderVars[4], 0.0f);
	Render_UpdateShaderVariable(ShaderVars[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_Draw(RenderObjs[Index]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// NOTE: Floor player fall at 1.0f.
	if (PlayerPhysObj.Position.y <= 1.0f)
	{
		Phys_WallSlide(&PlayerPhysObj, &v3(0.0f, 1.0f, 0.0f));
		Camera_SetPosition(&MainCamera,
			&PlayerPhysObj.Position);
	}

#if 0
		Render_UpdateColorVertice(
			Entity_GetObjectPtr(&State->EntityBlocks[0], 1, 0)->
			ObjectDescription.VertexBufferObjectHandleIDs, 
			PlayerPosition,
			v3(0.0f, 0.0f, 1.0f).Arr);
		Render_UnmapShaderDataPtr();
	
	// TODO: Remove, Test timer and clock features
	char Buffer[16] = {};
	Utility_FloatToChar(State->TimerArray[0].GetTime(), 1, Buffer);
	Text_DrawCharLine(string("Elapsed Time: ") + 
		string(Buffer),
		v3(Right - 200.0f, Top - 40.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0], State->FontArr);

	// NOTE: Display FPS
	char FPSBuffer[16] = {};
	Utility_FloatToChar(DEBUG_GetFPS(), 1, FPSBuffer);
	Text_DrawCharLine(string("Frames Per Second: ") +
		string(FPSBuffer),
		v3(Right - 200.0f, Top - 20.0f,
			0.0f), 0.15f, State->GPUShaderVarArray[0],
		State->FontArr);
#endif
}

void 
Game_Clean(ProgramState* State)
{
	Asset_Delete(TextureIDs[0]);
	Asset_Delete(TextureIDs[1]);
	Asset_Delete(TextureIDs[2]);
	Asset_Delete(TextureIDs[3]);
	
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(ShaderHandles[0]);
	Render_DeleteShaderProgram(ShaderHandles[1]);
}

int64
Game_Message_Handler(void* Window, uint32 Message,
	uint64 wParam, int64 lParam)
{
	int64 Result = 0;

	return Result;
}