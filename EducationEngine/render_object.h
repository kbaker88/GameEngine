#ifndef OBJECT_H
#define OBJECT_H

#include "debug_system.h"
#include "asset_system.h"

class RenderObject
{
public:
	RenderObject() :  VerticeFloatArrayPtr(0), Width(0.0f), Height(0.0f), Depth(0.0f),
	ObjectID(0), TextureID(0) {}
	
	~RenderObject();

	void Init();
	virtual void Init(Texture2D* ImageData);
	virtual void Init(uint32 width, uint32 depth);
	virtual void Init(float width, float height, float depth);
	void InputTexture(Texture2D* Texture);
	virtual void Draw();
	virtual void Draw(uint8 Choice);
	void Delete();

	//TODO: Do objects have up and front vectors?
	PipelineObjectDescription ObjectDescription;
	float* VerticeFloatArrayPtr;
	float Width, Height, Depth;
	uint32 ObjectID, TextureID, NumberOfVertices, NumberOfIndices;
};

#endif
