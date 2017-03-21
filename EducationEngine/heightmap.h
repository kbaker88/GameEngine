#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "object.h"

class HeightMap: public Object
{
public:
	HeightMap();
	~HeightMap();

	void Init(TextureStorage* ImageData);

};


#endif