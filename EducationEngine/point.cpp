#include "point.h"

Point::Point() : PointSize(1.0f)
{

}

Point::~Point()
{
	//DeleteVertexArrays(1, &ObjectDescription.VertexArrayObject);
	//DeleteBuffers(2, ObjectDescription.VertexBufferObjectHandles);
}

void Point::Init(float* PointPosition, float Size)
{
	PointSize = Size;

	float VerticePositions[] =
	{
		PointPosition[0], PointPosition[1], PointPosition[2]
	};

	float ColorData[] =
	{
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

void Point::Draw()
{
	Render_DrawPoint(ObjectDescription, PointSize, 1);
}