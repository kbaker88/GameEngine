#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "object.h"

class HeightMap: public Object
{
public:
	HeightMap();
	~HeightMap();

	void Init(TextureStorage* ImageData);
	void InputTexture(TextureStorage* Texture);
	void UpdateColorVertice(PipelineObjectDescription& Description, uint32 ArrayPosition, float* Color);
	void DrawIndices();

	uint32 TerrainMaxDepth, TerrainMaxWidth;
private:
	uint32 NumberOfIndices;
};


#endif