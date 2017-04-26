#ifndef PLANE2D_H
#define PLANE2D_H

#include "render_object.h"

class Plane2D : public RenderObject
{
public:
	Plane2D() {}

	~Plane2D();

	void Init(uint32 width, uint32 height);
	void Draw();
	
private:
	uint32 NumberOfSquares;
	
};


#endif