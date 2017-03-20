#ifndef POINT_H
#define POINT_H

#include "object.h"

class Point : public Object
{
public:
	Point();
	~Point();

	void Init(float* PointPositon, float Size);
	void Draw();
private:
	PipelineObjectDescription ObjectDescription;
	float PointSize;

};

#endif