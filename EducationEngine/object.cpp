#include "object.h"

Object::Object() : TextureID(0)
{
	VerticeFloatArrayPtr = 0;
}

Object::~Object()
{
	if (VerticeFloatArrayPtr)
	{
		delete[] VerticeFloatArrayPtr;
		VerticeFloatArrayPtr = 0;
	}
}

void Object::Init()
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
	//CurrentTexturePtr = Texture;

	Render_SetTexture(Texture->data, Texture->Width,
		Texture->Height, TextureID);
}

void Object::Draw()
{
	//DrawObject(ObjectDescription, TextureID, NumberOfVertices);
	Render_DrawObjectIndices(ObjectDescription, TextureID, NumberOfIndices);
}

void Object::Delete()
{
	Render_DeleteVertexArrays(1, &ObjectDescription.VertexArrayObject);
	Render_DeleteBuffers(4, ObjectDescription.VertexBufferObjectHandles);
	Render_DeleteTextureBuffer(1, TextureID);
}