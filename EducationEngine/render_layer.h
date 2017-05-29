#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include "utility.h"
#include "opengl.h"
#include <vulkan\vulkan.h>

#define RENDER_OPENGL 1
#define RENDER_VULKAN 0
#define RENDER_DIRECTX12 0

#if DATA_ORIENTED
#if RENDER_OPENGL

struct RenderObj  // Byte Total: 34
{
	// TODO: Is it better to pass as one big buffer?
	uint32* BufferID;					// Bytes 8
	uint32 VertexArrayID, NumVertices,  // Bytes 12
		IndiceID;
};

void 
Render_CreateVertexArrays(uint32 Amount,
	uint32* IDArray);
void 
Render_CreateBuffers(uint32 Amount, 
	uint32* BufferIDArray);
void 
Render_FillBuffer(uint32 BufferID, uint32 Size,
	float* Data, uint32 Flags);

//NOTE: Doesn't need pre-binding to vertex array object
void 
Render_FillVertexArrayData(uint32 VertexArrayObject,
	uint32 BindingIndex, uint32 AttribIndex, uint32 Buffer,
	uint32 SpaceOffset, uint32 StartOffset);
void 
Render_FillVertexArrayIndices(RenderObj* RenderObject);
void 
Render_FillVetexArrayObject(RenderObj* RenderObject, 
	uint32 NumberAttribs, uint32* Offsets);

void 
Render_BindVertexArray(uint32 VertexArrayObject);

void
Render_BuildTexture(uint32* TextureID, uint32 Width, uint32 Height,
	uint8* TextureData);

void
Render_BindTexture(uint32 TextureID);
void
Render_Draw(RenderObj *RenderObject, uint32 VerticeCount);

#endif
#else

struct VBODescription
{
	VBODescription() : Data(0), Size(0), Offset(0) {}
	~VBODescription() {}

	void* Data;
	uint32 Size;
	uint32 Offset;
};

struct PipelineObjectDescription
{
	PipelineObjectDescription() : VertexArrayObjectID(0),
		NumberOfVertexHandles(0), VertexBufferObjectHandleIDs(0),
		VertexBufferDescriptions(0) {}

	~PipelineObjectDescription() {}

	uint32 VertexArrayObjectID;
	uint32 NumberOfVertexHandles;
	uint32* VertexBufferObjectHandleIDs; 
	VBODescription IndiceDescription;
	VBODescription* VertexBufferDescriptions; 
};

void
Render_ObjectPipelineInit(PipelineObjectDescription* ObjectDescription);
#endif

static window_properties WindowProperties;

// NOTE: General Functions
void 
Render_Init(window_properties *Window);
void 
Render_UpdateWindow(window_properties Window);
void 
Render_UpdateWindow(uint32 Width, uint32 Height);
void 
Render_ClearScreen(v4* Color);

// Should this be here?
window_properties
Render_GetWindowProperties();

// NOTE: Shader Functions
uint32 
Render_CompileShaders(const char* VertexShaderSource,
	const char* FragmentShaderSource);
void 
Render_BindShaders(uint32 ShaderProgramHandle);
void 
Render_ClearCurrentShaderProgram();
void 
Render_DeleteShaderProgram(uint32 ShaderProgramHandle);
int32
Render_GetShaderVariable(uint32 ShaderProgramHandle, char* name);

void 
Render_UpdateShaderVariable(int32 Location, int32 Integer);
void 
Render_UpdateShaderVariable(int32 Location, int32 IntegerX, 
	int32 IntegerY);
void 
Render_UpdateShaderVariable(int32 Location, int32 IntegerX,
	int32 IntegerY, int32 IntegerZ);
void
Render_UpdateShaderVariable(int32 Location, int32 IntegerX,
	int32 IntegerY, int32 IntegerZ, int32 IntegerW);
void 
Render_UpdateShaderVariable(int32 Location, int32 VectorSize,
	int32* IntegerArray, int32 ArraySize);
void 
Render_UpdateShaderVariable(int32 Location, 
	uint32 UnsignedInteger);
void 
Render_UpdateShaderVariable(int32 Location, 
	uint32 UnsignedIntegerX, 
	uint32 UnsignedIntegerY);
void
Render_UpdateShaderVariable(int32 Location,
	uint32 UnsignedIntegerX,
	uint32 UnsignedIntegerY, uint32 UnsignedIntegerZ);
void 
Render_UpdateShaderVariable(int32 Location,
	uint32 UnsignedIntegerX,
	uint32 UnsignedIntegerY, uint32 UnsignedIntegerZ, 
	uint32 UnsignedIntegerW);
void 
Render_UpdateShaderVariable(int32 Location, int32 VectorSize,
	uint32* UnsignedIntegerArray, int32 ArraySize);
void 
Render_UpdateShaderVariable(int32 Location, float Float);
void 
Render_UpdateShaderVariable(int32 Location, float FloatX, 
	float FloatY);
void
Render_UpdateShaderVariable(int32 Location, float FloatX,
	float FloatY, float FloatZ);
void 
Render_UpdateShaderVariable(int32 Location, float FloatX,
	float FloatY, float FloatZ, float FloatW);
void 
Render_UpdateShaderVariable(int32 Location, int32 VectorSize,
	float* FloatArray, int32 ArraySize);
void
Render_UpdateShaderVariable(int32 Location, int32 MatrixSize,
	float* NewData, int32 MatrixArraySize, bool Transpose);

// NOTE: Object Rendering Functions
void
Render_SetTexture(unsigned char* ImageData, int32 Width,
				int32 Height, uint32 *TextureID);
void
Render_DrawPoint(uint32 VertexArrayObjectID,
	float PointSize, uint32 numVertices);
void 
Render_DrawLine(uint32 VertexArrayObjectID,
	float LineSize, uint32 numVertices);
void 
Render_DrawObject(uint32 VertexArrayObjectID,
	uint32 &Texture, uint32 numVertices);
void
Render_DrawObjectIndices(uint32 VertexArrayObjectID,
	uint32 &Texture, uint32 NumberOfIndices);

void 
Render_UpdateColorVertice(uint32* VertexObjectHandleIDArray,
	uint32 ArrayPosition, float* Color);
void* 
Render_GetObjectShaderDataPtr(uint32* VertexObjectHandleIDArray,
	int32 Offset, uint32 Length);

// NOTE: Cleanup Functions
void 
Render_UnmapShaderDataPtr();
void 
Render_DeleteTexture(uint32 NumberOfTextures, uint32 *TextureID);
void 
Render_DeleteVertexArrays(uint32 NumberOfVertexArrayObjects,
	uint32 *VAO);
//TODO: Will this one ever be used?
void 
Render_DeleteBuffers(uint32 NumberOfBuffers, uint32 *Buffers); 
void
Render_DeleteBuffers(uint32 NumberOfBuffers, 
	uint32 VertexArrayObjectID, uint32 *Buffers);

#endif
