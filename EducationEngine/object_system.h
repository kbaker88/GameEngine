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
	ObjectInstance() : ObjectPtr(NULL), Position(v3(0.0f, 0.0f, 0.0f))
	{};

	Object* ObjectPtr;
	m4 ModelMatrix;
	v3 Position;

	ObjectInstance & operator=(ObjectInstance & NewInstance)
	{
		ObjectPtr = NewInstance.ObjectPtr;
		ModelMatrix = NewInstance.ModelMatrix;
		Position = NewInstance.Position;
	}
};

void Object_CreateBlock(uint32 BlockID, uint32 Size);
void Object_DeleteBlock(uint32 BlockID);
void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID);
void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID, TextureStorage* HeightMapTexture);
void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID, uint32 Width, uint32 Depth);
void Object_Create(Object* NewObject, uint32 BlockID, uint32 ObjectID, float Width, float Height, float Depth);
void Object_SetTexture(uint32 BlockID, uint32 ObjectID, TextureStorage* Texture);
Object* Object_GetObjectPtr(uint32 BlockID, uint32 ObjectID);

#endif