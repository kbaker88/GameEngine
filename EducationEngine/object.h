#ifndef OBJECT_H
#define OBJECT_H

#include "debug_system.h"
#include "asset_system.h"

//TODO: Is ObjectID actually needed?

class Object
{
public:
	Object() : FrontVector(0.0f, 0.0f, 0.0f),
		UpVector(0.0f, 1.0f, 0.0f), VerticeFloatArrayPtr(NULL), Width(0.0f), 
		Height(0.0f), Depth(0.0f), Yaw(-90.0f), Pitch(0.0f), ObjectID(0), 
		TextureID(0) {}
	
	~Object();

	void Init();
	virtual void Init(TextureStorage* ImageData);
	virtual void Init(uint32 width, uint32 depth);
	virtual void Init(float width, float height, float depth);
	void InputTexture(TextureStorage* Texture);
	virtual void Draw();
	virtual void Draw(uint8 Choice);
	void Delete();

	//TODO: Do objects have up and front vectors?
	PipelineObjectDescription ObjectDescription;
	v3 FrontVector, UpVector;
	float* VerticeFloatArrayPtr;
	float Width, Height, Depth, Yaw, Pitch;
	uint32 ObjectID, TextureID, NumberOfVertices, NumberOfIndices;
};

#endif
