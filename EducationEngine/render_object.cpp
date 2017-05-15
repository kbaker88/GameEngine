#include "render_object.h"

#if DATA_ORIENTED

void 
RenderObj_Init(RenderObj* RenderObject, Model* ModelObj)
{
	//TODO: Think about creating many VAO's at once.
	Render_CreateVertexArrays(1, &RenderObject->VertexArrayID);
	Render_CreateBuffers(ModelObj->NumAttribs, 
		RenderObject->BufferID);
	for (uint32 Index = 0; Index < ModelObj->NumAttribs; Index++)
	{
		Render_FillBuffer(RenderObject->BufferID[Index],
			ModelObj->ArraySize[Index], ModelObj->Data[Index], 0);
	}
	Render_FillVetexArrayObject(RenderObject, ModelObj->NumAttribs);
}

void 
RenderObj_Delete(RenderObj* Object)
{

}

#else
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
#endif