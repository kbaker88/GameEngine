#include "rectangle.h"

void MyRectangle::Init()
{
	ObjectDescription.NumberOfVertexHandles = 5;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	NumberOfVertices = 4;

	Width = 1;
	Height = 1;

	float VerticePositions[] = {
		-0.5f, -0.5f, 0.0f,   // bottom left 0
		 0.5f, -0.5f, 0.0f,   // bottom right 1
		 0.5f, 0.5f,  0.0f,   // top right 2
		-0.5f, 0.5f,  0.0f,   // top left 3
	};

	uint32 Indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	NumberOfIndices = 6;

	float ColorData[] {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float TextureCoords[] {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	float NormalData[]{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	ObjectDescription.VertexBufferDescriptions[1].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[0].Uint32Data = Indices;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(Indices);

	ObjectDescription.VertexBufferDescriptions[2].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[3].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 2;

	ObjectDescription.VertexBufferDescriptions[4].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[4].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[4].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}

void MyRectangle::Init(float size)
{
	ObjectDescription.NumberOfVertexHandles = 5;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	Width = size;
	Height = size;

	float VerticePositions[] = {
		-size/2, -size/2, 0.0f,  // bottom left 0
		 size/2, -size/2, 0.0f,  // bottom right 1
		 size/2,  size/2, 0.0f,  // top right 2
		-size/2,  size/2, 0.0f,  // top left 3
	};

	uint32 Indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	NumberOfIndices = 6;

	float ColorData[]{
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float TextureCoords[]{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	float NormalData[]{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	ObjectDescription.VertexBufferDescriptions[1].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[0].Uint32Data = Indices;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(Indices);
					  
	ObjectDescription.VertexBufferDescriptions[2].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[3].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 2;
					  
	ObjectDescription.VertexBufferDescriptions[4].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[4].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[4].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}

void MyRectangle::Init(float width, float height)
{
	ObjectDescription.NumberOfVertexHandles = 5;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	Width = width;
	Height = height;

	float VerticePositions[] = {
		-(1.0f * Width / 2), -(1.0f * Height / 2),  0.0f,  // bottom left 0
		 (1.0f * Width / 2), -(1.0f * Height / 2),  0.0f,  // bottom right 1
		 (1.0f * Width / 2),  (1.0f * Height / 2),  0.0f,  // top right 2
		-(1.0f * Width / 2),  (1.0f * Height / 2),  0.0f,  // top left 3
	};

	uint32 Indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	NumberOfIndices = 6;

	float ColorData[]{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float TextureCoords[]{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	float NormalData[]{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	ObjectDescription.VertexBufferDescriptions[1].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[0].Uint32Data = Indices;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(Indices);
					  
	ObjectDescription.VertexBufferDescriptions[2].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[3].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 2;
					  
	ObjectDescription.VertexBufferDescriptions[4].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[4].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[4].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}

void MyRectangle::Init(float width, float height, float depth)
{
	ObjectDescription.NumberOfVertexHandles = 5;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	Width = width;
	Height = height;
	Depth = depth;

	float VerticePositions[] = {
		-(1.0f * Width / 2), -(1.0f * Height / 2),  (1.0f * depth / 2),  // bottom left 0
		(1.0f * Width / 2), -(1.0f * Height / 2),   (1.0f * depth / 2),  // bottom right 1
		(1.0f * Width / 2),  (1.0f * Height / 2),   -(1.0f * depth / 2), // top right 2
		-(1.0f * Width / 2),  (1.0f * Height / 2),  -(1.0f * depth / 2), // top left 3
	};

	uint32 Indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	NumberOfIndices = 6;

	float ColorData[]{
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	float TextureCoords[]{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	float NormalData[]{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	ObjectDescription.VertexBufferDescriptions[1].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[0].Uint32Data = Indices;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(Indices);
					  
	ObjectDescription.VertexBufferDescriptions[2].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[3].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 2;
					  
	ObjectDescription.VertexBufferDescriptions[4].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[4].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[4].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}