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

struct ObjectBlock
{
	ObjectBlock() : BlockObjects(0), BlockSize(0) {}
	Object** BlockObjects;
	uint32 BlockSize;
};

void Object_CreateBlock(ObjectBlock* Block, uint32 Size);
void Object_DeleteBlock(ObjectBlock* Block);
void Object_Create(Object* NewObject, ObjectBlock* Block,
	uint32 ObjectID);
void Object_Create(Object* NewObject, ObjectBlock* Block,
	uint32 ObjectID, Texture2D* HeightMapTexture);
void Object_Create(Object* NewObject, ObjectBlock* Block, 
	uint32 ObjectID, uint32 Width, uint32 Depth);
void Object_Create(Object* NewObject, ObjectBlock* Block,
	uint32 ObjectID, float Width, float Height, float Depth);
void Object_SetTexture(ObjectBlock* Block, uint32 ObjectID, 
	Texture2D* Texture);
Object* Object_GetObjectPtr(ObjectBlock* Block, uint32 ObjectID);

#endif