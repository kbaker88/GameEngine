
#include "box.h"

Box::Box() {}

Box::~Box() {}

void Box::Init(float NewWidth, float NewHeight, float NewDepth)
{
	ObjectDescription.NumberOfVertexHandles = 4;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	Width = NewWidth;
	Height = NewHeight;
	Depth = NewDepth;

	NumberOfVertices = 36;

	float VerticePositions[] =
	{	 // Back
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, // left, bottom, back
		 Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, // right, bottom, back
		 Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, // right, top, back
		 Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, // right, top, back
		-Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, // left, top, back
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, // left, bottom, back
		// Front									  
		-Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, // Left, Bottom, Front
	 	 Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, // Right, Bottom, Front
	 	 Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, // Right, Top, Front
	 	 Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, // Right, Top, Front
		-Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, // Left, Top, Front
		-Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, // Left, Bottom, Front
		// Left
		-Width * 0.5f,  Height * 0.5f,  Depth * 0.5f,
		-Width * 0.5f,  Height * 0.5f, -Depth * 0.5f,
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f,
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f,
		-Width * 0.5f, -Height * 0.5f,  Depth * 0.5f,
		-Width * 0.5f,  Height * 0.5f,  Depth * 0.5f,
		// Right
		Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, 
		Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, 
		Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, 
		Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, 
		Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, 
		Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, 
		// Bottom
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f,
		Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, 
		Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, 
		Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, 
		-Width * 0.5f, -Height * 0.5f,  Depth * 0.5f,
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f,
		// Top
		-Width * 0.5f,  Height * 0.5f, -Depth * 0.5f,
		Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, 
		Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, 
		Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, 
		-Width * 0.5f,  Height * 0.5f,  Depth * 0.5f,
		-Width * 0.5f,  Height * 0.5f, -Depth * 0.5f,
	};

	VerticeFloatArrayPtr = new float[sizeof(VerticePositions)];

	for (int i = 0; i < sizeof(VerticePositions); i++)
	{
		VerticeFloatArrayPtr[i] = VerticePositions[i];
	}

	NumberOfIndices = 0;

	float ColorData[]
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	float TextureCoords[]
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	float NormalData[]
	{
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	ObjectDescription.VertexBufferDescriptions[0].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[1].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[2].FloatData = NormalData;
	ObjectDescription.VertexBufferDescriptions[2].Size = sizeof(NormalData);
	ObjectDescription.VertexBufferDescriptions[2].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[3].FloatData = TextureCoords;
	ObjectDescription.VertexBufferDescriptions[3].Size = sizeof(TextureCoords);
	ObjectDescription.VertexBufferDescriptions[3].Offset = 2;


	Render_ObjectPipelineInit(&ObjectDescription);

	// Below is indice implementation of a box
/*	ObjectDescription.NumberOfVertexHandles = 5;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	Width = NewWidth;
	Height = NewHeight;
	Depth = NewDepth;

	NumberOfVertices = 36;

	float VerticePositions[] =
	{
		-Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, //0 left, bottom, front
	  	 Width * 0.5f, -Height * 0.5f,  Depth * 0.5f, //1 right, bottom, front
		 Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, //2 right, top, front
		-Width * 0.5f,  Height * 0.5f,  Depth * 0.5f, //3 left, top, front
		-Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, //4 left, bottom, back
		 Width * 0.5f, -Height * 0.5f, -Depth * 0.5f, //5 right, bottom, back
		 Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, //6 right, top, back
		-Width * 0.5f,  Height * 0.5f, -Depth * 0.5f, //7 left, top, back
	};

	VerticeFloatArrayPtr = new float[sizeof(VerticePositions)];

	for (int i = 0; i < sizeof(VerticePositions); i++)
	{
		VerticeFloatArrayPtr[i] = VerticePositions[i];
	}

	uint32 Indices[] =
	{
		0, 1, 2, // front face
		2, 3, 0,

		6, 3, 2, // top face
		7, 3, 6,

		5, 4, 7, // back face
		7, 6, 5,

		5, 4, 0, // bottom face
		0, 1, 5,

		4, 0, 3, // left face
		3, 7, 4,

		1, 5, 6, // right face
		6, 2, 1
	};

	NumberOfIndices = 36;


	float ColorData[]
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	float TextureCoords[]
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	float NormalData[]
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		- 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
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

	Render_ObjectPipelineInit(&ObjectDescription);*/
}