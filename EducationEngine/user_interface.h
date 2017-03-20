#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "world_terrain.h"
#include "clock.h" // TODO: move this later. for testing only
#include "camera.h" // TODO: move this later. for testing only

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	void Init();
	void Draw();

private:

	Timer TestTimer; // TODO: Remove, testing only
	Camera TestCamera; // TODO: Remove, testing only

	int32 ShaderVars[4];
	uint32 TextShaderHandle;
	float Top, Right, Bottom, Left;
	uint8 ConsoleOn;

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
		//"if (texture2D(myTexture, TexCoord).rgb == vec3(1.0,1.0,1.0))\n"
		//	"discard;\n"
		"if (texture2D(myTexture, TexCoord).rgb == vec3(0.0,0.0,0.0))\n"
		"discard;\n"
		"FragColor = texture2D(myTexture, TexCoord) * vec4(0.0, 1.0, 0.0, 1.0);\n" //* vec4(Color, 1.0);\n"
		"}\0";
};

#endif