#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include "utility.h"
#include "opengl.h"

static window_properties WindowProperties;

struct VBODescription
{
	VBODescription() : FloatData(NULL), Uint32Data(NULL),
		Size(0), Offset(0) {}

	float* FloatData;
	uint32* Uint32Data;
	uint32 Size;
	uint32 Offset;
};

struct PipelineObjectDescription
{
	PipelineObjectDescription() : VertexArrayObjectID(0),
		NumberOfVertexHandles(0), VertexBufferObjectHandleIDs(NULL),
		VertexBufferDescriptions(NULL) {}

	uint32 VertexArrayObjectID;
	uint32 NumberOfVertexHandles;
	uint32* VertexBufferObjectHandleIDs; 
	VBODescription* VertexBufferDescriptions; 
};

// General Functions
void Render_Init(window_properties Window);
void Render_UpdateWindow(window_properties Window);
void Render_UpdateWindow(uint32 Width, uint32 Height);
void Render_ClearScreen();
window_properties Render_GetWindowProperties(); // Should this be here?

// Shader Functions
uint32 Render_CompileShaders(const char* VertexShaderSource,
	const char* FragmentShaderSource);
void Render_BindShaders(uint32 ShaderProgramHandle);
void Render_ClearCurrentShaderProgram();
void Render_DeleteShaderProgram(uint32 ShaderProgramHandle);
int32 Render_GetShaderVariable(uint32 ShaderProgramHandle, char* name);
void Render_UpdateShaderVariable(int32 UniformVariable, int32 Integer);
void Render_UpdateShaderVariable(int32 Type, int32 UniformVariable,
	float* NewData);

// Object Rendering Functions
void Render_ObjectPipelineInit(
	PipelineObjectDescription& ObjectDescription);
void Render_SetTexture(unsigned char* ImageData, int32 Width,
				int32 Height, uint32 *TextureID);
void Render_DrawPoint(PipelineObjectDescription &ObjectDescription,
	float PointSize, uint32 numVertices);
void Render_DrawLine(PipelineObjectDescription &ObjectDescription,
	float LineSize, uint32 numVertices);
void Render_DrawObject(PipelineObjectDescription &ObjectDescription,
	uint32 &Texture, uint32 numVertices);
void Render_DrawObjectIndices(PipelineObjectDescription &ObjectDescription,
	uint32 &Texture, uint32 NumberOfIndices);

void Render_UpdateColorVertice(PipelineObjectDescription& Description,
	uint32 ArrayPosition, float* Color);

void* Render_GetObjectShaderDataPtr(PipelineObjectDescription& Description,
	int32 Offset, uint32 Length);
void Render_UnmapShaderDataPtr();
void Render_DeleteTexture(uint32 NumberOfTextures, uint32 *TextureID);
void Render_DeleteVertexArrays(uint32 NumberOfVertexArrayObjects, uint32 *VAO);
void Render_DeleteBuffers(uint32 NumberOfBuffers, uint32 *Buffers); //TODO: Will this one ever be used?
void Render_DeleteBuffers(uint32 NumberOfBuffers, 
	uint32 VertexArrayObjectID, uint32 *Buffers);

#endif
