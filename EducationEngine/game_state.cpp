#include "game_state.h"

static GLuint TextureID[1]; 
static v3 Position, HeightMapPos;
static v3 PlayerCameraFacingVect;
static PhysicsObject PlayerPhysObj;
static m4 PlayerMatrix;
static CollisionObject PlayerCollObj;
static m4 BoxA, BoxB;
static v3 BoxAPos, BoxBPos;

#if MODULE_MODE
Font *ThatFont;
#endif

void 
Game_Initialize(ProgramState* State)
{
#if MODULE_MODE
	Asset_LoadBMP("Images/grass2.bmp"); // 4
	Asset_LoadBMP("Images/container.bmp"); // 5
	Asset_LoadBMP("Images/woodfloor.bmp"); // 6
	Asset_LoadPNG("Images/StartHeightMap.png"); // 7

	ThatFont = new Font;
	Text_BuildFont("arial\0", ThatFont);

	State_CreateRenderObjectBlocks(State,
		2, 256);
	State_CreateModelObjectBlocks(State,
		2, 256);
	State_CreateCollisionObjects(State,
		12);
	State_CreatePhysicsObjects(State,
		12);
	State_CreateTextObjs(State,
		TEXT_OBJECTS_PER_PROGSTATE);
	State_CreateCameras(State, 1);
	State_CreateTimers(State, 1);
	State_CreateShaderVariables(State, 7);
	State_CreateShaderHandles(State, 2);

	State->FontArr = ThatFont;
	State->FontCount = 1;

	State->Status = 1;

#endif

	window_properties WindowDimensions = 
		Render_GetWindowProperties();
	float WindowHalfHeight = (float)WindowDimensions.Height * 0.5f;
	float WindowHalfWidth = (float)WindowDimensions.Width * 0.5f;

	State->CameraArray[0].Yaw = -90.0f;
	State->CameraArray[0].Pitch = 0.0f; 
	State->CameraArray[0].UpVector = v3(0.0f, 1.0f, 0.0f);

	// TODO: calculate the ratio once only on screen size
	//	changes and save value.
	State->CameraArray[0].ProjectionMatrix = 
		Math_PerspectiveMatrix(45.0f,
		(float)WindowDimensions.Width / 
			(float)WindowDimensions.Height,
		0.01f, 1000.0f);

	State->ShaderHandles[0] =
		Render_CompileShaders(VertexShader_Game,
			FragmentShader_Game);

	State->CameraArray[0].ForwardVector = v3(0.0f, 0.0f, -1.0f);

#if MEMORY_ON

#else
	State->ModelObjBlocks[0].BlockObjects[0] = new Model;
	State->RenderObjBlocks[0].BlockObjects[0] = new RenderObj;

	State->ModelObjBlocks[0].BlockObjects[1] = new Model;
	State->RenderObjBlocks[0].BlockObjects[1] = new RenderObj;

	State->ModelObjBlocks[0].BlockObjects[2] = new Model;
	State->RenderObjBlocks[0].BlockObjects[2] = new RenderObj;
#endif // MEMORY_ON
	ModelObj_CreateBox(State->ModelObjBlocks[0].BlockObjects[0],
		1.0f, 1.0f, 1.0f);
	ModelObj_CreatePlane(State->ModelObjBlocks[0].BlockObjects[1],
		20, 20);
	ModelObj_CreateHeightmap(
		State->ModelObjBlocks[0].BlockObjects[2],
		Asset_GetTexture(7));
	
	Position = v3(0.0f, 0.0f, 0.0f);
	Texture2D* Textures[2];
	Textures[0] = Asset_GetTexture(5);
	Textures[1] = Asset_GetTexture(4);

	// NOTE: Box
	uint32 Index = 0;
	RenderObj_CreateRenderObject(
		State->RenderObjBlocks[0].BlockObjects[Index],
		State->ModelObjBlocks[0].BlockObjects[Index]);
	Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
		Textures[Index]->Height, Textures[Index]->Data);
	
	// NOTE: Grass
	Index = 1;
	RenderObj_CreateRenderObject(
		State->RenderObjBlocks[0].BlockObjects[Index],
		State->ModelObjBlocks[0].BlockObjects[Index]);
	Render_BuildTexture(&TextureID[Index], Textures[Index]->Width,
		Textures[Index]->Height, Textures[Index]->Data);

	// NOTE: HeightMap
	Index = 2;
	RenderObj_CreateRenderObject(
		State->RenderObjBlocks[0].BlockObjects[Index],
		State->ModelObjBlocks[0].BlockObjects[Index]);
	HeightMapPos = v3(20.0f, 0.0f, -20.0f);
	Collision_FillObject(&State->CollisionObj[Index], 
		(float)Asset_GetTexture(7)->Width, 0.0f,
		(float)Asset_GetTexture(7)->Height, &HeightMapPos);
	State->CollisionObj[Index].VerticesPtr = 
		State->ModelObjBlocks[0].BlockObjects[Index]->Data[0];

	Index = 3;

	PlayerMatrix = Math_IdentityMatrix();
	PlayerPhysObj.AccelerationRate = 10.0f;
	PlayerPhysObj.Position = v3(2.0f, 3.0f, -2.0f);
	PlayerPhysObj.ModelMatrix = &PlayerMatrix;

	Collision_FillObject(&PlayerCollObj, 0.5f, 0.5f, 0.5f,
		&PlayerPhysObj.Position);

	BoxAPos = v3(1.0f, 0.5f, -10.0);
	BoxBPos = v3(6.0f, 0.5f, -10.0f);
	State->PhysicsObj[0].AccelerationRate = 10.0f;
	State->PhysicsObj[0].Position = BoxAPos;
	State->PhysicsObj[0].ModelMatrix = &BoxA;
	Collision_FillObject(&State->CollisionObj[0], 1.0f, 1.0f, 1.0f,
		&BoxAPos);
	State->CollisionObj[0].VerticesPtr = 
		State->ModelObjBlocks[0].BlockObjects[0]->Data[0];
	State->CollisionObj[0].NumVertices = 
		State->ModelObjBlocks[0].BlockObjects[0]->NumVertices;

	State->PhysicsObj[1].AccelerationRate = 10.0f;
	State->PhysicsObj[1].Position = BoxBPos;
	State->PhysicsObj[1].ModelMatrix = &BoxB;
	Collision_FillObject(&State->CollisionObj[1], 1.0f, 1.0f, 1.0f,
		&BoxBPos);
	State->CollisionObj[1].VerticesPtr =
		State->ModelObjBlocks[0].BlockObjects[0]->Data[0];
	State->CollisionObj[1].NumVertices =
		 State->ModelObjBlocks[0].BlockObjects[0]->NumVertices;

	Phys_AddForce(&PlayerPhysObj, &Gravity);
	Phys_AddForce(&State->PhysicsObj[0], &v3(1.0f, 0.0f, 0.0f));
	Phys_AddForce(&State->PhysicsObj[1], &v3(-1.0f, 0.0f, 0.0f));

	State->TimerArray[0].Start();
}

