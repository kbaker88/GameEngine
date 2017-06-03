#include "render_layer.h"

// OpenGL 2
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
static gl_uniform_2i *glUniform2i;
static gl_uniform_3i *glUniform3i;
static gl_uniform_4i *glUniform4i;
static gl_uniform_1iv *glUniform1iv;
static gl_uniform_2iv *glUniform2iv;
static gl_uniform_3iv *glUniform3iv;
static gl_uniform_4iv *glUniform4iv;
static gl_uniform_1ui *glUniform1ui;
static gl_uniform_2ui *glUniform2ui;
static gl_uniform_3ui *glUniform3ui;
static gl_uniform_4ui *glUniform4ui;
static gl_uniform_1uiv *glUniform1uiv;
static gl_uniform_2uiv *glUniform2uiv;
static gl_uniform_3uiv *glUniform3uiv;
static gl_uniform_4uiv *glUniform4uiv;
static gl_uniform_1f *glUniform1f;
static gl_uniform_2f *glUniform2f;
static gl_uniform_3f *glUniform3f;
static gl_uniform_4f *glUniform4f;
static gl_uniform_1fv *glUniform1fv;
static gl_uniform_2fv *glUniform2fv;
static gl_uniform_3fv *glUniform3fv;
static gl_uniform_4fv *glUniform4fv;
static gl_uniform_matrix_2fv *glUniformMatrix2fv;
static gl_uniform_matrix_3fv *glUniformMatrix3fv;
static gl_uniform_matrix_4fv *glUniformMatrix4fv;
static gl_uniform_matrix_2x3fv *glUniformMatrix2x3fv;
static gl_uniform_matrix_3x2fv *glUniformMatrix3x2fv;
static gl_uniform_matrix_2x4fv *glUniformMatrix2x4fv;
static gl_uniform_matrix_4x2fv *glUniformMatrix4x2fv;
static gl_uniform_matrix_3x4fv *glUniformMatrix3x4fv;
static gl_uniform_matrix_4x3fv *glUniformMatrix4x3fv;
static gl_delete_program *glDeleteProgram;
static gl_gen_buffers *glGenBuffers;
static gl_bind_buffer *glBindBuffer;
static gl_buffer_data *glBufferData;
static gl_enable_vertex_attrib_array *glEnableVertexAttribArray;
static gl_disable_vertex_attrib_array *glDisableVertexAttribArray;
static gl_delete_buffers *glDeleteBuffers;
static gl_unmap_buffer *glUnmapBuffer;

// OpenGL 3
static gl_gen_vertex_arrays *glGenVertexArrays;
static gl_bind_vertex_array *glBindVertexArray;
static gl_delete_vertex_arrays *glDeleteVertexArrays;
static gl_map_buffer_range *glMapBufferRange;

// OpenGL 4.2
static gl_tex_storage_2d *glTexStorage2D;

// OpenGL 4.3
static gl_bind_vertex_buffer *glBindVertexBuffer;
static gl_vertex_attrib_format *glVertexAttribFormat;
static gl_vertex_attrib_binding *glVertexAttribBinding;

// OpenGL 4.5
static gl_create_vertex_arrays* glCreateVertexArrays;
static gl_named_buffer_storage* glNamedBufferStorage;
static gl_enable_vertex_array_attrib* glEnableVertexArrayAttrib;
static gl_disable_vertex_array_attrib* glDisableVertexArrayAttrib;
static gl_vertex_array_vertex_buffer* glVertexArrayVertexBuffer;
static gl_vertex_array_attrib_binding* glVertexArrayAttribBinding;
static gl_vertex_array_attrib_ib_format* glVertexArrayAttribFormat;
static gl_create_buffers* glCreateBuffers;
static gl_create_textures* glCreateTextures;
static gl_texture_storage_2d* glTextureStorage2D;
static gl_texture_sub_image_2d* glTextureSubImage2D;

