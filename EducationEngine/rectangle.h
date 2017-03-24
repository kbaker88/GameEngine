#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "object.h"

class MyRectangle : public Object
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