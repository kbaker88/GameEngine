#include "rectangle.h"

MyRectangle::MyRectangle() 
{
	NumberOfVertices = 4;
}

MyRectangle::~MyRectangle()
{
}

void MyRectangle::Init()
{
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

	ObjectDescription.NumberOfVertexHandles = 5;

	ObjectDescription.Description[1].Data = VerticePositions;
	ObjectDescription.Description[1].Size = sizeof(VerticePositions);
	ObjectDescription.Description[1].Offset = 3;

	ObjectDescription.Description[0].IndiceData = Indices;
	ObjectDescription.Description[0].Size = sizeof(Indices);

	ObjectDescription.Description[2].Data = ColorData;
	ObjectDescription.Description[2].Size = sizeof(ColorData);
	ObjectDescription.Description[2].Offset = 3;

	ObjectDescription.Description[3].Data = TextureCoords;
	ObjectDescription.Description[3].Size = sizeof(TextureCoords);
	ObjectDescription.Description[3].Offset = 2;

	ObjectDescription.Description[4].Data = NormalData;
	ObjectDescription.Description[4].Size = sizeof(NormalData);
	ObjectDescription.Description[4].Offset = 3;

	Render_ObjectPipelineInit(ObjectDescription);
}

void MyRectangle::Init(float size)
{
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

	ObjectDescription.NumberOfVertexHandles = 5;

	ObjectDescription.Description[1].Data = VerticePositions;
	ObjectDescription.Description[1].Size = sizeof(VerticePositions);
	ObjectDescription.Description[1].Offset = 3;

	ObjectDescription.Description[0].IndiceData = Indices;
	ObjectDescription.Description[0].Size = sizeof(Indices);

	ObjectDescription.Description[2].Data = ColorData;
	ObjectDescription.Description[2].Size = sizeof(ColorData);
	ObjectDescription.Description[2].Offset = 3;

	ObjectDescription.Description[3].Data = TextureCoords;
	ObjectDescription.Description[3].Size = sizeof(TextureCoords);
	ObjectDescription.Description[3].Offset = 2;

	ObjectDescription.Description[4].Data = NormalData;
	ObjectDescription.Description[4].Size = sizeof(NormalData);
	ObjectDescription.Description[4].Offset = 3;

	Render_ObjectPipelineInit(ObjectDescription);
}

void MyRectangle::Init(float width, float height)
{
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

	ObjectDescription.NumberOfVertexHandles = 5;

	ObjectDescription.Description[1].Data = VerticePositions;
	ObjectDescription.Description[1].Size = sizeof(VerticePositions);
	ObjectDescription.Description[1].Offset = 3;

	ObjectDescription.Description[0].IndiceData = Indices;
	ObjectDescription.Description[0].Size = sizeof(Indices);

	ObjectDescription.Description[2].Data = ColorData;
	ObjectDescription.Description[2].Size = sizeof(ColorData);
	ObjectDescription.Description[2].Offset = 3;

	ObjectDescription.Description[3].Data = TextureCoords;
	ObjectDescription.Description[3].Size = sizeof(TextureCoords);
	ObjectDescription.Description[3].Offset = 2;

	ObjectDescription.Description[4].Data = NormalData;
	ObjectDescription.Description[4].Size = sizeof(NormalData);
	ObjectDescription.Description[4].Offset = 3;

	Render_ObjectPipelineInit(ObjectDescription);
}

void MyRectangle::Init(float width, float height, float depth)
{
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

	ObjectDescription.NumberOfVertexHandles = 5;

	ObjectDescription.Description[1].Data = VerticePositions;
	ObjectDescription.Description[1].Size = sizeof(VerticePositions);
	ObjectDescription.Description[1].Offset = 3;

	ObjectDescription.Description[0].IndiceData = Indices;
	ObjectDescription.Description[0].Size = sizeof(Indices);

	ObjectDescription.Description[2].Data = ColorData;
	ObjectDescription.Description[2].Size = sizeof(ColorData);
	ObjectDescription.Description[2].Offset = 3;

	ObjectDescription.Description[3].Data = TextureCoords;
	ObjectDescription.Description[3].Size = sizeof(TextureCoords);
	ObjectDescription.Description[3].Offset = 2;

	ObjectDescription.Description[4].Data = NormalData;
	ObjectDescription.Description[4].Size = sizeof(NormalData);
	ObjectDescription.Description[4].Offset = 3;

	Render_ObjectPipelineInit(ObjectDescription);
}