void 
Game_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(0.2f, 0.3f, 0.3f, 1.0f));

	Input_UpdateMouseState(State, &State->CameraArray[0]);
	Input_UpdateKeyStates(State, &State->CameraArray[0],
		&PlayerPhysObj.MoveDirection);
	Phys_CalculatePosition(&PlayerPhysObj);
	Camera_SetPosition(&State->CameraArray[0],
		&PlayerPhysObj.Position);

	Render_BindShaders(State->ShaderHandles[0]);
	State->GPUShaderVarArray[0] =
		Render_GetShaderVariable(State->ShaderHandles[0],
			"model");
	State->GPUShaderVarArray[1] =
		Render_GetShaderVariable(State->ShaderHandles[0],
			"view");
	State->GPUShaderVarArray[2] =
		Render_GetShaderVariable(State->ShaderHandles[0], 
			"projection");
	State->GPUShaderVarArray[3] =
		Render_GetShaderVariable(State->ShaderHandles[0], 
			"myTexture");
	State->GPUShaderVarArray[4] =
		Render_GetShaderVariable(State->ShaderHandles[0],
			"TextureFlag");

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)&State->CameraArray[0].ViewMatrix,
		1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)&State->CameraArray[0].ProjectionMatrix,
		1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 1.0f);


	m4 ModelMatrix = Math_IdentityMatrix();

	// NOTE: Box
	uint32 Index = 0;
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_BindTexture(TextureID[Index]);
	Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);

	Phys_CalculatePosition(&State->PhysicsObj[0]);
	Phys_CalculatePosition(&State->PhysicsObj[1]);
	State->CollisionObj[0].Position = State->PhysicsObj[0].Position;
	State->CollisionObj[1].Position = State->PhysicsObj[1].Position;

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
		BoxA = Math_TranslateMatrix(BoxA, BoxAPos);
		Render_UpdateShaderVariable(
			State->GPUShaderVarArray[0], 44, &BoxA.Rc[0][0], 1, 0);
		Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);

		BoxBPos = State->PhysicsObj[1].Position;
		BoxB = Math_IdentityMatrix();
		BoxB = Math_TranslateMatrix(BoxB, BoxBPos);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
			&BoxB.Rc[0][0], 1, 0);
		Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	}
	else
	{
		//Phys_CalculatePosition(&State->PhysicsObj[0]);
		BoxAPos = State->PhysicsObj[0].Position;
		BoxA = Math_IdentityMatrix();
		BoxA = Math_TranslateMatrix(BoxA, BoxAPos);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
			&BoxA.Rc[0][0], 1, 0);
		Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);

		//Phys_CalculatePosition(&State->PhysicsObj[1]);
		BoxBPos = State->PhysicsObj[1].Position;
		BoxB = Math_IdentityMatrix();
		BoxB = Math_TranslateMatrix(BoxB, BoxBPos);
		Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
			&BoxB.Rc[0][0], 1, 0);
		Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	}
	Render_BindTexture(0);
	
	// NOTE: Grass
	Index = 1;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_BindTexture(TextureID[Index]);
	Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	Render_BindTexture(0);

	// NOTE: Heightmap
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Index = 2;

	PlayerCollObj.Position = PlayerPhysObj.Position;
	if (Collision_HeightMap(&State->CollisionObj[Index],
		State->ModelObjBlocks[0].BlockObjects[2]->Data[3],
		&PlayerCollObj))
	{
		Phys_WallSlide(&PlayerPhysObj, 
			&State->CollisionObj[Index].CollideNormal);
		Camera_SetPosition(&State->CameraArray[0],
			&PlayerPhysObj.Position);
	}

	ModelMatrix = Math_TranslateMatrix(ModelMatrix, HeightMapPos);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 0.0f);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// NOTE: Floor player fall at 1.0f.
	if (PlayerPhysObj.Position.y <= 1.0f)
	{
		Phys_WallSlide(&PlayerPhysObj, &v3(0.0f, 1.0f, 0.0f));
		Camera_SetPosition(&State->CameraArray[0],
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

#if 0
	Entity_DeleteBlock(&State->EntityBlocks[0]);
	RenderObj_DeleteBlock(&State->RenderObjBlocks[0]);
	State->ObjectCount = 0;
	State->EntityCount = 0;
#endif 
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
#if MODULE_MODE
	delete ThatFont;
#endif
}