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

class Box : public RenderObject
{
public:
	Box() {};
	~Box() {};

	void Init(float NewWidth, float NewHeight, float NewDepth);

private:

};

class HeightMap : public RenderObject
{
public:
	HeightMap() {};
	~HeightMap();

	void Init(Texture2D* ImageData);
};


class Line : public RenderObject
{
public:
	Line() {};
	~Line() {};

	void Init(float* Point1, float* Point2, float lineSize);
	void Draw();

private:
	PipelineObjectDescription ObjectDescription = {};
	float LineSize;
};

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

class Point : public RenderObject
{
public:
	Point() : PointSize(1.0f) {};
	~Point() {};

	void Init(float* PointPositon, float Size);
	void Draw();
private:
	PipelineObjectDescription ObjectDescription;
	float PointSize;

};

class MyRectangle : public RenderObject
{
public:
	MyRectangle() {};
	~MyRectangle() {};

	void Init();
	void Init(float size);
	void Init(float width, float height);
	void Init(float width, float height, float depth);

private:

};

class Sphere : public RenderObject
{
public:
	Sphere();
	~Sphere();

	void Init();

private:
};

#endif
