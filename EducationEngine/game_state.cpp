#include "game_state.h"

void Game_Initialize(ProgramState* State)
{
	Entity_CreateBlock(State->EntityBlockNum, 256);
	Object_CreateBlock(State->ObjectBlockNum, 256);

	State->ShaderHandles[0] = Render_CompileShaders(VertShaderForTextureAndLight,
		FragShaderForTextureAndLight);
	State->ShaderHandles[1] = Render_CompileShaders(TestVertexShader,
		TestFragShader);

	//NOTE: This represents player for now
	Object_Create(new Box, State->EntityBlockNum, 0, 0.25f, 0.25f, 0.25f);
	Entity_Create(State->EntityBlockNum, 0, State->ObjectBlockNum, 0, v3(0.0f, 6.0f, 20.0f));
	Entity_CreatePlayer(State->EntityBlockNum, 0, new Player);

	//TODO: Find a better way to link player and camera?
	State->CameraArray[0].SetPosition(&v3(0.0f, 6.0f, 20.0f));
	State->CameraArray[0].SetFrontDirection(&v3(0.0f, 0.0f, -1.0f));
	State->CameraArray[0].SetProjectionMatrix(1);

	Pysc_SetAccelerationRate(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), 1000.0f);

	// Wood Box
	Object_Create(new Box, State->ObjectBlockNum, 1, 0.25f, 0.25f, 0.25f);
	Entity_Create(State->EntityBlockNum, 1, State->ObjectBlockNum, 1, v3(3.0f, 0.0f, 15.0f));
	Entity_AddTexture(State->EntityBlockNum, 1, Asset_GetTexture(5));
	//TODO: Add Opengl uniforms for all
	// Light Box
	Object_Create(new Box, State->ObjectBlockNum, 2, 0.25f, 0.25f, 0.25f);
	Entity_Create(State->EntityBlockNum, 2, State->ObjectBlockNum, 2, v3(2.0f, 1.0f, 17.0f)); 

	// Wood Floor
	Object_Create(new Plane2D, State->ObjectBlockNum, 3, 10, 10);
	Entity_Create(State->EntityBlockNum, 3, State->ObjectBlockNum, 3, v3(0.0f, -0.5f, 20.0f));
	Entity_AddTexture(State->EntityBlockNum, 3, Asset_GetTexture(6));

	//NOTE: Terrain
	Object_Create(new HeightMap, State->ObjectBlockNum, 4, Asset_GetTexture(7));
	Entity_Create(State->EntityBlockNum, 4, State->ObjectBlockNum, 4, v3(0.0f, 0.0f, 0.0f));
	Entity_AddTexture(State->EntityBlockNum, 4, Asset_GetTexture(4));

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
	v3 LightPosition = { 2.0f, 1.0f, -17.0f }; //TODO: Move into a shader struct?
	v3 LightColor = { 1.0f, 1.0f, 1.0f };

	Render_ClearScreen();

	void* Player1 = Entity_GetPlayerPtr(State->EntityBlockNum, 0);
	static_cast<Player*>(Player1)->UpdateRotation();
	State->CameraArray[0].SetFrontDirection(
		static_cast<Player*>(Player1)->GetFrontDirection());
	static_cast<Player*>(Player1)->UpdateTranslation();

	Phys_SetMoveDirection(Entity_GetPhysObjPtr(State->EntityBlockNum, 0),
		*static_cast<Player*>(Player1)->GetDirection());
	Phys_CalculatePosition(Entity_GetPhysObjPtr(State->EntityBlockNum, 0));
	State->CameraArray[0].SetPosition(
		&Entity_GetPosition(State->EntityBlockNum, 0));

	Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), &Gravity);

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
	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44,
		(float*)State->CameraArray[0].GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44,
		(float*)State->CameraArray[0].GetProjectionMatrix(), 1, 0);

	// Update Shader Variables for World
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3],
		LightColor.x, LightColor.y, LightColor.z);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[4],
		LightPosition.x, LightPosition.y, LightPosition.z);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[5], (int32)0);

	// NOTE: Draw terrain below
	m4 ModelMatrix = IdentityMatrix();

	bool Choice = 0;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[6], Choice);

	Render_UpdateShaderVariable(State->GPUShaderVarArray[0], 44,
		(float*)&ModelMatrix, 1, 0);
	Entity_DrawPolyGonMode(State->EntityBlockNum, 4, State->GPUShaderVarArray[0]);

	v3 Position = Entity_GetPosition(State->EntityBlockNum, 0);

	if ((Position.x < 1.0f) ||
		(Position.x > Entity_GetWidth(State->EntityBlockNum, 4) - 1) ||
		(Position.z > -1.0f) ||
		(Position.z < -Entity_GetDepth(State->EntityBlockNum, 4) + 1))
	{
		// outside of terrain map
		if (Position.y < 0)
		{
			Position.y = 0;
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), &(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), 
				&v3(0.0f, -Entity_GetPhysObjPtr(State->EntityBlockNum, 0)->Acceleration.y,
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
		uint32 PlayerPosition = static_cast<uint32>(((x * Entity_GetWidth(State->EntityBlockNum, 4)) +
			(Entity_GetDepth(State->EntityBlockNum, 4) - z)) * 3 * sizeof(float));

		Render_UpdateColorVertice(
			Entity_GetObjInstancePtr(State->EntityBlockNum, 4)->
			ObjectPtr->ObjectDescription.VertexBufferObjectHandleIDs,
			PlayerPosition,
			v3(0.0f, 0.0f, 1.0f).Arr);
		Render_UnmapShaderDataPtr();

		if (Collision_HeightMap(static_cast<HeightMap*>(Entity_GetObjInstancePtr(State->EntityBlockNum, 4)->ObjectPtr), Position))
		{
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), &(-Gravity));
			Phys_AddForce(Entity_GetPhysObjPtr(State->EntityBlockNum, 0), &v3(0.0f,
					-Entity_GetPhysObjPtr(State->EntityBlockNum, 0)->Acceleration.y, 
					0.0f));
			Entity_SetPosition(State->EntityBlockNum, 0, Position);
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
	Choice = 1;
	Render_UpdateShaderVariable(State->GPUShaderVarArray[6], Choice);
	for (uint32 Index = 1; Index < 4; Index++) 
	{
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

	Render_UpdateShaderVariable(State->GPUShaderVarArray[1], 44, (float*)State->CameraArray[1].GetViewMatrix(), 1, 0); // TODO: clean this up
	Render_UpdateShaderVariable(State->GPUShaderVarArray[2], 44, (float*)State->CameraArray[1].GetViewMatrix(), 1, 0);
	Render_UpdateShaderVariable(State->GPUShaderVarArray[3], (int32)0);

	// TODO: Remove, Test timer and clock features
	Text_DrawCharLine(string("Elapsed Time: ") + 
		string(Platform_FloatToChar(State->TimerArray[0].GetTime(), 1)),
		v3(Right - 200.0f, Top - 40.0f, 0.0f), 0.15f, State->GPUShaderVarArray[0]);

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

	Text_ClearGlobalStream();
}

void Game_Clean(ProgramState* State)
{
	Entity_DeleteBlock(State->EntityBlockNum);
	Object_DeleteBlock(State->ObjectBlockNum);
	Render_ClearCurrentShaderProgram();
	Render_DeleteShaderProgram(State->ShaderHandles[0]);
	Render_DeleteShaderProgram(State->ShaderHandles[1]);
	Render_DeleteShaderProgram(State->ShaderHandles[2]);
}