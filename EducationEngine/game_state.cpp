#include "game_state.h"

#if DATA_ORIENTED
static GLuint TextureID[1]; 
static v3 Position, HeightMapPos;
static v3 PlayerCameraFacingVect;
static PhysicsObject PlayerPhysObj;
static m4 PlayerMatrix;
static CollisionObject PlayerCollObj;
#endif

void 
Game_Initialize(ProgramState* State)
{
	window_properties WindowDimensions = 
		Render_GetWindowProperties();
	float WindowHalfHeight = (float)WindowDimensions.Height * 0.5f;
	float WindowHalfWidth = (float)WindowDimensions.Width * 0.5f;

	State->CameraArray[0].Yaw = -90.0f;
	State->CameraArray[0].Pitch = 0.0f;
	State->CameraArray[0].UpVector = v3(0.0f, 1.0f, 0.0f);

	// TODO: calculate the ratio once only on screen size changes and save value.
	State->CameraArray[0].ProjectionMatrix = Math_PerspectiveMatrix(45.0f,
		(float)WindowDimensions.Width / (float)WindowDimensions.Height,
		0.01f, 1000.0f);

#if DATA_ORIENTED
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
	ModelObj_CreateHeightmap(State->ModelObjBlocks[0].BlockObjects[2],
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

	PlayerMatrix = Math_IdentityMatrix();
	PlayerPhysObj.AccelerationRate = 10.0f;
	PlayerPhysObj.Position = v3(2.0f, 3.0f, -2.0f);
	PlayerPhysObj.ModelMatrix = &PlayerMatrix;

	Collision_FillObject(&PlayerCollObj, 0.5f, 0.5f, 0.5f,
		&PlayerPhysObj.Position);

	Phys_AddForce(&PlayerPhysObj, &Gravity);
#else // NOT DATA_ORIENTED

	State->ShaderHandles[0] = 
		Render_CompileShaders(VertShaderForTextureAndLight,
		FragShaderForTextureAndLight);
	State->ShaderHandles[1] = 
		Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	// NOTE: Player's Model 0
	uint32 PlayerBox =
		Utility_CreateBox(State, 0.25f, 0.25f, 0.25f);
	uint32 PlayerEntity =
		Utility_CreateEntity(State, &v3(0.0f, 6.0f, 20.0f), 
		PlayerBox, 0x1111);
	Phys_SetAccelerationRate(
		Entity_GetPhysObjPtr(&State->EntityBlocks[0],
		PlayerEntity, 0), 1000.0f);

	// NOTE: Terrain 1
	uint32 Terrain = 
		Utility_CreateHeightMap(State, Asset_GetTexture(7),
		Asset_GetTexture(4));
	uint32 TerrainEntity = 
		Utility_CreateEntity(State, &v3(0.0f, 0.0f, 0.0f), 
		Terrain, 0x111);

	// NOTE: Wood Floor 2
	uint32 WoodFloor =
		Utility_CreatePlane(State, 10, 10, Asset_GetTexture(6));
	uint32 PlaneEntity = 
		Utility_CreateEntity(State, &v3(0.0f, 0.0f, 20.0f), 
		WoodFloor, 0x111);

	// NOTE: Light Box 3
	uint32 LightBox = 
		Utility_CreateBox(State, 0.25f, 0.25f, 0.25f);
	uint32 LightEntity = 
		Utility_CreateEntity(State, &v3(2.0f, 1.0f, 17.0f), 
		LightBox, 0x111);

	// NOTE: Wood Box 4
	uint32 WoodBox =
		Utility_CreateBox(State, 1.0f, 1.0f, 1.0f, 
		Asset_GetTexture(5));
	uint32 BoxEntity1 =
		Utility_CreateEntity(State, &v3(3.0f, 0.5f, 15.0f), 
		WoodBox, 0x111);

	// NOTE: Wood Box 5
	uint32 BoxEntity2 =
		Utility_CreateEntity(State, &v3(0.0f, 0.5f, 10.0f), 
		WoodBox, 0x111);
	
	// NOTE: Wood Box 6
	uint32 BoxEntity3 = 
		Utility_CreateEntity(State, &v3(6.0f, 0.5f, 10.0f), 
		WoodBox, 0x111);

	Phys_AddForce(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
		PlayerEntity, 0), &Gravity);

	//Phys_SetAccelerationRate(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
	//	BoxEntity3, 0), 10000.0f);
	Phys_AddForce(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
		BoxEntity3,	0), &v3(-1.0f, 0.0f, 0.0f));
	Phys_AddForce(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
		BoxEntity2, 0), &v3(1.0f, 0.0f, 0.0f));
	
