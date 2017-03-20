#include "world_system.h"

static uint32 EntityBlockNum = 0; //TODO: Remove This later, temporary

v3 LightPosition = { 2.0f, 1.0f, -17.0f }; //TODO: Move into a shader struct?
v3 LightColor = {1.0f, 1.0f, 1.0f};
Camera Player1Camera; //TODO: Move this, testing only location

void InitializeWorld()
{
	Entity_CreateBlock(EntityBlockNum, 256);
	ProgramShaderHandle = Render_CompileShaders(VertShaderForTextureAndLight,
		FragShaderForTextureAndLight);
	TerrainShaderHandle = Render_CompileShaders(TestVertexShader,
		TestFragShader);

	if (!GetPlayer(0)->InitPlayer(EntityBlockNum, 0, v3(0.0f, 6.0f, 20.0f)))
	{
		Platform_TemporaryError("\nGetPlayer Problem\n");
	}
	Player1Camera.SetPosition(&v3(0.0f, 6.0f, 20.0f));//TODO: Link in the manager player1camera to player 1
	Player1Camera.SetFrontDirection(&v3(0.0f, 0.0f, -1.0f));
	Player1Camera.SetProjectionMatrix(1);

	Pysc_SetAccelerationRate(Entity_GetPhysObjPtr(EntityBlockNum, GetPlayer(0)->PlayerEntityID), 1000.0f);

	InitWorldTerrain();
	InitWorldObjects();

	ActiveWorldChunks = 1;
}

void DrawWorld()
{
	void* Player1 = GetPlayer(0);
	static_cast<Player*>(Player1)->UpdateRotation();
	Player1Camera.SetFrontDirection(
		static_cast<Player*>(Player1)->GetFrontDirection());
	static_cast<Player*>(Player1)->UpdateTranslation();

	Phys_SetMoveDirection(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID),
		*static_cast<Player*>(Player1)->GetDirection());
	Phys_CalculatePosition(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID));
	Player1Camera.SetPosition(
		&Entity_GetPosition(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID));

	Phys_AddForce(Entity_GetPhysObjPtr(EntityBlockNum, static_cast<Player*>(Player1)->PlayerEntityID), &Gravity);

	// Bind New Shaders
	Render_BindShaders(ProgramShaderHandle);
	ShaderVars[0] = Render_GetShaderVariable(ProgramShaderHandle,"model");
	ShaderVars[1] = Render_GetShaderVariable(ProgramShaderHandle,"view");
	ShaderVars[2] = Render_GetShaderVariable(ProgramShaderHandle,"projection");
	ShaderVars[3] = Render_GetShaderVariable(ProgramShaderHandle,"lightColor");
	ShaderVars[4] = Render_GetShaderVariable(ProgramShaderHandle,"lightPos");
	ShaderVars[5] = Render_GetShaderVariable(ProgramShaderHandle,"myTexture");
	ShaderVars[6] = Render_GetShaderVariable(ProgramShaderHandle, "isTextured");

	// Update Shader Variables for Player 1
	Render_UpdateShaderVariable(3, ShaderVars[1],
		(float*)Player1Camera.GetViewMatrix());
	Render_UpdateShaderVariable(3, ShaderVars[2],
		(float*)Player1Camera.GetProjectionMatrix());

	// Update Shader Variables for World
	Render_UpdateShaderVariable(2, ShaderVars[3], LightColor.Arr);
	Render_UpdateShaderVariable(2, ShaderVars[4], LightPosition.Arr);
	Render_UpdateShaderVariable(1, ShaderVars[5], 0);

	float Choice = 0;
	Render_UpdateShaderVariable(4, ShaderVars[6], &Choice);
	DrawWorldTerrain(ShaderVars[0]);
	DrawWorldObjects(ShaderVars[0], ShaderVars[6]);

}

void WorldCleanUp()
{
	Entity_DeleteBlock(EntityBlockNum);
	Object_ClearAll();
	Render_DeleteShaderProgram(ProgramShaderHandle);
}