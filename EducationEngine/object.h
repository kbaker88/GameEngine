#ifndef OBJECT_H
#define OBJECT_H

#include "debug_system.h"
#include "asset_system.h"

//TODO: Is ObjectID actually needed?

class Object
{
public:
	Object();
	~Object();

	void Init();
	virtual void Init(TextureStorage* ImageData);
	virtual void Init(uint32 width, uint32 depth);
	virtual void Init(float width, float height, float depth);
	void InputTexture(TextureStorage* Texture);
	virtual void Draw();
	virtual void Draw(uint8 Choice);
	void Delete();

	PipelineObjectDescription ObjectDescription;
	float* VerticeFloatArrayPtr;
	float Width, Height, Depth;
	uint32 ObjectID, TextureID, NumberOfVertices, NumberOfIndices;
};

#endif
