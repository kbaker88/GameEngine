#include "render_layer.h"

// gl 2
static gl_create_shader *glCreateShader;
static gl_shader_source *glShaderSource;
static gl_compile_shader *glCompileShader;
static gl_get_shader_iv *glGetShaderiv;
static gl_create_program *glCreateProgram;
static gl_attach_shader *glAttachShader;
static gl_link_program *glLinkProgram;
static gl_detach_shader *glDetachShader;
static gl_delete_shader *glDeleteShader;
static gl_get_shader_info_log *glGetShaderInfoLog;
static gl_get_program_iv *glGetProgramiv;
static gl_use_program *glUseProgram;
static gl_get_uniform_location *glGetUniformLocation;
static gl_uniform_1i *glUniform1i;
static gl_uniform_3f *glUniform3f;
static gl_uniform_matrix_4fv *glUniformMatrix4fv;
static gl_delete_program *glDeleteProgram;
static gl_gen_buffers *glGenBuffers;
static gl_bind_buffer *glBindBuffer;
static gl_buffer_data *glBufferData;
static gl_enable_vertex_attrib_array *glEnableVertexAttribArray;
static gl_delete_buffers *glDeleteBuffers;
static gl_unmap_buffer *glUnmapBuffer;

// gl 3
static gl_gen_vertex_arrays *glGenVertexArrays;
static gl_bind_vertex_array *glBindVertexArray;
static gl_delete_vertex_arrays *glDeleteVertexArrays;
static gl_map_buffer_range *glMapBufferRange;

// gl 4
static gl_bind_vertex_buffer *glBindVertexBuffer;
static gl_vertex_attrib_format *glVertexAttribFormat;
static gl_vertex_attrib_binding *glVertexAttribBinding;
static gl_tex_storage_2d *glTexStorage2D;

void Render_Init(window_properties Window)
{
	WindowProperties = Window;

	Render_UpdateWindow(WindowProperties);

	glEnable(GL_DEPTH_TEST);

	// gl2
	glCreateShader = (gl_create_shader *)wglGetProcAddress("glCreateShader");
	glShaderSource = (gl_shader_source *)wglGetProcAddress("glShaderSource");
	glCompileShader = (gl_compile_shader *)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (gl_get_shader_iv *)wglGetProcAddress("glGetShaderiv");
	glCreateProgram = (gl_create_program *)wglGetProcAddress("glCreateProgram");
	glAttachShader = (gl_attach_shader *)wglGetProcAddress("glAttachShader");
	glLinkProgram = (gl_link_program *)wglGetProcAddress("glLinkProgram");
	glDetachShader = (gl_detach_shader *)wglGetProcAddress("glDetachShader");
	glDeleteShader = (gl_delete_shader *)wglGetProcAddress("glDeleteShader");
	glGetShaderInfoLog = (gl_get_shader_info_log *)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramiv = (gl_get_program_iv *)wglGetProcAddress("glGetProgramiv");
	glUseProgram = (gl_use_program *)wglGetProcAddress("glUseProgram");
	glGetUniformLocation = (gl_get_uniform_location *)wglGetProcAddress("glGetUniformLocation");
	glDeleteProgram = (gl_delete_program *)wglGetProcAddress("glDeleteProgram");
	glUniformMatrix4fv = (gl_uniform_matrix_4fv *)wglGetProcAddress("glUniformMatrix4fv");
	glUniform3f = (gl_uniform_3f *)wglGetProcAddress("glUniform3f");
	glUniform1i = (gl_uniform_1i *)wglGetProcAddress("glUniform1i");
	glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
	glBufferData = (gl_buffer_data *)wglGetProcAddress("glBufferData");
	glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
	glDeleteBuffers = (gl_delete_buffers *)wglGetProcAddress("glDeleteBuffers");
	glUnmapBuffer = (gl_unmap_buffer*)wglGetProcAddress("glUnmapBuffer");

	// gl3
	glGenVertexArrays = (gl_gen_vertex_arrays *)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (gl_delete_vertex_arrays *)wglGetProcAddress("glDeleteVertexArrays");
	glMapBufferRange = (gl_map_buffer_range *)wglGetProcAddress("glMapBufferRange");
	
	// gl4
	glBindVertexBuffer = (gl_bind_vertex_buffer *)wglGetProcAddress("glBindVertexBuffer");
	glVertexAttribFormat = (gl_vertex_attrib_format *)wglGetProcAddress("glVertexAttribFormat");
	glVertexAttribBinding = (gl_vertex_attrib_binding *)wglGetProcAddress("glVertexAttribBinding");
	glTexStorage2D = (gl_tex_storage_2d *)wglGetProcAddress("glTexStorage2D");
}

