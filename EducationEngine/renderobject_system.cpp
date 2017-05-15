#include "renderobject_system.h"

void 
RenderObj_CreateBlock(RenderObjBlock* Block, uint32 Size)
{
	if (!Block)
	{
		// TODO: Error
	}
	else
	{
		if (!Block->BlockObjects)
		{
			Block->BlockObjects = new RenderObject*[Size] {};
			Block->BlockSize = Size;
		}
		else
		{
			//TODO: Error checking
		}
	}
}

void 
RenderObj_DeleteBlock(RenderObjBlock* Block)
{
	if (Block->BlockObjects != 0)
	{
		for (uint32 i = 0; i < Block->BlockSize; i++)
		{
			if (Block->BlockObjects[i])
			{
				Block->BlockObjects[i]->Delete();
				delete Block->BlockObjects[i];
			}
		}
		delete[] Block->BlockObjects;
		Block->BlockObjects = 0;
		Block->BlockSize = 0;
	}
}

void 
RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init();
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void
RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID, Texture2D* HeightMapTexture)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init(HeightMapTexture);
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void 
RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID, uint32 Width, uint32 Depth)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init(Width, Depth);
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void 
RenderObj_Create(RenderObject* NewObject, RenderObjBlock* Block,
	uint32 ObjectID, float Width, float Height, float Depth)
{
	Block->BlockObjects[ObjectID] = NewObject;
	Block->BlockObjects[ObjectID]->Init(Width, Height, Depth);
	Block->BlockObjects[ObjectID]->ObjectID = ObjectID;
}

void
RenderObj_SetTexture(RenderObjBlock* Block, uint32 ObjectID,
	Texture2D* Texture)
{
	Block->BlockObjects[ObjectID]->InputTexture(Texture);
}

RenderObject*
RenderObj_GetObjectPtr(RenderObjBlock* Block, uint32 ObjectID)
{
	return Block->BlockObjects[ObjectID];
}