void 
Render_Init(window_properties *Window)
{
	WindowProperties.Width = Window->Width;
	WindowProperties.Height = Window->Height;

	Render_UpdateWindow(WindowProperties);

	glEnable(GL_DEPTH_TEST);

	// OpenGL 2
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
	glUniform1i = (gl_uniform_1i *)wglGetProcAddress("glUniform1i");
	glUniform2i = (gl_uniform_2i *)wglGetProcAddress("glUniform2i");
	glUniform3i = (gl_uniform_3i *)wglGetProcAddress("glUniform3i");
	glUniform4i = (gl_uniform_4i *)wglGetProcAddress("glUniform4i");
	glUniform1iv = (gl_uniform_1iv *)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (gl_uniform_2iv *)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (gl_uniform_3iv *)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (gl_uniform_4iv *)wglGetProcAddress("glUniform4iv");
	glUniform1ui = (gl_uniform_1ui *)wglGetProcAddress("glUniform1ui");
	glUniform2ui = (gl_uniform_2ui *)wglGetProcAddress("glUniform2ui");
	glUniform3ui = (gl_uniform_3ui *)wglGetProcAddress("glUniform3ui");
	glUniform4ui = (gl_uniform_4ui *)wglGetProcAddress("glUniform4ui");
	glUniform1uiv = (gl_uniform_1uiv *)wglGetProcAddress("glUniform1uiv");
	glUniform2uiv = (gl_uniform_2uiv *)wglGetProcAddress("glUniform2uiv");
	glUniform3uiv = (gl_uniform_3uiv *)wglGetProcAddress("glUniform3uiv");
	glUniform4uiv = (gl_uniform_4uiv *)wglGetProcAddress("glUniform4uiv");
	glUniform1f = (gl_uniform_1f *)wglGetProcAddress("glUniform1f");
	glUniform2f = (gl_uniform_2f *)wglGetProcAddress("glUniform2f");
	glUniform3f = (gl_uniform_3f *)wglGetProcAddress("glUniform3f");
	glUniform4f = (gl_uniform_4f *)wglGetProcAddress("glUniform4f");
	glUniform1fv = (gl_uniform_1fv *)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (gl_uniform_2fv *)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (gl_uniform_3fv *)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (gl_uniform_4fv *)wglGetProcAddress("glUniform4fv");
	glUniformMatrix2fv = (gl_uniform_matrix_2fv *)wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv = (gl_uniform_matrix_3fv *)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (gl_uniform_matrix_4fv *)wglGetProcAddress("glUniformMatrix4fv");
	glUniformMatrix2x3fv = (gl_uniform_matrix_2x3fv *)wglGetProcAddress("glUniformMatrix2x3fv");
	glUniformMatrix3x2fv = (gl_uniform_matrix_3x2fv *)wglGetProcAddress("glUniformMatrix3x2fv");
	glUniformMatrix2x4fv = (gl_uniform_matrix_2x4fv *)wglGetProcAddress("glUniformMatrix2x4fv");
	glUniformMatrix4x2fv = (gl_uniform_matrix_4x2fv *)wglGetProcAddress("glUniformMatrix4x2fv");
	glUniformMatrix3x4fv = (gl_uniform_matrix_3x4fv *)wglGetProcAddress("glUniformMatrix3x4fv");
	glUniformMatrix4x3fv = (gl_uniform_matrix_4x3fv *)wglGetProcAddress("glUniformMatrix4x3fv");
	glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
	glBufferData = (gl_buffer_data *)wglGetProcAddress("glBufferData");
	glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (gl_disable_vertex_attrib_array *)wglGetProcAddress("glDisableVertexAttribArray");
	glDeleteBuffers = (gl_delete_buffers *)wglGetProcAddress("glDeleteBuffers");
	glUnmapBuffer = (gl_unmap_buffer*)wglGetProcAddress("glUnmapBuffer");

	// OpenGL 3
	glGenVertexArrays = (gl_gen_vertex_arrays *)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (gl_delete_vertex_arrays *)wglGetProcAddress("glDeleteVertexArrays");
	glMapBufferRange = (gl_map_buffer_range *)wglGetProcAddress("glMapBufferRange");
	
	// OpenGL 4.2
	glTexStorage2D = (gl_tex_storage_2d *)wglGetProcAddress("glTexStorage2D");

	// OpenGL 4.3
	glBindVertexBuffer = (gl_bind_vertex_buffer *)wglGetProcAddress("glBindVertexBuffer");
	glVertexAttribFormat = (gl_vertex_attrib_format *)wglGetProcAddress("glVertexAttribFormat");
	glVertexAttribBinding = (gl_vertex_attrib_binding *)wglGetProcAddress("glVertexAttribBinding");
	
	// OpenGL 4.5
	glCreateVertexArrays = (gl_create_vertex_arrays *)wglGetProcAddress("glCreateVertexArrays");
	glNamedBufferStorage = (gl_named_buffer_storage *)wglGetProcAddress("glNamedBufferStorage");
	glEnableVertexArrayAttrib = (gl_enable_vertex_array_attrib *)wglGetProcAddress("glEnableVertexArrayAttrib");
	glDisableVertexArrayAttrib = (gl_disable_vertex_array_attrib *)wglGetProcAddress("glDisableVertexArrayAttrib");
	glVertexArrayVertexBuffer = (gl_vertex_array_vertex_buffer *)wglGetProcAddress("glVertexArrayVertexBuffer");
	glVertexArrayAttribBinding = (gl_vertex_array_attrib_binding *)wglGetProcAddress("glVertexArrayAttribBinding");
	glVertexArrayAttribFormat = (gl_vertex_array_attrib_ib_format *)wglGetProcAddress("glVertexArrayAttribFormat");
	glCreateBuffers = (gl_create_buffers*)wglGetProcAddress("glCreateBuffers");
	glCreateTextures = (gl_create_textures *)wglGetProcAddress("glCreateTextures");
	glTextureStorage2D = (gl_texture_storage_2d *)wglGetProcAddress("glTextureStorage2D");
	glTextureSubImage2D = (gl_texture_sub_image_2d *)wglGetProcAddress("glTextureSubImage2D");
}

