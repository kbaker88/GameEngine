#ifndef OBJECTS_H
#define OBJECTS_H

#include "render_object.h"

#include "box.h"
#include "rectangle.h"
#include "sphere.h"
#include "line.h"
#include "point.h"
#include "plane2d.h"
#include "heightmap.h"

struct RenderObjBlock
{
	RenderObjBlock() : BlockObjects(0), BlockSize(0) {}
	RenderObject** BlockObjects;
	uint32 BlockSize;
};

void RenderObj_CreateBlock(RenderObjBlock* Block, uint32 Size);
void RenderObj_DeleteBlock(RenderObjBlock* Block);
void RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID);
void RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID, Texture2D* HeightMapTexture);
void RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID, uint32 Width, uint32 Depth);
void RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID, float Width, float Height, float Depth);
void RenderObj_SetTexture(RenderObjBlock* Block, uint32 ObjectID,
	Texture2D* Texture);
RenderObject* RenderObj_GetObjectPtr(RenderObjBlock* Block, uint32 ObjectID);

#endif