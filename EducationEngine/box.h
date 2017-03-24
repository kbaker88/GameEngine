#ifndef BOX_H
#define BOX_H

#include "object.h"

class Box : public Object
{
public:
	Box() {};
	~Box() {};

	void Init(float NewWidth, float NewHeight, float NewDepth);

private:
	
};

#endif