#endif // DATA_ORIENTED
	State->TimerArray[0].Start();
}

void 
Game_Draw(ProgramState* State)
{
	Render_ClearScreen(&v4(0.2f, 0.3f, 0.3f, 1.0f));

#if DATA_ORIENTED
	Input_UpdateMouseState(State, &State->CameraArray[0]);
	Input_UpdateKeyStates(State, &State->CameraArray[0],
		&PlayerPhysObj.MoveDirection);
	Phys_CalculatePosition(&PlayerPhysObj);
	Camera_SetPosition(&State->CameraArray[0], &PlayerPhysObj.Position);

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
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, Position);
	//ModelMatrix = Math_RotateMatrix(ModelMatrix, 45.0f,
	//	v3(1.0f, 0.0f, 0.0f));

	uint32 Index = 0;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_BindTexture(TextureID[Index]);
	Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	Render_BindTexture(0);
	
	Index = 1;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	Render_BindTexture(TextureID[Index]);
	Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	Render_BindTexture(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Index = 2;
	ModelMatrix = Math_TranslateMatrix(ModelMatrix, HeightMapPos);
	
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

	Render_UpdateShaderVariable(State->GPUShaderVarArray[4], 0.0f);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		&ModelMatrix.Rc[0][0], 1, 0);
	//Render_BindTexture(TextureID[Index]);
	Render_Draw(State->RenderObjBlocks[0].BlockObjects[Index]);
	//Render_BindTexture(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (PlayerPhysObj.Position.y <= 1.0f)
	{
		Phys_WallSlide(&PlayerPhysObj, &v3(0.0f, 1.0f, 0.0f));
		Camera_SetPosition(&State->CameraArray[0],
			&PlayerPhysObj.Position);
	}


