#include "render_object.h"

#if DATA_ORIENTED
PipelineObjectDescription ObjectDescription;

void RenderObj_Init(RenderObj* Object, float* Vertices,
	uint32 VerticeAmount, uint32* Indices, uint32 IndiceAmount,
	float* ColorData, float* TextureCoords, uint32 NumTextCoords,
	float* NormalVecs)
{
	Object->Buffer[0] = Vertices;
	Object->Size[0] = VerticeAmount;
	Object->IndicesPtr = Indices;
	Object->NumIndices = IndiceAmount;
	Object->Buffer[1] = ColorData;
	Object->Size[1] = VerticeAmount;
	Object->Buffer[2] = TextureCoords;
	Object->Size[2] = NumTextCoords;
	Object->Buffer[3] = NormalVecs;
	Object->Size[3] = VerticeAmount;
}

void RenderObj_Delete(RenderObj* Object)
{
	Render_DeleteBuffers(ObjectDescription.NumberOfVertexHandles,
		ObjectDescription.VertexArrayObjectID,
		ObjectDescription.VertexBufferObjectHandleIDs);
	Render_DeleteVertexArrays(1, &ObjectDescription.VertexArrayObjectID);
	Render_DeleteTexture(1, &Object->TextureID);

	if (ObjectDescription.VertexBufferDescriptions)
	{
		delete[] ObjectDescription.VertexBufferDescriptions;
		ObjectDescription.VertexBufferDescriptions = 0;
	}

	if (ObjectDescription.VertexBufferObjectHandleIDs)
	{
		delete[] ObjectDescription.VertexBufferObjectHandleIDs;
		ObjectDescription.VertexBufferObjectHandleIDs = 0;
	}

	if (Object->VerticeFloatArrayPtr)
	{
		delete[] Object->VerticeFloatArrayPtr;
		Object->VerticeFloatArrayPtr = 0;
	}
}
#endif
RenderObject::~RenderObject()
{
	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = NULL;
	}
}

void RenderObject::Init() {}

void RenderObject::Init(Texture2D* ImageData) {}

void RenderObject::Init(uint32 width, uint32 depth) {}

void RenderObject::Init(float width, float height, float depth) {}

void RenderObject::InputTexture(Texture2D* Texture)
{
	Render_SetTexture(Texture->Data, Texture->Width,
		Texture->Height, &TextureID);
}

void RenderObject::Draw()
{
	if (NumberOfIndices)
	{
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID,
			TextureID, NumberOfIndices);
	}
	else
	{
		Render_DrawObject(ObjectDescription.VertexArrayObjectID, 
			TextureID, NumberOfVertices);
	}
}

void RenderObject::Draw(uint8 Choice)
{
	if (Choice == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID,
			TextureID, NumberOfIndices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID,
			TextureID, NumberOfIndices);
	}
}

void RenderObject::Delete()
{
	Render_DeleteBuffers(ObjectDescription.NumberOfVertexHandles,
		ObjectDescription.VertexArrayObjectID,
		ObjectDescription.VertexBufferObjectHandleIDs);
	Render_DeleteVertexArrays(1, &ObjectDescription.VertexArrayObjectID);
	Render_DeleteTexture(1, &TextureID);

	//for (uint32 Index = 0; 
	//	Index < ObjectDescription.NumberOfVertexHandles; 
	//	Index++)
	//{
	//	//TODO: Are these actually needed?
	//	if (ObjectDescription.VertexBufferDescriptions[Index].FloatData)
	//	{
	//		delete[] ObjectDescription.VertexBufferDescriptions[Index].FloatData;
	//		ObjectDescription.VertexBufferDescriptions[Index].FloatData = NULL;
	//	}
	//	if (ObjectDescription.VertexBufferDescriptions[Index].Uint32Data)
	//	{
	//		delete[] ObjectDescription.VertexBufferDescriptions[Index].Uint32Data;
	//		ObjectDescription.VertexBufferDescriptions[Index].Uint32Data = NULL;
	//	}
	//}

	if (ObjectDescription.VertexBufferDescriptions)
	{
		delete[] ObjectDescription.VertexBufferDescriptions;
		ObjectDescription.VertexBufferDescriptions = 0;
	}

	if (ObjectDescription.VertexBufferObjectHandleIDs)
	{
		delete[] ObjectDescription.VertexBufferObjectHandleIDs;
		ObjectDescription.VertexBufferObjectHandleIDs = 0;
	}

	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = 0;
	}
}