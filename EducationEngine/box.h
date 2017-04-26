#ifndef BOX_H
#define BOX_H

#include "render_object.h"

class Box : public RenderObject
{
public:
	Box() {};
	~Box() {};

	void Init(float NewWidth, float NewHeight, float NewDepth);

private:
	
};

#endif
