#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include "game_state.h"

//TODO: Use glsl uniform buffers
//TODO: Testing Git

class TitleState
{
public:
	TitleState();
	~TitleState();

	void Init(uint8 &programState);
	uint8 CheckInitialization();
	void Display();
	void DisplayButtons();
	void DisplayConsole();
	void CleanUp();

private:
	window_properties WindowProperties;

	Camera TempCamera;
	int32 ShaderVars[4];

	uint32 ObjectInstances[6];
	uint32 ProgramShaderHandle, TextShaderHandle;

	uint8 initialized, ConsoleOn, NeedToClean, mouseoverstart;
	uint8 *StateOfProgram;

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
};

#endif