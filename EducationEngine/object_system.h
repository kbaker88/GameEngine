#ifndef OBJECTS_H
#define OBJECTS_H

#include "object.h"

#include "box.h"
#include "rectangle.h"
#include "sphere.h"
#include "line.h"
#include "point.h"
#include "plane2d.h"
#include "heightmap.h"

struct ObjectInstance
{
	ObjectInstance() : ObjectPtr(NULL), Position(v3(0.0f, 0.0f, 0.0f)), ID(0)
	{};

	Object* ObjectPtr;
	m4 ModelMatrix;
	v3 Position;
	uint32 ID;

	ObjectInstance & operator=(ObjectInstance & NewInstance)
	{
		ObjectPtr = NewInstance.ObjectPtr;
		ModelMatrix = NewInstance.ModelMatrix;
		Position = NewInstance.Position;
		ID = NewInstance.ID;
	}
};

void Object_ResetObjectCounter();

uint32 Object_Load(Object* NewObject);
uint32 Object_Load(Object* NewObject, uint32 Width, uint32 Depth);
uint32 Object_Load(Object* NewObject, float Width, float Height, float Depth);
void Object_SetTexture(uint32 ObjectID, TextureStorage* Texture);

Object* Object_Find(uint32 ObjectID);
void Object_Delete(uint32 ObjectID);

void Object_ClearAll();

struct ObjectListNode
{
	ObjectListNode *NextObject;
	Object *ObjectPtr;
};

class ObjectList
{
public:
	ObjectList();
	~ObjectList();

	void Add(Object *ObjectPtr);
	void Delete(uint32 ID);
	Object* Find(uint32 ID); // rename
	void Clear();

	ObjectListNode *FirstObject;
};

#endif