#else
	//TODO: Move into a shader struct?
	v3 LightPosition = { 2.0f, 1.0f, -17.0f };
	v3 LightColor = { 1.0f, 1.0f, 1.0f };

	Input_UpdateMouseState(
		State, Entity_GetCamera(&State->EntityBlocks[0], 0));
	Input_UpdateKeyStates(
		State, Entity_GetCamera(&State->EntityBlocks[0], 0),
		&Entity_Ptr(&State->EntityBlocks[0], 0)->DirectionVector);

	Phys_CalculatePosition(Entity_GetPhysObjPtr(
		&State->EntityBlocks[0], 0, 0));
	Entity_GetCamera(&State->EntityBlocks[0], 0)->
		SetPosition(&Entity_GetPosition(&State->EntityBlocks[0], 0));

	v3 MouseRay = Collision_UpdateMousePickRay(
		&Entity_GetCamera(&State->EntityBlocks[0], 0)->ProjectionMatrix,
		&Entity_GetCamera(&State->EntityBlocks[0], 0)->ViewMatrix);

	// NOTE: Bind New Shaders
	Render_BindShaders(State->ShaderHandles[0]);
	State->GPUShaderVarArray[0] =
		Render_GetShaderVariable(State->ShaderHandles[0], "model");
	State->GPUShaderVarArray[1] =
		Render_GetShaderVariable(State->ShaderHandles[0], "view");
	State->GPUShaderVarArray[2] =
		Render_GetShaderVariable(State->ShaderHandles[0], "projection");
	State->GPUShaderVarArray[3] =
		Render_GetShaderVariable(State->ShaderHandles[0], "lightColor");
	State->GPUShaderVarArray[4] =
		Render_GetShaderVariable(State->ShaderHandles[0], "lightPos");
	State->GPUShaderVarArray[5] =
		Render_GetShaderVariable(State->ShaderHandles[0], "myTexture");
	State->GPUShaderVarArray[6] =
		Render_GetShaderVariable(State->ShaderHandles[0], "isTextured");

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)Entity_GetCamera(&State->EntityBlocks[0], 
			0)->GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)Entity_GetCamera(&State->EntityBlocks[0],
			0)->GetProjectionMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3],
		LightColor.x, LightColor.y, LightColor.z);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4],
		LightPosition.x, LightPosition.y, LightPosition.z);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[5],
		(int32)0);

	// NOTE: Draw terrain below
	m4 ModelMatrix = Math_IdentityMatrix();

	bool IsTextured = 0;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[6], 
		IsTextured);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		(float*)&ModelMatrix, 1, 0);

	Entity_DrawPolyGonMode(&State->EntityBlocks[0], 1,
		State->GPUShaderVarArray[0]);

	v3 Position = Entity_GetPosition(&State->EntityBlocks[0], 0);

	if ((Position.x < 1.0f) ||
		(Position.x > Entity_Width(&State->EntityBlocks[0], 1) - 1) ||
		(Position.z > -1.0f) ||
		(Position.z < -Entity_Depth(&State->EntityBlocks[0], 1) + 1))
	{
		// NOTE: Outside of terrain map
		// NOTE: If collision with floor
		if (Position.y <= 1.0f)
		{
			Phys_WallSlide(
				Entity_GetPhysObjPtr(&State->EntityBlocks[0],
				0, 0), &v3(0.0f, 1.0f, 0.0f));
			Entity_GetCamera(&State->EntityBlocks[0], 0)->
				SetPosition(&Entity_GetPosition(
					&State->EntityBlocks[0], 0));
		}
		else
		{
			// NOTE: No collision.
		}
	}
	else
	{
		int32 x, z;
		x = (int32)round(Position.x) - 1; // because negative is backwards?
		z = (int32)round(Position.z);

		// NOTE: Test Code
		uint32 PlayerPosition =
			static_cast<uint32>(((x * Entity_Width(&State->EntityBlocks[0],
				1)) + (Entity_Depth(&State->EntityBlocks[0], 1) - z)) * 
				3 * sizeof(float));

		Render_UpdateColorVertice(
			Entity_GetObjectPtr(&State->EntityBlocks[0], 1, 0)->
			ObjectDescription.VertexBufferObjectHandleIDs, PlayerPosition,
			v3(0.0f, 0.0f, 1.0f).Arr);
		Render_UnmapShaderDataPtr();

		if (Collision_HeightMap(Entity_GetCollisionObjPtr(
			&State->EntityBlocks[0], 1, 0), Position))
		{
			Phys_WallSlide(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
				0, 0), &Entity_GetCollisionObjPtr(&State->EntityBlocks[0],
					1, 0)->CollideNormal);
			Entity_GetCamera(&State->EntityBlocks[0], 0)->
				SetPosition(&Entity_GetPosition(&State->EntityBlocks[0], 0));
		}
		else
		{
			// NOTE: No Heightmap collision.
		}
	}

	//NOTE: Draw Objects below
	IsTextured = 1;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[6], IsTextured);
	for (uint32 Index = 2; Index < State->EntityCount; Index++) 
	{
		Entity_Draw(&State->EntityBlocks[0], Index,
			State->GPUShaderVarArray[0]);
	}

	//NOTE: Draw UI Below
	//TODO: Store Dimensions in State
	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfHeight = 0.5f * (float)WindowDimensions.Height;
	float WindowHalfWidth = 0.5f * (float)WindowDimensions.Width;
	float Top = WindowHalfHeight;
	float Bottom = -WindowHalfHeight;
	float Left = -WindowHalfWidth;
	float Right = WindowHalfWidth;

	Render_BindShaders(State->ShaderHandles[1]);
	State->GPUShaderVarArray[0] =
		Render_GetShaderVariable(State->ShaderHandles[1], "model");
	State->GPUShaderVarArray[1] =
		Render_GetShaderVariable(State->ShaderHandles[1], "view");
	State->GPUShaderVarArray[2] =
		Render_GetShaderVariable(State->ShaderHandles[1], "projection");
	State->GPUShaderVarArray[3] =
		Render_GetShaderVariable(State->ShaderHandles[1], "myTexture");

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)State->CameraArray[0].GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)State->CameraArray[0].GetProjectionMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3], 0);

	//TODO: Remove this, testing only
	Phys_CalculatePosition(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
		6, 0));
	Phys_CalculatePosition(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
		5, 0));

	if (Collision_GJK(Entity_GetCollisionObjPtr(&State->EntityBlocks[0],
		5, 0),
		Entity_GetCollisionObjPtr(&State->EntityBlocks[0], 6, 0)))
	{
		Phys_BounceBack(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
			5, 0), &v3(-1.0f, 0.0f, 0.0f));
		Phys_BounceBack(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
			6, 0), &v3(1.0f, 0.0f, 0.0f));
	}

	if (Collision_GJK(Entity_GetCollisionObjPtr(&State->EntityBlocks[0],
		0, 0),
		Entity_GetCollisionObjPtr(&State->EntityBlocks[0], 6, 0)))
	{
		Phys_BounceBack(Entity_GetPhysObjPtr(&State->EntityBlocks[0],
			0, 0), &v3(0.0f, 0.0f, 1.0f));
		Entity_GetCamera(&State->EntityBlocks[0], 0)->
			SetPosition(&Entity_GetPosition(&State->EntityBlocks[0], 0));
	}

	//char BufferX[16] = {};
	//char BufferY[16] = {};
	//char BufferZ[16] = {};
	//Utility_FloatToChar(Entity_GetPhysObjPtr(&State->EntityBlocks,
	//	6)->ForceSum.x, 3, BufferX);
	//Utility_FloatToChar(Entity_GetPhysObjPtr(&State->EntityBlocks,
	//	6)->ForceSum.y, 3, BufferY);
	//Utility_FloatToChar(Entity_GetPhysObjPtr(&State->EntityBlocks,
	//	6)->ForceSum.z, 3, BufferZ);
	//
	//Text_DrawCharLine(string("Box 6 Force: \0") +
	//	string(BufferX) + string(" ") + 
	//	string(BufferY) + string(" ") +
	//	string(BufferZ), 
	//	v3(Left + 20.0f, Top - 100.0f, 0.0f), 0.15f,
	//	State->GPUShaderVarArray[0]);

	float Distance = 0.0f;
	if (Collision_RayToOBB(&Entity_GetPosition(&State->EntityBlocks[0], 0),
		&MouseRay, Entity_GetCollisionObjPtr(&State->EntityBlocks[0], 4, 0),
		&Distance))
	{
		Text_DrawCharLine(string("MOUSE COLLISION \0"),
			v3(Left + 20.0f, Top - 100.0f, 0.0f), 0.15f,
			State->GPUShaderVarArray[0], State->FontArr);
	}
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
			0.0f), 0.15f, State->GPUShaderVarArray[0], State->FontArr);

	v3 PlayerPosition = Entity_GetPosition(&State->EntityBlocks[0], 0);

	char BufferX2[16] = {};
	char BufferY2[16] = {};
	char BufferZ2[16] = {};
	Utility_FloatToChar(PlayerPosition.x, 3, BufferX2);
	Utility_FloatToChar(PlayerPosition.y, 3, BufferY2);
	Utility_FloatToChar(PlayerPosition.z, 3, BufferZ2);
	Text_DrawCharLine(string("Player Position: \0") +
		string(BufferX2) + string(" ") +
		string(BufferY2) + string(" ") +
		string(BufferZ2),
		v3(Left + 20.0f, Top - 20.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0], State->FontArr);

	v2 CursorPosition;
	CursorPosition.x =
		State->CursorPosition.x - ((float)WindowDimensions.Width * 0.5f);
	CursorPosition.y =
		((float)WindowDimensions.Height * 0.5f) - State->CursorPosition.y;

	char BufferX3[16] = {};
	char BufferY3[16] = {};
	Utility_FloatToChar(CursorPosition.x, 3, BufferX3);
	Utility_FloatToChar(CursorPosition.y, 3, BufferY3);
	Text_DrawCharLine(string("Cursor Position: \0") +
		string(BufferX3) + string(" ") +
		string(BufferY3),
		v3(Left + 20.0f, Top - 40.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0], State->FontArr);

	char BufferX4[16] = {};
	char BufferY4[16] = {};
	char BufferZ4[16] = {};
	Utility_FloatToChar(MouseRay.x, 3, BufferX4);
	Utility_FloatToChar(MouseRay.y, 3, BufferY4);
	Utility_FloatToChar(MouseRay.z, 3, BufferZ4);
	Text_DrawCharLine(string("Cursor World Array: \0") +
		string(BufferX4) + string(" ") +
		string(BufferY4) + string(" ") +
		string(BufferZ4),
		v3(Left + 20.0f, Top - 60.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0], State->FontArr);
#endif // DATA_ORIENTED
}

void 
Game_Clean(ProgramState* State)
{
#if DATA_ORIENTED

#else
	Entity_DeleteBlock(&State->EntityBlocks[0]);
	RenderObj_DeleteBlock(&State->RenderObjBlocks[0]);
	State->ObjectCount = 0;
	State->EntityCount = 0;
#endif // DATA_ORIENTED
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}