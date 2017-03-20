#include "user_interface.h"

static uint32 EntityBlockNum = 0; //TODO: Remove This later, temporary

UserInterface::UserInterface() : ConsoleOn(0)
{

}

UserInterface::~UserInterface()
{
	 
}

void UserInterface::Init()
{
	window_properties WindowDimensions = Render_GetWindowProperties();
	float WindowHalfHeight = 0.5f * (float)WindowDimensions.Height;
	float WindowHalfWidth = 0.5f * (float)WindowDimensions.Width;

	TestCamera.SetPosition(&v3(-WindowHalfWidth,
			-WindowHalfHeight, 1.0f));
	TestCamera.SetProjectionMatrix(0);

	Top = WindowHalfHeight;
	Bottom = -WindowHalfHeight;
	Left = -WindowHalfWidth;
	Right = WindowHalfWidth;
	
	TextShaderHandle = Render_CompileShaders(TextVertexShaderSource,
		TextFragmentShaderSource);

	TestTimer.Start(); // TODO: Remove, testing only.
}

void UserInterface::Draw()
{
	//ViewMatrix = TestCamera.GetViewMatrix();

	Render_BindShaders(TextShaderHandle);
	ShaderVars[0] = Render_GetShaderVariable(TextShaderHandle, "model");
	ShaderVars[1] = Render_GetShaderVariable(TextShaderHandle, "view");
	ShaderVars[2] = Render_GetShaderVariable(TextShaderHandle, "projection");
	ShaderVars[3] = Render_GetShaderVariable(TextShaderHandle, "myTexture");

	Render_UpdateShaderVariable(3, ShaderVars[1], &(*TestCamera.GetViewMatrix()).Rc[0][0]); // TODO: clean this up
	Render_UpdateShaderVariable(3, ShaderVars[2], &(*TestCamera.GetProjectionMatrix()).Rc[0][0]);
	Render_UpdateShaderVariable(1, ShaderVars[3], 0);

	// TODO: Remove, Test timer and clock features
	Text_DrawCharLine(string("Elapsed Time: ") + string(Platform_FloatToChar(TestTimer.GetTime(), 1)),
		v3(Right - 200.0f, Top - 40.0f, 0.0f), 0.15f, ShaderVars[0]);

	// Display FPS
	Text_DrawCharLine(string("Frames Per Second: ") + 
		string(Platform_FloatToChar(DEBUG_GetFPS(), 1)), 
		v3(Right - 200.0f, Top - 20.0f,
		0.0f), 0.15f, ShaderVars[0]);

	v3 PlayerPosition = Entity_GetPosition(EntityBlockNum, GetPlayer(0)->PlayerEntityID);
	
	Text_DrawCharLine(string("Player Position: \0") +
		string(Platform_FloatToChar(PlayerPosition.x)) + string(" ") + 
		string(Platform_FloatToChar(PlayerPosition.y)) + string(" ") +
		string(Platform_FloatToChar(PlayerPosition.z)),
		v3(Left + 20.0f, Top - 20.0f, 0.0f), 0.15f, ShaderVars[0]);

	v2 CursorPosition = GetOrthoMousePosition();
	
	Text_DrawCharLine(string("Cursor Position: \0") +
		string(Platform_FloatToChar(CursorPosition.x)) + string(" ") +
		string(Platform_FloatToChar(CursorPosition.y)),
		v3(Left + 20.0f, Top - 40.0f, 0.0f), 0.15f, ShaderVars[0]);
	//Text_StepDown();
	//Text_DrawCharLine(string("Cursor Position: \0") +
	//	string(Platform_FloatToChar(CursorPosition.x)) + string(" ") +
	//	string(Platform_FloatToChar(CursorPosition.y)));

	Text_ClearGlobalStream();
}