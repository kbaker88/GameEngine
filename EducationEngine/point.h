#ifndef POINT_H
#define POINT_H

#include "render_object.h"

class Point : public RenderObject
{
public:
	Point() : PointSize(1.0f) {};
	~Point() {};

	void Init(float* PointPositon, float Size);
	void Draw();
private:
	PipelineObjectDescription ObjectDescription;
	float PointSize;

};

#endif