#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "world_system.h"

class MenuState
{
public:
	MenuState();
	~MenuState();

	void Init(uint8 &programState);
	uint8 CheckInitialization();
	void Display();
	void DisplayButtons();
	void DisplayText();

	void CleanUp();

private:
	MyRectangle Button;

	Camera TempCamera;
	//m4 view;
	//m4 projection;
	//
	//v3 cameraPos;
	//v3 cameraUp;
	//v3 cameraFront;

	uint32 ProgramShaderHandle, TextShaderHandle;

	int32 ShaderVars[3];
	uint8 initialized;
	uint8 *StateOfProgram;
	int8 ErrorCheck;
	uint8 NeedToClean;
	
	uint32 ObjectInstances[8];
	//test
	//uint32 ObjectMaps[4];
	//

	window_properties WindowSize;

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
		"void main()\n"
		"{\n"
		//"FragColor = vec4(Color, 1.0);\n"
		"FragColor = texture2D(myTexture, TexCoord);\n" //* vec4(Color, 1.0);\n"
		"}\n\0";

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