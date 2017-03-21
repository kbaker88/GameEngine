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
		Texture->Height, TextureID);
}

void Object::Draw()
{
	//DrawObject(ObjectDescription, TextureID, NumberOfVertices);
	Render_DrawObjectIndices(ObjectDescription, TextureID, NumberOfIndices);
}

void Object::Draw(uint8 Choice)
{
	if (Choice == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Render_DrawObjectIndices(ObjectDescription, TextureID, NumberOfIndices);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		Render_DrawObjectIndices(ObjectDescription, TextureID, NumberOfIndices);
	}
}

void Object::Delete()
{
	Render_DeleteVertexArrays(1, &ObjectDescription.VertexArrayObject);
	Render_DeleteBuffers(4, ObjectDescription.VertexBufferObjectHandles);
	Render_DeleteTextureBuffer(1, TextureID);
}