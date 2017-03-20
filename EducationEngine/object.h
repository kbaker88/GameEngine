#ifndef OBJECT_H
#define OBJECT_H

#include "debug_system.h"
#include "asset_system.h"

class Object
{
public:
	Object();
	~Object();

	void Init();
	virtual void Init(uint32 width, uint32 depth);
	virtual void Init(float width, float height, float depth);
	void InputTexture(TextureStorage* Texture);
	virtual void Draw();
	void Delete();

	PipelineObjectDescription ObjectDescription;
	uint32 ID;
	float* VerticeFloatArrayPtr;
	uint32 NumberOfVertices;
	uint32 NumberOfIndices;

	float Width, Height, Depth;
	
	uint32 TextureID;
};

#endif
