#include "line.h"

Line::Line() {}

Line::~Line()
{
	//Render_DeleteVertexArrays(1, &ObjectDescription.VertexArrayObjectID);
	//Render_DeleteBuffers(2, ObjectDescription.VertexBufferObjectHandleIDs);
}

void Line::Init(float* Point1, float* Point2, float lineSize)
{
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

	ObjectDescription.NumberOfVertexHandles = 2;

	ObjectDescription.Description[0].Data = VerticePositions;
	ObjectDescription.Description[0].Size = sizeof(VerticePositions);
	ObjectDescription.Description[0].Offset = 3;

	ObjectDescription.Description[1].Data = ColorData;
	ObjectDescription.Description[1].Size = sizeof(ColorData);
	ObjectDescription.Description[1].Offset = 3;

	Render_ObjectPipelineInit(ObjectDescription);
}

void Line::Draw()
{
	Render_DrawLine(ObjectDescription, LineSize, 2);
}