void Render_UpdateWindow(window_properties Window)
{
	WindowProperties.Width = Window.Width;
	WindowProperties.Height = Window.Height;
	glViewport(0, 0, WindowProperties.Width, WindowProperties.Height); 
	//UpdateWindowSize(Window.Width, Window.Height);
}

void Render_UpdateWindow(uint32 Width, uint32 Height)
{
	WindowProperties.Width = Width;
	WindowProperties.Height = Height;
	glViewport(0, 0, WindowProperties.Width, WindowProperties.Height);
	Platform_UpdateWindowSize(Width, Height);
}

void Render_ClearScreen()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

window_properties Render_GetWindowProperties()
{
	return WindowProperties;
}

uint32 Render_CompileShaders(const char* VertexShaderSource,
	const char* FragmentShaderSource) 
{
	GLuint ShaderProgramHandle;

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (VertexShader == 0)
	{
		Platform_TemporaryError("Error in CreateShader for VertexShader\n");
	}

	const char* codeArray[] = { VertexShaderSource };
	glShaderSource(VertexShader, 1, codeArray, 0);
	glCompileShader(VertexShader);

	GLint result;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		Platform_TemporaryError("Error in GetShaderiv for VertexShader\n");
	}

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (FragmentShader == 0)
	{
		Platform_TemporaryError("Error in CreateShader for FragmentShader\n");
	}

	const char* codeArray2[] = { FragmentShaderSource };
	glShaderSource(FragmentShader, 1, codeArray2, 0);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		Platform_TemporaryError("Error in GetShaderiv for FragmentShader\n");
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(FragmentShader, 256, &returnedlength, buffer);
		Platform_TemporaryError(buffer);
	}

	ShaderProgramHandle = glCreateProgram();
	if (ShaderProgramHandle == 0)
	{
		Platform_TemporaryError("Error in creating the ShaderProgramHandle\n");
	}

	glAttachShader(ShaderProgramHandle, VertexShader);
	glAttachShader(ShaderProgramHandle, FragmentShader);

	glLinkProgram(ShaderProgramHandle);

	glDetachShader(ShaderProgramHandle, VertexShader);
	glDetachShader(ShaderProgramHandle, FragmentShader);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return ShaderProgramHandle;
}

void Render_BindShaders(uint32 ShaderProgramHandle)
{
	GLint status;
	glGetProgramiv(ShaderProgramHandle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		Platform_TemporaryError("Error in compiling the ShaderProgramHandle\n");
	}
	else
	{
		glUseProgram(ShaderProgramHandle);
	}
}

void Render_ClearCurrentShaderProgram()
{
	glUseProgram(0);
}

void Render_DeleteShaderProgram(uint32 ShaderProgramHandle)
{
	glDeleteProgram(ShaderProgramHandle);
}


int32 Render_GetShaderVariable(uint32 ShaderProgramHandle, char* name)
{
	int32 ReturnValue;

	ReturnValue = glGetUniformLocation(ShaderProgramHandle, name);

	return ReturnValue;
}

void Render_UpdateShaderVariable(int32 UniformVariable, int32 Integer)
{
	glUniform1i(UniformVariable, Integer);
}

void Render_UpdateShaderVariable(int32 Type, int32 UniformVariable, float* NewData)
{
	switch (Type)
	{
	case 0: 
	{
	} break;
	case 1: // Texture
	{
		glUniform1i(UniformVariable, 0);
	} break;
	case 2: // vector of 3 floats
	{
		glUniform3f(UniformVariable, NewData[0], NewData[1], NewData[2]);
	} break;
	case 3: // 4x4 matrix of floats
	{
		glUniformMatrix4fv(UniformVariable, 1, GL_FALSE, NewData);
	} break;
	case 4:
	{
		glUniform1i(UniformVariable, (int)*NewData);
	} break;
	}
}