void 
Render_UpdateWindow(window_properties Window)
{
	WindowProperties.Width = Window.Width;
	WindowProperties.Height = Window.Height;
	glViewport(0, 0, WindowProperties.Width, WindowProperties.Height); 
}

void 
Render_UpdateWindow(uint32 Width, uint32 Height)
{
	WindowProperties.Width = Width;
	WindowProperties.Height = Height;
	glViewport(0, 0, WindowProperties.Width, WindowProperties.Height);
	Platform_UpdateWindowSize(Width, Height);
}

void 
Render_ClearScreen(v4* Color)
{
	glClearColor(Color->x, Color->y, Color->z, Color->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

window_properties 
Render_GetWindowProperties()
{
	return WindowProperties;
}

uint32 
Render_CompileShaders(const char* VertexShaderSource,
	const char* FragmentShaderSource) 
{
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
	glCompileShader(VertexShader);

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
	glCompileShader(FragmentShader);

#if DEBUG_MODE
	GLint ErrorResult;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == 0)
	{
		Platform_TemporaryError("Error in GetShaderiv for VertexShader\n");
	}

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &ErrorResult);
	if (ErrorResult == 0)
	{
		Platform_TemporaryError("Error in GetShaderiv for FragmentShader\n");
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(FragmentShader, 256, &returnedlength, buffer);
		Platform_TemporaryError(buffer);
	}
#endif

	GLuint ShaderProgram;
	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, VertexShader);
#if DEBUG_MODE
	HWND Window = GetActiveWindow();
	glGetProgramiv(ShaderProgram, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(Window, "Error in Shader Attachment\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(Window, buffer, 0, 0);
	}
#endif
	glAttachShader(ShaderProgram, FragmentShader);
#if DEBUG_MODE
	glGetProgramiv(ShaderProgram, GL_ATTACHED_SHADERS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(Window, "Error in Shader Attachment\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(Window, buffer, 0, 0);
	}
#endif
	glLinkProgram(ShaderProgram);
#if DEBUG_MODE
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &ErrorResult);
	if (ErrorResult == GL_FALSE)
	{
		MessageBox(Window, "Error in Linking Shader Program\n", 0, 0);
		GLsizei returnedlength;
		char buffer[256];
		glGetShaderInfoLog(ShaderProgram, 256, &returnedlength, buffer);
		MessageBox(Window, buffer, 0, 0);
	}
#endif
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return ShaderProgram;
}

void 
Render_BindShaders(uint32 ShaderProgramHandle)
{
	GLint status;
	glGetProgramiv(ShaderProgramHandle, GL_LINK_STATUS, &status);
#if DEBUG_MODE
	if (status == GL_FALSE)
	{
		Platform_TemporaryError("Error in compiling the ShaderProgramHandle\n");
	}
	else
	{
		glUseProgram(ShaderProgramHandle);
	}
#else
	glUseProgram(ShaderProgramHandle);
#endif
}

void 
Render_ClearCurrentShaderProgram()
{
	glUseProgram(0);
}

void 
Render_DeleteShaderProgram(uint32 ShaderProgramHandle)
{
	glDeleteProgram(ShaderProgramHandle);
}

