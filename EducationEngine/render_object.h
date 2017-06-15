#ifndef OBJECT_H
#define OBJECT_H

#include "model_object_system.h"

struct RenderObjBlock
{
	RenderObject** BlockObjects;
	uint32 BlockSize;
};

void
RenderObj_CreateBlock(RenderObjBlock* Block, 
	uint32 Size);
void
RenderObj_DeleteBlock(RenderObjBlock* Block);

void
RenderObj_CreateRenderObject(RenderObject* RenderObj,
	Model* ModelObj);
void
RenderObj_Delete(RenderObject* RenderObj);

#endif
