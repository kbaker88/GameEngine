#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "render_object.h"

class MyRectangle : public RenderObject
{
public:
	MyRectangle() {};
	~MyRectangle() {};

	void Init();
	void Init(float size);
	void Init(float width, float height);
	void Init(float width, float height, float depth);

private:

};

#endif