int32 
Render_GetShaderVariable(uint32 ShaderProgramHandle, char* name)
{
	return glGetUniformLocation(ShaderProgramHandle, name);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 Integer)
{
	glUniform1i(Location, Integer);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 IntegerX, 
	int32 IntegerY)
{
	glUniform2i(Location, IntegerX, IntegerY);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 IntegerX,
	int32 IntegerY, int32 IntegerZ)
{
	glUniform3i(Location, IntegerX, IntegerY, IntegerZ);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 IntegerX,
	int32 IntegerY, int32 IntegerZ, int32 IntegerW)
{
	glUniform4i(Location, IntegerX, IntegerY, IntegerZ, IntegerW);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 VectorSize,
	int32* IntegerArray, int32 ArraySize)
{
	switch (VectorSize)
	{
	case 1:
	{
		glUniform1iv(Location, ArraySize, IntegerArray);
	} break;
	case 2:
	{
		glUniform2iv(Location, ArraySize, IntegerArray);
	} break;
	case 3:
	{
		glUniform3iv(Location, ArraySize, IntegerArray);
	} break;
	case 4:
	{
		glUniform4iv(Location, ArraySize, IntegerArray);
	} break;
	default:
	{
		//TODO: Error, not a valid array size
	} break;
	}
}

void 
Render_UpdateShaderVariable(int32 Location, uint32 UnsignedInteger)
{
	glUniform1i(Location, UnsignedInteger);
}

void 
Render_UpdateShaderVariable(int32 Location, uint32 UnsignedIntegerX, 
	uint32 UnsignedIntegerY)
{
	glUniform2i(Location, UnsignedIntegerX, UnsignedIntegerY);
}

void 
Render_UpdateShaderVariable(int32 Location, uint32 UnsignedIntegerX,
	uint32 UnsignedIntegerY, uint32 UnsignedIntegerZ)
{
	glUniform3i(Location, UnsignedIntegerX, UnsignedIntegerY,
		UnsignedIntegerZ);
}

void 
Render_UpdateShaderVariable(int32 Location, uint32 UnsignedIntegerX,
	uint32 UnsignedIntegerY, uint32 UnsignedIntegerZ,
	uint32 UnsignedIntegerW)
{
	glUniform4i(Location, UnsignedIntegerX, UnsignedIntegerY, 
		UnsignedIntegerZ, UnsignedIntegerW);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 VectorSize,
	uint32* UnsignedIntegerArray, int32 ArraySize)
{
	switch (VectorSize)
	{
	case 1:
	{
		glUniform1uiv(Location, ArraySize, UnsignedIntegerArray);
	} break;
	case 2:
	{
		glUniform2uiv(Location, ArraySize, UnsignedIntegerArray);
	} break;
	case 3:
	{
		glUniform3uiv(Location, ArraySize, UnsignedIntegerArray);
	} break;
	case 4:
	{
		glUniform4uiv(Location, ArraySize, UnsignedIntegerArray);
	} break;
	default:
	{
		//TODO: Error, not a valid array size
	} break;
	}
}

void 
Render_UpdateShaderVariable(int32 Location, float Float)
{
	glUniform1f(Location, Float);
}

void 
Render_UpdateShaderVariable(int32 Location, float FloatX, 
	float FloatY)
{
	glUniform2f(Location, FloatX, FloatY);
}

void 
Render_UpdateShaderVariable(int32 Location, float FloatX,
	float FloatY, float FloatZ)
{
	glUniform3f(Location, FloatX, FloatY, FloatZ);
}

void 
Render_UpdateShaderVariable(int32 Location, float FloatX,
	float FloatY, float FloatZ, float FloatW)
{
	glUniform4f(Location, FloatX, FloatY, FloatZ, FloatW);
}

void 
Render_UpdateShaderVariable(int32 Location, int32 VectorSize,
	float* FloatArray, int32 ArraySize)
{
	switch (VectorSize)
	{
	case 1:
	{
		glUniform1fv(Location, ArraySize, FloatArray);
	} break;
	case 2:
	{
		glUniform2fv(Location, ArraySize, FloatArray);
	} break;
	case 3:
	{
		glUniform3fv(Location, ArraySize, FloatArray);
	} break;
	case 4:
	{
		glUniform4fv(Location, ArraySize, FloatArray);
	} break;
	default:
	{
		//TODO: Error, not a valid array size
	} break;
	}
}

