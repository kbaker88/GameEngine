#include "render_object.h"

#if DATA_ORIENTED
	// TODO: Delete this file.
#else
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

	ObjectDescription.IndiceDescription.Data = Indices;
	ObjectDescription.IndiceDescription.Size = sizeof(Indices);

	ObjectDescription.VertexBufferDescriptions[0].Data = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[1].Data = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[2].Data = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 2;

	ObjectDescription.VertexBufferDescriptions[3].Data = NormalData;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 3;

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

	ObjectDescription.IndiceDescription.Data = Indices;
	ObjectDescription.IndiceDescription.Size = sizeof(Indices);

	ObjectDescription.VertexBufferDescriptions[0].Data = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[1].Data = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[2].Data = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 2;
					  
	ObjectDescription.VertexBufferDescriptions[3].Data = NormalData;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 3;

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

	ObjectDescription.IndiceDescription.Data = Indices;
	ObjectDescription.IndiceDescription.Size = sizeof(Indices);

	ObjectDescription.VertexBufferDescriptions[0].Data = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;
							  
	ObjectDescription.VertexBufferDescriptions[1].Data = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[2].Data = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 2;
					  
	ObjectDescription.VertexBufferDescriptions[3].Data = NormalData;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 3;

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

	ObjectDescription.IndiceDescription.Data = Indices;
	ObjectDescription.IndiceDescription.Size = sizeof(Indices);

	ObjectDescription.VertexBufferDescriptions[0].Data = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[1].Data = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;
					  
	ObjectDescription.VertexBufferDescriptions[2].Data = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 2;
					  
	ObjectDescription.VertexBufferDescriptions[3].Data = NormalData;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}
#endif