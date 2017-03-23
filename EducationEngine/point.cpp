#include "point.h"

Point::Point() : PointSize(1.0f) {}

Point::~Point() {}

void Point::Init(float* PointPosition, float Size)
{
	ObjectDescription.NumberOfVertexHandles = 2;
	ObjectDescription.VertexBufferDescriptions =
		new VBODescription[ObjectDescription.NumberOfVertexHandles];
	ObjectDescription.VertexBufferObjectHandleIDs =
		new uint32[ObjectDescription.NumberOfVertexHandles];

	PointSize = Size;

	float VerticePositions[] =
	{
		PointPosition[0], PointPosition[1], PointPosition[2]
	};

	float ColorData[] =
	{
		1.0f, 1.0f, 1.0f
	};

	ObjectDescription.VertexBufferDescriptions[0].FloatData = VerticePositions;
	ObjectDescription.VertexBufferDescriptions[0].Size = sizeof(VerticePositions);
	ObjectDescription.VertexBufferDescriptions[0].Offset = 3;

	ObjectDescription.VertexBufferDescriptions[1].FloatData = ColorData;
	ObjectDescription.VertexBufferDescriptions[1].Size = sizeof(ColorData);
	ObjectDescription.VertexBufferDescriptions[1].Offset = 3;

	Render_ObjectPipelineInit(&ObjectDescription);
}

void Point::Draw()
{
	Render_DrawPoint(ObjectDescription.VertexArrayObjectID, PointSize, 1);
}