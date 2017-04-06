#include "game_state.h"

void Game_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 256);
	Object_CreateBlock(State->ObjectBlockNum, 256);
	uint32 ObjectCount = 0;
	uint32 EntityCount = 0;

	State->ShaderHandles[0] = 
		Render_CompileShaders(VertShaderForTextureAndLight,
		FragShaderForTextureAndLight);

	//NOTE: Player's Model
	Object_Create(new Box, State->ObjectBlockNum, ObjectCount,
		0.25f, 0.25f, 0.25f);
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum, 
		ObjectCount, &v3(0.0f, 6.0f, 20.0f));
	Entity_AddCamera(State->ObjectBlockNum, ObjectCount, new Camera);
	Phys_SetAccelerationRate(Entity_GetPhysObjPtr(State->ObjectBlockNum,
		ObjectCount), 1000.0f);
	ObjectCount++;
	EntityCount++;

	//NOTE: Terrain
	Object_Create(new HeightMap, State->ObjectBlockNum, ObjectCount, 
		Asset_GetTexture(7));
	Object_SetTexture(State->ObjectBlockNum, ObjectCount, Asset_GetTexture(4));
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum,
		ObjectCount, &v3(0.0f, 0.0f, 0.0f));
	ObjectCount++;
	EntityCount++;

	// Wood Floor
	Object_Create(new Plane2D, State->ObjectBlockNum, ObjectCount, 10, 10);
	Object_SetTexture(State->ObjectBlockNum, ObjectCount, Asset_GetTexture(6));
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum,
		ObjectCount, &v3(0.0f, -0.5f, 20.0f));
	ObjectCount++;
	EntityCount++;

	// Light Box
	Object_Create(new Box, State->ObjectBlockNum, ObjectCount,
		0.25f, 0.25f, 0.25f);
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum,
		ObjectCount, &v3(2.0f, 1.0f, 17.0f));
	ObjectCount++;
	EntityCount++;

	// Wood Box 1
	Object_Create(new Box, State->ObjectBlockNum, ObjectCount,
		0.25f, 0.25f, 0.25f);
	Object_SetTexture(State->ObjectBlockNum, ObjectCount, Asset_GetTexture(5));
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum,
		ObjectCount, &v3(3.0f, 0.0f, 15.0f));
	EntityCount++;

	// Wood Box 2
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum,
		ObjectCount, &v3(0.0f, 0.0f, 10.0f));
	Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, EntityCount),
		&v3(0.001f, 0.0f, 0.0f));
	EntityCount++;

	// Wood Box 3
	Entity_Create(State->EntityBlockNum, EntityCount, State->ObjectBlockNum,
		ObjectCount, &v3(6.0f, 0.0f, 10.0f));
	Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, EntityCount),
		&v3(-0.001f, 0.0f, 0.0f));
	ObjectCount++;
	EntityCount++;

	//NOTE: User Inferface initialization below
	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfHeight = 0.5f * (float)WindowDimensions.Height;
	float WindowHalfWidth = 0.5f * (float)WindowDimensions.Width;

	State->CameraArray[1].SetPosition(&v3(-WindowHalfWidth,
		-WindowHalfHeight, 1.0f));
	State->CameraArray[1].SetProjectionMatrix(0);

	State->ShaderHandles[1] = Render_CompileShaders(TextVertexShaderSource, 
			TextFragmentShaderSource);

	State->TimerArray[0].Start();
}

