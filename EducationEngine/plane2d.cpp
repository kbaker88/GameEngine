#include "plane2d.h"

Plane2D::Plane2D() {}

Plane2D::~Plane2D() {}

void Plane2D::Init(uint32 width, uint32 height)
{
	ObjectDescription.NumberOfVertexHandles = 4;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	uint32 index = 0;
	NumberOfSquares = width * height;
	const uint32 floatsPerSquare = 18;
	bool flip = false;

	float *VerticePositions = new float[floatsPerSquare * NumberOfSquares]{};

	for (uint32 i = 0; i < width; i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0; j < height; j++)
		{
			if (!flip)
			{
				VerticePositions[index + 0] = (float)i;
				VerticePositions[index + 1] = 0;
				VerticePositions[index + 2] = -(float)j  - 1.0f;

				VerticePositions[index + 3] = (float)i    + 1.0f;
				VerticePositions[index + 4] = 0;
				VerticePositions[index + 5] = -(float)j  - 1.0f;

				VerticePositions[index + 6] = (float)i   + 1.0f;
				VerticePositions[index + 7] = 0;
				VerticePositions[index + 8] = -(float)j;

				VerticePositions[index + 9] = (float)i   + 1.0f;
				VerticePositions[index + 10] = 0;
				VerticePositions[index + 11] = -(float)j;

				VerticePositions[index + 12] = (float)i;
				VerticePositions[index + 13] = 0;
				VerticePositions[index + 14] = -(float)j;
				
				VerticePositions[index + 15] = (float)i;
				VerticePositions[index + 16] = 0;
				VerticePositions[index + 17] = -(float)j - 1.0f;

				index = index + 18;

				flip = true;
			}
			else
			{
				//new
				VerticePositions[index + 0] = (float)i; // f left
				VerticePositions[index + 1] = 0;
				VerticePositions[index + 2] = -(float)j;

				VerticePositions[index + 3] = (float)i; // b left
				VerticePositions[index + 4] = 0;
				VerticePositions[index + 5] = -(float)j - 1.0f;

				VerticePositions[index + 6] = (float)i  + 1.0f; // b right
				VerticePositions[index + 7] = 0;
				VerticePositions[index + 8] = -(float)j - 1.0f;

				VerticePositions[index + 9] = (float)i + 1.0f; // b right
				VerticePositions[index + 10] = 0;
				VerticePositions[index + 11] = -(float)j - 1.0f;

				VerticePositions[index + 12] = (float)i + 1.0f; // f right
				VerticePositions[index + 13] = 0;
				VerticePositions[index + 14] = -(float)j;

				VerticePositions[index + 15] = (float)i; // f left
				VerticePositions[index + 16] = 0;
				VerticePositions[index + 17] = -(float)j;


				index = index + 18;
				flip = false;
			}
		}
	}

	VerticeFloatArrayPtr = VerticePositions;
	index = 0;


	float *ColorData = new float[floatsPerSquare * NumberOfSquares]{};

	for (uint32 i = 0; i < width; i++)
	{
		for (uint32 j = 0; j < height; j++)
		{
			ColorData[index + 0] =  1;
			ColorData[index + 1] =  1;
			ColorData[index + 2] =  1;
			
			ColorData[index + 3] =  1;
			ColorData[index + 4] =  1;
			ColorData[index + 5] =  1;
			
			ColorData[index + 6] =  1;
			ColorData[index + 7] =  1;
			ColorData[index + 8] =  1;
			
			ColorData[index + 9] =  1;
			ColorData[index + 10] = 1;
			ColorData[index + 11] = 1;
			
			ColorData[index + 12] = 1;
			ColorData[index + 13] = 1;
			ColorData[index + 14] = 1;
			
			ColorData[index + 15] = 1;
			ColorData[index + 16] = 1;
			ColorData[index + 17] = 1;

			index = index + 18;
		}
	}

	index = 0;
	flip = false;

	float *TextureCoords = new float[12 * NumberOfSquares]{};

	for (uint32 i = 0; i < width; i++)
	{
		if (i % 2 == 0)
			flip = false;
		else
			flip = true;

		for (uint32 j = 0; j < height; j++)
		{
			if (!flip)
			{
				TextureCoords[index + 0] = 0.0f;
				TextureCoords[index + 1] = 1.0f;
				
				TextureCoords[index + 2] = 1.0f;
				TextureCoords[index + 3] = 1.0f;
				
				TextureCoords[index + 4] = 1.0f;
				TextureCoords[index + 5] = 0.0f;
				
				TextureCoords[index + 6] = 1.0f;
				TextureCoords[index + 7] = 0.0f;
				
				TextureCoords[index + 8] = 0.0f;
				TextureCoords[index + 9] = 0.0f;
				
				TextureCoords[index + 10] = 0.0f;
				TextureCoords[index + 11] = 1.0f;

				index = index + 12;
				flip = true;
			}
			else
			{
				TextureCoords[index + 0] = 0.0f;
				TextureCoords[index + 1] = 0.0f;
				
				TextureCoords[index + 2] = 0.0f;
				TextureCoords[index + 3] = 1.0f;
				
				TextureCoords[index + 4] = 1.0f;
				TextureCoords[index + 5] = 1.0f;
				
				TextureCoords[index + 6] = 1.0f;
				TextureCoords[index + 7] = 1.0f;
				
				TextureCoords[index + 8] = 1.0f;
				TextureCoords[index + 9] = 0.0f;
				
				TextureCoords[index + 10] = 0.0f;
				TextureCoords[index + 11] = 0.0f;

				index = index + 12;
				flip = false;
			}
		}
	}

	index = 0; 

	float *NormalData = new float[floatsPerSquare * NumberOfSquares]{};

	for (uint32 i = 0; i < width; i++)
	{
		for (uint32 j = 0; j < height; j++)
		{
			NormalData[index + 0] = 0;
			NormalData[index + 1] = 1;
			NormalData[index + 2] = 0;
			
			NormalData[index + 3] = 0;
			NormalData[index + 4] = 1;
			NormalData[index + 5] = 0;
			
			NormalData[index + 6] = 0;
			NormalData[index + 7] = 1;
			NormalData[index + 8] = 0;
			
			NormalData[index + 9] = 0;
			NormalData[index + 10] = 1;
			NormalData[index + 11] = 0;
			
			NormalData[index + 12] = 0;
			NormalData[index + 13] = 1;
			NormalData[index + 14] = 0;
			
			NormalData[index + 15] = 0;
			NormalData[index + 16] = 1;
			NormalData[index + 17] = 0;

			index = index + 18;
		}
	} 

	ObjectDescription.VertexBufferDescriptions[0].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = 
		floatsPerSquare * NumberOfSquares * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[1].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = 
		floatsPerSquare * NumberOfSquares * sizeof(float);;
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[2].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[2].Size = 
		floatsPerSquare * NumberOfSquares * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[3].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[3].Size = 
		12 * NumberOfSquares * sizeof(float);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 2;

	Render_ObjectPipelineInit(&ObjectDescription);

	delete[] ColorData;
	delete[] NormalData; 
	delete[] TextureCoords;

	NumberOfVertices = 6 * NumberOfSquares;
}

void Plane2D::Draw()
{
	Render_DrawObject(ObjectDescription.VertexArrayObjectID, TextureID, NumberOfVertices);
}