void 
Render_UpdateShaderVariable(int32 Location, int32 MatrixSize, 
	float* NewData, int32 MatrixArraySize, bool Transpose)
{
	switch (MatrixSize)
	{
	case 22: 
	{	
		glUniformMatrix2fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 33: 
	{
		glUniformMatrix3fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 44:
	{
		glUniformMatrix4fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 23:
	{
		glUniformMatrix2x3fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 32:
	{
		glUniformMatrix3x2fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 24:
	{
		glUniformMatrix2x4fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 42:
	{
		glUniformMatrix4x2fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 34:
	{
		glUniformMatrix3x4fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	case 43:
	{
		glUniformMatrix4x3fv(Location, MatrixArraySize, Transpose, NewData);
	} break;
	default:
	{
		//TODO: Error, not a valid matrix size.
	} break;
	}
}

#if DATA_ORIENTED

void 
Render_CreateVertexArrays(uint32 Amount,
	uint32* IDArray)
{
	
	glCreateVertexArrays(Amount, IDArray);
}

void 
Render_CreateBuffers(uint32 Amount, uint32* BufferIDArray)
{
	glCreateBuffers(Amount, BufferIDArray);
}

void 
Render_FillBuffer(uint32 BufferID, uint32 Size,
	float* Data, uint32 Flags)
{
	glNamedBufferStorage(BufferID, Size, Data, Flags);
}

void 
Render_FillVertexArrayData(uint32 VertexArrayObject,
	uint32 BindingIndex, uint32 AttribIndex, uint32 Buffer,
	uint32 SpaceOffset, uint32 StartOffset)
{
	glVertexArrayVertexBuffer(VertexArrayObject, BindingIndex,
		Buffer, 0, sizeof(float) * SpaceOffset);
	glVertexArrayAttribFormat(VertexArrayObject, AttribIndex, 
		SpaceOffset, GL_FLOAT, GL_FALSE, StartOffset);
	glVertexArrayAttribBinding(VertexArrayObject, AttribIndex,
		BindingIndex);
	// TODO: Find when to disable or if it is even worth doing.
	glEnableVertexArrayAttrib(VertexArrayObject, BindingIndex);
}

void 
Render_FillVertexArrayIndices(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderObject->IndiceID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	//	RenderObject->IndiceCount * sizeof(unsigned int),
	//	RenderObject->IndiceData,
	//	GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void 
Render_FillVetexArrayObject(RenderObj* RenderObject,
	uint32 NumberAttribs, uint32* Offsets)
{
	for (uint32 i = 0; i < NumberAttribs; i++)
	{
		Render_FillVertexArrayData(RenderObject->VertexArrayID,
			i, i, RenderObject->BufferID[i], Offsets[i], 0);
	}
}

void 
Render_BindVertexArray(uint32 VertexArrayObject)
{
	glBindVertexArray(VertexArrayObject);
}

void
Render_BuildTexture(uint32* TextureID, uint32 Width,
	uint32 Height, uint8* TextureData)
{
	glCreateTextures(GL_TEXTURE_2D, 1, TextureID);
	
	glTextureStorage2D(*TextureID, 1, GL_RGBA8, Width,
		Height);
	glTextureSubImage2D(*TextureID, 0, 0, 0,
		Width, Height,
		GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
}

void
Render_BindTexture(uint32 TextureID)
{
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void 
Render_Draw(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	//glBindTexture(GL_TEXTURE_2D, Texture);

	glDrawArrays(GL_TRIANGLES, 0, RenderObject->NumVertices);

	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void
Render_DrawPoints(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	glDrawArrays(GL_POINTS, 0, RenderObject->NumVertices);
	glBindVertexArray(0);
}

void
Render_DrawLines(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	glDrawArrays(GL_LINES, 0, RenderObject->NumVertices);
	glBindVertexArray(0);
}

void
Render_DrawIndices(RenderObj* RenderObject)
{
	glBindVertexArray(RenderObject->VertexArrayID);
	glDrawElements(GL_TRIANGLES, RenderObject->IndiceID, GL_UNSIGNED_INT,
		(void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, RenderObject->NumVertices);
	glBindVertexArray(0);
}

#else
void 
Render_ObjectPipelineInit(PipelineObjectDescription* ObjectDescription)
{
	glGenBuffers(ObjectDescription->NumberOfVertexHandles,
		ObjectDescription->VertexBufferObjectHandleIDs);
	glGenVertexArrays(1, &ObjectDescription->VertexArrayObjectID);
	glBindVertexArray(ObjectDescription->VertexArrayObjectID);

	uint32 LoopCount = ObjectDescription->NumberOfVertexHandles;

	if (ObjectDescription->IndiceDescription.Data)
	{
		LoopCount--;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			ObjectDescription->VertexBufferObjectHandleIDs[LoopCount]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			ObjectDescription->IndiceDescription.Size,
			ObjectDescription->IndiceDescription.Data,
			GL_STATIC_DRAW);
	}

	for (uint32 Index = 0; Index < LoopCount; Index++)
	{
		glBindBuffer(GL_ARRAY_BUFFER,
			ObjectDescription->VertexBufferObjectHandleIDs[Index]);
		glBufferData(GL_ARRAY_BUFFER, 
			ObjectDescription->VertexBufferDescriptions[Index].Size,
			ObjectDescription->VertexBufferDescriptions[Index].Data,
			GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(Index);
		glBindVertexBuffer(Index, 
			ObjectDescription->VertexBufferObjectHandleIDs[Index],
			0, sizeof(GLfloat) * 
			ObjectDescription->VertexBufferDescriptions[Index].Offset);
		glVertexAttribFormat(Index,
			ObjectDescription->VertexBufferDescriptions[Index].Offset,
			GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(Index, Index);
	}
	// TODO: Should i be disabling the vertex attribute arrays? 

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	// NOTE: Do not unbind the EBO, keep it bound to this VAO
	glBindVertexArray(0); 
}

#endif

void* 
Render_GetObjectShaderDataPtr(uint32* VertexObjectHandleIDArray,
	int32 Offset, uint32 Length)
{
	// NOTE: Color Buffer is [1]
	glBindBuffer(GL_ARRAY_BUFFER, VertexObjectHandleIDArray[1]); 
	GLsizeiptr NewDataSize;
	NewDataSize = Length * 3 * sizeof(float);
	void* test = glMapBufferRange(GL_ARRAY_BUFFER, Offset,
		NewDataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	return test;
}

void 
Render_UnmapShaderDataPtr()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void 
Render_SetTexture(unsigned char* ImageData, int32 Width,
				int32 Height, uint32 *TextureID)
{
	glGenTextures(1, TextureID);
	glBindTexture(GL_TEXTURE_2D, *TextureID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, Width, Height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, GL_RGBA,
		GL_UNSIGNED_BYTE, ImageData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void 
Render_DrawPoint(uint32 VertexArrayObjectID,
	float PointSize, uint32 numVertices)
{
	glPointSize(PointSize);
	glBindVertexArray(VertexArrayObjectID);
	glDrawArrays(GL_POINTS, 0, numVertices);
	glBindVertexArray(0);
	glPointSize(1.0f);
}

void 
Render_DrawLine(uint32 VertexArrayObjectID,
	float LineSize, uint32 numVertices)
{
	glLineWidth(LineSize);
	glBindVertexArray(VertexArrayObjectID);
	glDrawArrays(GL_LINES, 0, numVertices);
	glBindVertexArray(0);
	glLineWidth(1.0f);
}

void 
Render_DrawObject(uint32 VertexArrayObjectID,
	uint32 &Texture, uint32 numVertices)
{
	glBindVertexArray(VertexArrayObjectID);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void 
Render_DrawObjectIndices(uint32 VertexArrayObjectID,
	uint32 &Texture, uint32 NumberOfIndices)
{
	glBindVertexArray(VertexArrayObjectID);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawElements(GL_TRIANGLES, NumberOfIndices, GL_UNSIGNED_INT, 
		(void*)0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void 
Render_UpdateColorVertice(uint32* VertexObjectHandleIDArray,
	uint32 ArrayPosition, float* Color)
{
	float* GPUMemoryMapPtr = (float*)Render_GetObjectShaderDataPtr(
		VertexObjectHandleIDArray, ArrayPosition, 1);
	*GPUMemoryMapPtr = Color[0];
	*++GPUMemoryMapPtr = Color[1];
	*++GPUMemoryMapPtr = Color[2];

	Render_UnmapShaderDataPtr();
}

void 
Render_DeleteTexture(uint32 NumberOfTextures, uint32 *TextureID)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(NumberOfTextures, TextureID);
}

void 
Render_DeleteVertexArrays(uint32 NumberOfVertexArrayObjects,
	uint32 *VAO)
{
	glBindVertexArray(0);
	glDeleteVertexArrays(NumberOfVertexArrayObjects, VAO);
}

void 
Render_DeleteBuffers(uint32 NumberOfBuffers, uint32 *Buffers)
{
	glDeleteBuffers(NumberOfBuffers, Buffers);
}

void 
Render_DeleteBuffers(uint32 NumberOfBuffers, 
	uint32 VertexArrayObjectID, uint32 *Buffers)
{
	glBindVertexArray(VertexArrayObjectID);
	glDeleteBuffers(NumberOfBuffers, Buffers);
}