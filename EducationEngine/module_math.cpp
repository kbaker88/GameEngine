#include "module_math.h"

void
Module_Math_Load(ProgramState* State)
{
	window_properties WindowDimensions =
		Render_GetWindowProperties();
	float HalfScreenWidth = (float)WindowDimensions.Width * 0.5f;
	float HalfScreenHeight = (float)WindowDimensions.Height * 0.5f;

	State->CameraArray[0].Yaw = -90.0f;
	State->CameraArray[0].Pitch = 0.0f;
	State->CameraArray[0].UpVector = v3(0.0f, 1.0f, 0.0f);
	State->CameraArray[0].ForwardVector = v3(0.0f, 0.0f, -1.0f);
	State->CameraArray[0].ProjectionMatrix =
		Math_OrthographicMarix(0.0f, (float)WindowDimensions.Width,
			0.0f, (float)WindowDimensions.Height,
			0.1f, 100.0f);

	Camera_SetPosition(&State->CameraArray[0], &v3(-HalfScreenWidth,
		-HalfScreenHeight, 1.0f));

	State->ShaderHandles[0] =
		Render_CompileShaders(
			VertexShader_Title, FragmentShader_Title);
	State->ShaderHandles[1] =
		Render_CompileShaders(TextVertexShaderSource,
			TextFragmentShaderSource);
}

void
Module_Math_Run(ProgramState* State)
{

}

void
Module_Math_Clean()
{

}