void Game_Draw(ProgramState* State)
{
	v3 LightPosition = { 2.0f, 1.0f, -17.0f }; //TODO: Move into a shader struct?
	v3 LightColor = { 1.0f, 1.0f, 1.0f };

	Render_ClearScreen();
	Platform_GetCursorPosition(&State->CursorPosition.x,
		&State->CursorPosition.y);
	Input_UpdateMouseState(State);
	Input_UpdateKeyStates(State);

	Phys_CalculatePosition(Entity_GetPhysObjPtr(State->EntityBlockNum, 0));
	Entity_GetCamera(State->EntityBlockNum, 0)->
		SetPosition(&Entity_GetPosition(State->EntityBlockNum, 0));

	Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0),
		&Gravity);

	v3 MouseRay = Collision_UpdateMousePickRay(
		&Entity_GetCamera(State->EntityBlockNum, 0)->ProjectionMatrix,
		&Entity_GetCamera(State->EntityBlockNum, 0)->ViewMatrix);

	//TODO: Remove, test code.
	Entity_Ptr(State->EntityBlockNum, 5)->DirectionVector = v3(1.0f, 0.0f, 0.0f);
	Entity_Ptr(State->EntityBlockNum, 6)->DirectionVector = v3(-1.0f, 0.0f, 0.0f);
	Phys_CalculatePosition(Entity_GetPhysObjPtr(State->EntityBlockNum, 5));
	Phys_CalculatePosition(Entity_GetPhysObjPtr(State->EntityBlockNum, 6));
	Entity_UpdatePosition(State->EntityBlockNum, 5);
	Entity_UpdatePosition(State->EntityBlockNum, 6);


	// Bind New Shaders
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

	// Update Shader Variables for Player 1
	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)Entity_GetCamera(State->EntityBlockNum, 0)->GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)Entity_GetCamera(State->EntityBlockNum, 0)->GetProjectionMatrix(), 1, 0);

	// Update Shader Variables for World
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3],
		LightColor.x, LightColor.y, LightColor.z);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4],
		LightPosition.x, LightPosition.y, LightPosition.z);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[5], (int32)0);

	// NOTE: Draw terrain below
	m4 ModelMatrix = Math_IdentityMatrix();

	bool IsTextured = 0;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[6], IsTextured);
		
	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		(float*)&ModelMatrix, 1, 0);
	Entity_DrawPolyGonMode(State->EntityBlockNum, 1,
		State->GPUShaderVarArray[0]);

	v3 Position = Entity_GetPosition(State->EntityBlockNum, 0);

	if ((Position.x < 1.0f) ||
		(Position.x > Entity_GetWidth(State->EntityBlockNum, 1) - 1) ||
		(Position.z > -1.0f) ||
		(Position.z < -Entity_GetDepth(State->EntityBlockNum, 1) + 1))
	{
		// outside of terrain map
		if (Position.y < 0)
		{
			Position.y = 0;
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0),
				&(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), 
				&v3(0.0f,
					-Entity_GetPhysObjPtr(State->EntityBlockNum, 0)->Force.y,
					0.0f));
			Entity_SetPosition(State->EntityBlockNum, 0, Position);
		}
	}
	else
	{
		int32 x, z;
		x = (int32)round(Position.x) - 1; // because negative is backwards?
		z = (int32)round(Position.z);

		// Test Code
		uint32 PlayerPosition =
			static_cast<uint32>(((x * Entity_GetWidth(State->EntityBlockNum, 1)) +
			(Entity_GetDepth(State->EntityBlockNum, 1) - z)) * 3 * sizeof(float));

		Render_UpdateColorVertice(
			Entity_GetObjectPtr(State->EntityBlockNum, 1)->
			ObjectDescription.VertexBufferObjectHandleIDs, PlayerPosition,
			v3(0.0f, 0.0f, 1.0f).Arr);
		Render_UnmapShaderDataPtr();

		if (Collision_HeightMap(
			static_cast<HeightMap*>(Entity_GetObjectPtr(State->EntityBlockNum, 1)), Position))
		{
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0),
				&(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0),
				&v3(0.0f,
					-Entity_GetPhysObjPtr(State->EntityBlockNum, 0)->Force.y, 
					0.0f));
			Entity_SetPosition(State->EntityBlockNum, 0, Position);
		}
	}

	//v3 MouseArray = Collision_UpdateMousePickRay(
	//	State->CameraArray[1].GetProjectionMatrix(),
	//	State->CameraArray[1].GetViewMatrix());

	//NOTE: Draw Objects below
	IsTextured = 1;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[6], IsTextured);
	for (uint32 Index = 2; Index < 7; Index++) 
	{
		Entity_Draw(State->EntityBlockNum, Index, State->GPUShaderVarArray[0]);
	}
	//testing code, remove later
	bool collide = Collision_RayToObject(&Entity_GetPosition(State->EntityBlockNum, 0), &MouseRay, Entity_GetCollisionObjPtr(State->EntityBlockNum, 4));


	//NOTE: Draw UI Below
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

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1],
		44, (float*)State->CameraArray[1].GetViewMatrix(), 1, 0); // TODO: clean this up
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2],
		44, (float*)State->CameraArray[1].GetProjectionMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3],
		(int32)0);

	//TODO: Remove this, testing only
	if (Collision_GJK(Entity_GetCollisionObjPtr(State->EntityBlockNum, 5),
		Entity_GetCollisionObjPtr(State->EntityBlockNum, 6)))
	{
		Text_DrawCharLine(string("COLLISION \0"),
			v3(Left + 20.0f, Top - 80.0f, 0.0f), 0.15f,
			State->GPUShaderVarArray[0]);
	}

	v3 ViewDirection = Entity_GetCamera(State->EntityBlockNum, 0)->FacingVector;
	Text_DrawCharLine(string("View Direction: \0") +
		string(Platform_FloatToChar(ViewDirection.x)) + string(" ") +
		string(Platform_FloatToChar(ViewDirection.y)) + string(" ") +
		string(Platform_FloatToChar(ViewDirection.z)),
		v3(Left + 20.0f, Top - 100.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0]);

	m4 *MatrixViewDirection = Entity_GetCamera(State->EntityBlockNum, 0)->GetViewMatrix();
	Text_DrawCharLine(string("Matrix View Direction: \0") +
		string(Platform_FloatToChar(MatrixViewDirection->Rc[2][0])) + string(" ") +
		string(Platform_FloatToChar(MatrixViewDirection->Rc[2][1])) + string(" ") +
		string(Platform_FloatToChar(MatrixViewDirection->Rc[2][2])),
		v3(Left + 20.0f, Top - 120.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0]);
	

	// TODO: Remove, Test timer and clock features
	Text_DrawCharLine(string("Elapsed Time: ") + 
		string(Platform_FloatToChar(State->TimerArray[0].GetTime(), 1)),
		v3(Right - 200.0f, Top - 40.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0]);

	// Display FPS
	Text_DrawCharLine(string("Frames Per Second: ") +
		string(Platform_FloatToChar(DEBUG_GetFPS(), 1)),
		v3(Right - 200.0f, Top - 20.0f,
			0.0f), 0.15f, State->GPUShaderVarArray[0]);

	v3 PlayerPosition = Entity_GetPosition(State->EntityBlockNum, 0);

	Text_DrawCharLine(string("Player Position: \0") +
		string(Platform_FloatToChar(PlayerPosition.x)) + string(" ") +
		string(Platform_FloatToChar(PlayerPosition.y)) + string(" ") +
		string(Platform_FloatToChar(PlayerPosition.z)),
		v3(Left + 20.0f, Top - 20.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0]);

	v2 CursorPosition;
	CursorPosition.x =
		State->CursorPosition.x - ((float)WindowDimensions.Width * 0.5f);
	CursorPosition.y =
		((float)WindowDimensions.Height * 0.5f) - State->CursorPosition.y;

	Text_DrawCharLine(string("Cursor Position: \0") +
		string(Platform_FloatToChar(CursorPosition.x)) + string(" ") +
		string(Platform_FloatToChar(CursorPosition.y)),
		v3(Left + 20.0f, Top - 40.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0]);

	Text_DrawCharLine(string("Cursor World Array: \0") +
		string(Platform_FloatToChar(MouseRay.x)) + string(" ") +
		string(Platform_FloatToChar(MouseRay.y)) + string(" ") + 
		string(Platform_FloatToChar(MouseRay.z)),
		v3(Left + 20.0f, Top - 60.0f, 0.0f), 0.15f,
		State->GPUShaderVarArray[0]);
	
	//Text_StepDown();
	//Text_DrawCharLine(string("Cursor Position: \0") +
	//	string(Platform_FloatToChar(CursorPosition.x)) + string(" ") +
	//	string(Platform_FloatToChar(CursorPosition.y)));

	Text_ClearGlobalStream();
}

void Game_Clean(ProgramState* State)
{
	Entity_DeleteBlock(State->EntityBlockNum);
	Object_DeleteBlock(State->ObjectBlockNum);
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
}