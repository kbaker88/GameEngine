#include "object.h"

Object::Object() : VerticeFloatArrayPtr(NULL), ObjectID(0), 
TextureID(0), NumberOfVertices(0), NumberOfIndices(0), Width(0.0f), 
Height(0.0f), Depth(0.0f) {}

Object::~Object()
{
	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = NULL;
	}
}

void Object::Init()
{
}

void Object::Init(TextureStorage* ImageData)
{

}

void Object::Init(uint32 width, uint32 depth)
{
	Width = 0;
	Depth = 0;
}


void Object::Init(float width, float height, float depth)
{
	Width = 0;
	Height = 0;
	Depth = 0;
}

void Object::InputTexture(TextureStorage* Texture)
{
	Render_SetTexture(Texture->data, Texture->Width,
		Texture->Height, &TextureID);
}

void Object::Draw()
{
	if (NumberOfIndices)
	{
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID, TextureID, NumberOfIndices);
	}
	else
	{
		Render_DrawObject(ObjectDescription.VertexArrayObjectID, TextureID, NumberOfVertices);
	}
}

void Object::Draw(uint8 Choice)
{
	if (Choice == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID, TextureID, NumberOfIndices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		Render_DrawObjectIndices(ObjectDescription.VertexArrayObjectID, TextureID, NumberOfIndices);
	}
}

void Object::Delete()
{
	//TODO: Change the 1 for deletion of texture and vertex arrays so multiple is possible
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
		ObjectDescription.VertexBufferDescriptions = NULL;
	}

	if (ObjectDescription.VertexBufferObjectHandleIDs)
	{
		delete[] ObjectDescription.VertexBufferObjectHandleIDs;
		ObjectDescription.VertexBufferObjectHandleIDs = NULL;
	}

	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = NULL;
	}
}