void Render_ObjectPipelineInit(
	PipelineObjectDescription& ObjectDescription)
{
	glGenBuffers(ObjectDescription.NumberOfVertexHandles,
		ObjectDescription.VertexBufferObjectHandles);
	glGenVertexArrays(1, &ObjectDescription.VertexArrayObject);
	glBindVertexArray(ObjectDescription.VertexArrayObject);

	uint32 Index = 0;

	for (Index; Index < ObjectDescription.NumberOfVertexHandles; Index++)
	{
		if (ObjectDescription.Description[0].IndiceData == 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, ObjectDescription.VertexBufferObjectHandles[Index]);
			glBufferData(GL_ARRAY_BUFFER, ObjectDescription.Description[Index].Size,
				ObjectDescription.Description[Index].Data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(Index);
			glBindVertexBuffer(Index, ObjectDescription.VertexBufferObjectHandles[Index],
				0, sizeof(GLfloat) * ObjectDescription.Description[Index].Offset);
			glVertexAttribFormat(Index, ObjectDescription.Description[Index].Offset, GL_FLOAT, GL_FALSE, 0);
			glVertexAttribBinding(Index, Index);
		}
		else
		{
			if (Index == 0)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ObjectDescription.VertexBufferObjectHandles[Index]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, ObjectDescription.Description[Index].Size,
					ObjectDescription.Description[Index].IndiceData, GL_STATIC_DRAW);
				Index++;
			}

			glBindBuffer(GL_ARRAY_BUFFER, ObjectDescription.VertexBufferObjectHandles[Index]);
			glBufferData(GL_ARRAY_BUFFER, ObjectDescription.Description[Index].Size,
				ObjectDescription.Description[Index].Data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(Index - 1);
			glBindVertexBuffer(Index - 1, ObjectDescription.VertexBufferObjectHandles[Index],
				0, sizeof(GLfloat) * ObjectDescription.Description[Index].Offset);
			glVertexAttribFormat(Index - 1, ObjectDescription.Description[Index].Offset, GL_FLOAT, GL_FALSE, 0);
			glVertexAttribBinding(Index - 1, Index - 1);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void* Render_GetObjectShaderDataPtr(PipelineObjectDescription& Description, int32 Offset, uint32 Length)
{
	glBindBuffer(GL_ARRAY_BUFFER, Description.VertexBufferObjectHandles[2]);
	GLsizeiptr NewDataSize;
	NewDataSize = Length * 3 * sizeof(float);
	void* test = glMapBufferRange(GL_ARRAY_BUFFER, Offset,
		NewDataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	return test;
}

void Render_UnmapShaderDataPtr()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render_SetTexture(unsigned char* ImageData, int32 Width,
				int32 Height, uint32 &TextureID)
{
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, Width, Height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, GL_RGBA,
		GL_UNSIGNED_BYTE, ImageData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Render_DrawPoint(PipelineObjectDescription &ObjectDescription,
	float PointSize, uint32 numVertices)
{
	glPointSize(PointSize);
	glBindVertexArray(ObjectDescription.VertexArrayObject);
	glDrawArrays(GL_POINTS, 0, numVertices);
	glBindVertexArray(0);
	glPointSize(1.0f);
}

void Render_DrawLine(PipelineObjectDescription &ObjectDescription,
	float LineSize, uint32 numVertices)
{
	glLineWidth(LineSize);
	glBindVertexArray(ObjectDescription.VertexArrayObject);
	glDrawArrays(GL_LINES, 0, numVertices);
	glBindVertexArray(0);
	glLineWidth(1.0f);
}

void Render_DrawObject(PipelineObjectDescription &ObjectDescription,
	uint32 &Texture, uint32 numVertices)
{
	glBindVertexArray(ObjectDescription.VertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Render_DrawObjectIndices(PipelineObjectDescription &ObjectDescription,
	uint32 &Texture, uint32 NumberOfIndices)
{
	glBindVertexArray(ObjectDescription.VertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawElements(GL_TRIANGLES, NumberOfIndices, GL_UNSIGNED_INT, (void*)0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Render_UpdateColorVertice(PipelineObjectDescription& Description,
	uint32 ArrayPosition, float* Color)
{
	float* GPUMemoryMapPtr = (float*)Render_GetObjectShaderDataPtr(
		Description, ArrayPosition, 1);
	*GPUMemoryMapPtr = Color[0];
	*++GPUMemoryMapPtr = Color[1];
	*++GPUMemoryMapPtr = Color[2];

	Render_UnmapShaderDataPtr();
}

void Render_DeleteTextureBuffer(uint32 Size, uint32 &TextureID)
{
	glDeleteTextures(Size, &TextureID);
}

void Render_DeleteVertexArrays(uint32 Size, uint32 *VAO)
{
	glDeleteVertexArrays(1, VAO);
}

void Render_DeleteBuffers(uint32 Size, uint32 *Buffers)
{
	glDeleteBuffers(4, Buffers);
}
