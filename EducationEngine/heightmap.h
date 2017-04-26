#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "render_object.h"

class HeightMap: public RenderObject
{
public:
	HeightMap() {};
	~HeightMap();

	void Init(Texture2D* ImageData);
};


#endif