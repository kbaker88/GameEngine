#include "render_object.h"

void Line::Init(float* Point1, float* Point2, float lineSize)
{
	ObjectDescription.NumberOfVertexHandles = 2;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	LineSize = lineSize;

	float VerticePositions[] =
	{
		Point1[0], Point1[1], Point1[2],
		Point2[0], Point2[1], Point2[2]
	};

	float ColorData[] =
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	ObjectDescription.VertexBufferDescriptions[0].Data = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[1].Data = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}

void Line::Draw()
{
	Render_DrawLine(ObjectDescription.VertexArrayObjectID, LineSize, 2);
}