#ifndef LINE_H
#define LINE_H

#include "render_object.h"

class Line : public RenderObject
{
public:
	Line() {};
	~Line() {};

	void Init(float* Point1, float* Point2, float lineSize);
	void Draw();

private:
	PipelineObjectDescription ObjectDescription = {};
	float LineSize;
};

#endif
