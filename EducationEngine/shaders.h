#ifndef SHADERS_H
#define SHADERS_H

static const char* VertexShader_Title =
{
	"#version 450 core												\n"
	"																\n"
	"layout (location = 0) in vec3 PositionData;					\n"
	"layout (location = 1) in vec3 ColorData;						\n"
	"layout (location = 2) in vec2 TexturePos;						\n"
	"																\n"
	"uniform mat4 model;											\n"
	"uniform mat4 view;												\n"
	"uniform mat4 projection;										\n"
	"																\n"
	"out vec3 color;												\n"
	"out vec2 TexPosition;											\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"																\n"
	"color = ColorData;												\n"
	"TexPosition = TexturePos;										\n"
	"gl_Position = projection * view * model *						\n"
	"	vec4(PositionData, 1.0);									\n"
	"}																\n"
};

static const char* FragmentShader_Title =
{
	"#version 450 core												\n"
	"																\n"
	"in vec3 color;													\n"
	"in vec2 TexPosition;											\n"
	"																\n"
	"uniform sampler2D myTexture;									\n"
	"uniform vec3 HoverColor;										\n"
	"																\n"
	"out vec4 FragColor;											\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"if (texture2D(myTexture, TexPosition).rgb ==					\n"
	"								vec3(1.0, 1.0, 1.0))			\n"
	"{																\n"
	"	discard;													\n"
	"}																\n"
	"else if (texture2D(myTexture, TexPosition).rgb ==				\n"
	"								vec3(0.0, 0.0, 0.0))			\n"
	"{																\n"
	"FragColor =													\n"
	"	vec4(color, 1.0);											\n"
	"}																\n"
	"else															\n"
	"{																\n"
	"FragColor = texture2D(myTexture, TexPosition) *				\n"
	"	vec4(HoverColor, 1.0);										\n"
	"}																\n"
	"																\n"
	"}																\n"
};

static const char* VertexShader_Game =
{
	"#version 450 core												\n"
	"																\n"
	"layout (location = 0) in vec3 PositionData;					\n"
	"layout (location = 1) in vec3 ColorData;						\n"
	"layout (location = 2) in vec2 TexturePos;						\n"
	"																\n"
	"uniform mat4 model;											\n"
	"uniform mat4 view;												\n"
	"uniform mat4 projection;										\n"
	"																\n"
	"out vec3 color;												\n"
	"out vec2 TexPosition;											\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"																\n"
	"color = ColorData;												\n"
	"TexPosition = TexturePos;										\n"
	"gl_Position = projection * view * model *						\n"
	"	vec4(PositionData, 1.0);									\n"
	"}																\n"
};

static const char* FragmentShader_Game =
{
	"#version 450 core												\n"
	"																\n"
	"in vec3 color;													\n"
	"in vec2 TexPosition;											\n"
	"																\n"
	"uniform sampler2D myTexture;									\n"
	"uniform float TextureFlag;										\n"
	"																\n"
	"out vec4 FragColor;											\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"FragColor = TextureFlag * texture2D(myTexture, TexPosition) *  \n"
	"	vec4(color, 1.0) +	(1 - TextureFlag) * vec4(color, 1.0);	\n"
	"}																\n"
};

static const char* VertexShader_Text =
{
	"#version 450 core												\n"
	"																\n"
	"layout (location = 0) in vec3 PositionData;					\n"
	"layout (location = 1) in vec3 ColorData;						\n"
	"layout (location = 2) in vec2 TexturePos;						\n"
	"																\n"
	"uniform mat4 model;											\n"
	"uniform mat4 view;												\n"
	"uniform mat4 projection;										\n"
	"																\n"
	"out vec3 color;												\n"
	"out vec2 TexPosition;											\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"																\n"
	"color = ColorData;												\n"
	"TexPosition = TexturePos;										\n"
	"gl_Position = projection * view * model *						\n"
	"	vec4(PositionData, 1.0);									\n"
	"}																\n"
};

static const char* FragmentShader_Text =
{
	"#version 450 core												\n"
	"																\n"
	"in vec3 color;													\n"
	"in vec2 TexPosition;											\n"
	"																\n"
	"uniform sampler2D myTexture;									\n"
	"uniform vec3 TextColor;										\n"
	"																\n"
	"out vec4 FragColor;											\n"
	"																\n"
	"void main(void)												\n"
	"{																\n"
	"if (texture2D(myTexture, TexPosition).rgb ==					\n"
	"								vec3(0.0, 0.0, 0.0))			\n"
	"{																\n"
	"	discard;													\n"
	"}																\n"
	"FragColor = texture2D(myTexture, TexPosition) *				\n"
	"	vec4(TextColor, 1.0);										\n"
	"																\n"
	"}																\n"
};

static const char* TextVertexShaderSource = "#version 430 core\n"
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

static const char* TextFragmentShaderSource = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"void main()\n"
"{\n"
"if (texture2D(myTexture, TexCoord).rgb == vec3(0.0,0.0,0.0))\n"
"{\n"
"discard;\n"
"}\n"
"FragColor = texture2D(myTexture, TexCoord) * vec4(1.0, 0.0, 0.0, 1.0);\n" //* vec4(Color, 1.0);\n"
"}\0";

#if 0
// TODO: Temporary file until shaders are pre-compiled.
//TODO: Include shader header with shader version defined
static const char* MenuVertexShaderSource = "#version 430 core\n"
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

static const char* MenuFragmentShaderSource = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"uniform int mouseOver;\n"
"void main()\n"
"{\n"
"if (mouseOver == 1.0f) {\n"
"FragColor = texture2D(myTexture, TexCoord) * vec4(0.0f, 1.0f, 0.0f, 1.0);\n"
"} else {\n"
"FragColor = texture2D(myTexture, TexCoord);\n"
"}\n"
"}\0";

////////////////////FOR TEXT ///////////////////////

static const char* TextVertexShaderSource = "#version 430 core\n"
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

static const char* TextFragmentShaderSource = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"void main()\n"
"{\n"
"if (texture2D(myTexture, TexCoord).rgb == vec3(0.0,0.0,0.0))\n"
"{\n"
"discard;\n"
"}\n"
"FragColor = texture2D(myTexture, TexCoord) * vec4(0.0, 0.0, 0.0, 1.0);\n" //* vec4(Color, 1.0);\n"
"}\0";

static const char* VertexShaderSource1 = "#version 430 core\n"
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

static const char* FragmentShaderSource1 = "#version 430 core\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform sampler2D myTexture;\n"
"void main()\n"
"{\n"
//"FragColor = vec4(Color, 1.0);\n"
"FragColor = texture2D(myTexture, TexCoord);\n" //* vec4(Color, 1.0);\n"
"}\n\0";

static const char* VertShaderForTextureAndLight = "#version 430 core\n"
"layout (location = 0) in vec3 VertexPosition;\n"
"layout (location = 1) in vec3 VertexColor;\n"
"layout (location = 2) in vec2 TextureCoord;\n"
"layout (location = 3) in vec3 NormalPosition;\n"

"out vec3 Color;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec3 LightPos;\n"
"out vec2 TexCoord;\n"

"uniform vec3 lightPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"Color = VertexColor;\n"
"TexCoord = TextureCoord;\n"
"gl_Position = projection * view * model * vec4(VertexPosition, 1.0);\n"
"FragPos = vec3(model * vec4(VertexPosition, 1.0f));\n"
"Normal = mat3(transpose(inverse(view * model))) * NormalPosition;\n"
"LightPos = vec3(view * vec4(lightPos, 1.0));\n"
"}\0";

static const char* FragShaderForTextureAndLight = "#version 430 core\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec3 LightPos;\n"
"in vec3 Color;\n"
"in vec2 TexCoord;\n"

"out vec4 color;\n"

"uniform vec3 lightColor;\n"
"uniform sampler2D myTexture;\n"
"uniform vec3 viewPos;\n"
"uniform bool isTextured;\n"

"void main()\n"
"{\n"
"float ambientStrength = 0.5f;\n" // Ambient Light
"vec3 ambient = ambientStrength * lightColor;\n" // Ambient Light

"vec3 norm = normalize(Normal);\n" // Diffuse Light
"vec3 lightDir = normalize(LightPos - FragPos);\n" // Diffuse Light
"float diff = max(dot(norm, lightDir), 0.0);\n" // Diffuse Light
"vec3 diffuse = diff * lightColor;\n" // Diffuse Light

"float specularStrength = 1.5f;\n" // Specular Light
"vec3 viewDir = normalize(-FragPos);\n" // Specular Light
"vec3 reflectDir = reflect(-lightDir, norm);\n" // Specular Light
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n" // Specular Light
"vec3 specular = specularStrength * spec * lightColor;\n" // Specular Light
"vec3 result = (ambient + diffuse + specular) * Color;\n"

"if (isTextured)\n"
"{\n"
"color = vec4(result, 1.0f) * texture2D(myTexture, TexCoord);\n"
"}\n"
"else\n"
"{\n"
"color = vec4(result, 1.0f);\n" //* texture2D(myTexture, TexCoord);\n" //vec4(result, 1.0f);\n"
"}\n"
"}\0";

////////////////////////////////////////

static const char* TestVertexShader = "#version 430 core\n"
"layout (location = 0) in vec3 VertexPosition;\n"
"layout (location = 1) in vec3 VertexColor;\n"

"out vec3 Color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"Color = VertexColor;\n"
"gl_Position = projection * view * model * vec4(VertexPosition, 1.0);\n"
"}\0";

static const char* TestFragShader = "#version 430 core\n"
"in vec3 Color;\n"

"out vec4 color;\n"

"void main()\n"
"{\n"
"color = vec4(Color, 1.0f);\n"

"}\n\0";
#endif // 0

#endif
